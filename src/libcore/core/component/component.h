#pragma once

#include <cassert>
#include <vector>
#include "../foundation.h"

namespace atom {

enum class ComponentType : u32 {
  UNKNOWN,
  MODEL,
  MATERIAL,
  RENDER,
  MESH,
  RAW_MESH,
  GEOMETRY,
  SCRIPT,
  RIGID_BODY,
  SKELETON,
  COLLIDER
};

typedef std::vector<GenericSlot *> SlotArray;

/**
 *
 *
 * Component contains these important methods
 *  - attach
 *  - detach
 *  - duplicate
 *
 *  - (v)init
 *  - (v)activate
 *  - (v)deactivate
 *  - (v)terminate
 *  - (v)clone
 * 
 * Lifecycle:
 *  - init (Entity has been put into world)
 *  - activate (World has been activated)
 *  - deactivate (World has been deactivated, finished)
 *  - terminate (Entity is going to be removed from World)
 *
 * Properties of component:
 *  - constructor with no parameters
 *  - clone method
 *  - controlled activation/deactivation through activate/deactivate method
 *  - component has distinct types
 *  - access to parent entity (entity is valid between calls activate/deactivate)
 */
class Component : NonCopyable {
  ComponentType my_type;
  Entity       *my_entity;
  String        my_name;
  SlotArray     my_slots;
  
  virtual void init() = 0;

  // empty implementation
  virtual void activate() = 0;

  // empty implementation
  virtual void deactivate() = 0;
  
  virtual void terminate() = 0;

  virtual uptr<Component> clone() const = 0;

public:
  META_DECLARE_CLASS_PTR; // each instance contains pointer to the MetaClass
  META_DECLARE_CLASS;     // static instance of MetaClass for Material
  
  // doesn't need to be private because it is abstract class
  Component(ComponentType type);
  virtual ~Component();
  
  uptr<Component> duplicate() const;

  void attach(Entity &entity);
  void detach();

  // volat len po welcome a pred goodbye
  Entity& entity() const;

  World& world() const;

  Core& core() const;

  WorldProcessorsRef processors() const;

  ComponentType type() const;

  const String& name() const;

  void set_name(const String &name);

  void register_slot(GenericSlot *slot);
};


class NullComponent : public Component {
  void init() override
  {
    // empty
  }
  
  // empty implementation
  void activate() override
  {
    // empty
  }

  // empty implementation
  void deactivate() override
  {
    
  }
  
  void terminate() override
  {
    // empty
  }
    
protected:
  NullComponent(ComponentType type)
    : Component(type)
  {
    // empty
  }
  
  ~NullComponent()
  {
    // empty
  }
};

MAP_TYPE(ComponentType, COMPONENT_TYPE)

template<typename T>
ComponentType component_type_of();

#define MAP_COMPONENT_TYPE(type, mapped)  \
  template<>                    \
  inline ComponentType component_type_of<type>()   \
  { return ComponentType::mapped; }


/**
 * See @ref Slot class
 */
class GenericSlot : NonCopyable {
  Component    *my_component;
  ComponentType my_type;
  String        my_name;  ///< component name

public:
  explicit GenericSlot(Component *parent, ComponentType type,
    const String &name = String())
    : my_component(nullptr)
    , my_type(type)
    , my_name(name)
  {
    assert(parent != nullptr);
    parent->register_slot(this);
  }

  void activate(Entity &entity);

  Component* get() const;

  operator bool() const
  {
    return my_component != nullptr;
  }
};

/**
 *
 * Important methods:
 *  - activate
 */
template<typename T>
class Slot : public GenericSlot {
public:
  explicit Slot(Component *parent, const String &name = String())
    : GenericSlot(parent, component_type_of<T>(), name)
  {
    // if you've got compile error -> probably trying to use nonexisting component
  }

  T* operator->() const
  {
    T* component = static_cast<T *>(get());
    assert(component != nullptr);
    return component;
  }
};

}
