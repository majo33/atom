#include "entity.h"

#include "core/video/mesh.h"
#include "core/video/uniforms.h"
#include "core/component/component.h"

namespace atom {

META_DEFINE_FIELDS(Entity) {
  FIELD(Entity, my_id, "id"),
  FIELD(Entity, my_class, "class"),
  FIELD(Entity, my_position, "position"),
  FIELD(Entity, my_rotation, "rotation")
};

META_DEFINE_ROOT_CLASS(Entity, "Entity");

Entity::Entity(World &world, Core &core, f32 width, f32 height)
  : my_world(world)
  , my_core(core)
{
  META_INIT();
  init(width, height);
}

Entity::~Entity()
{
}

void Entity::welcome()
{
  for (Component *component : my_components) {
    component->attach();
  }

  on_welcome();
}

void Entity::goodbye()
{
  on_goodbye();

  for (Component *component : my_components) {
    component->detach();
  }
}

void Entity::init()
{
  update_bounding_box();
  on_init();
}

void Entity::update()
{
  on_update();
}

//void Entity::wake_up()
//{
//  assert(my_state == State::NEW);
//  my_state = State::RUNNING;

//  for (const sptr<Component> &component : my_components) {
//    component->wake_up();
//  }
//}

const String& Entity::id() const
{
  return my_id;
}

void Entity::set_id(const String &id)
{
  my_id = id;
}

Vec2f Entity::position() const
{
  return my_position;
}

void Entity::set_position(const Vec2f position)
{
  my_position = position;
}

void Entity::set_size(f32 width, f32 height)
{
  my_width = width;
  my_height = height;
}

void Entity::update_bounding_box()
{
  my_bounding_box = BoundingBox::from_params(my_position, my_width, my_height);
}

const String &Entity::class_name() const
{
  return my_class;
}

void Entity::set_class_name(const String &class_name)
{
  my_class = class_name;
}

World& Entity::world() const
{
  return my_world;
}

BoundingBox Entity::bounding_box() const
{
  return my_bounding_box;
}

bool Entity::is_live() const
{
  return my_state == State::RUNNING;
}

Core& Entity::core() const
{
  return my_core;
}

void Entity::register_component(Component *component)
{
  assert(component != nullptr);
  my_components.push_back(component);
}

void Entity::init(f32 width, f32 height, const Vec2f &position, f32 rotation)
{
  my_state = State::NEW;
  my_width = width;
  my_height = height;
  my_class = "Base class";
  my_position = position;
  my_rotation = rotation;
  META_INIT();
}

void Entity::on_welcome()
{
  // empty
}

void Entity::on_goodbye()
{
  // empty
}

void Entity::on_init()
{
  // empty
}

void Entity::on_update()
{
  // empty
}

}