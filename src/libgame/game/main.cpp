#include <core/constants.h>
#include <core/log.h>
#include <core/frame.h>
#include <core/system/game_api.h>
#include <core/component/mesh_component.h>
#include <core/component/render_component.h>
#include <core/system/resource_service.h>
#include <core/component/material_component.h>
#include <core/component/script_component.h>
#include <core/component/skeleton_component.h>

using namespace atom;

namespace atom {
namespace {

class AnimalScript : public ScriptComponent {
  u32 my_tick;
  Slot<SkeletonComponent> my_skeleton;

  uptr<Component> clone() const override
  {
    return uptr<Component>(new AnimalScript());
  }

public:
  AnimalScript()
    : my_skeleton(this)
    , my_tick(0)
  {

  }

  void update() override
  {
    ++my_tick;
    f32 angle1 = my_tick / 20.0f;
    f32 angle2 = my_tick / 40.0f;

//    for (Mat4f &m : my_skeleton->my_transforms) {
//      m = Mat4f::rotation_z(angle1);
//    }

//    my_skeleton->my_transforms[0] = Mat4f::rotation_z(angle2);
    my_skeleton->my_transforms[2] = Mat4f::rotation_z(angle2);
    my_skeleton->my_transforms[1] = Mat4f::rotation_z(angle1);
//    my_skeleton->my_transforms[] = Mat4f::rotation_z(angle);
  }
};



Frame* create_first_frame(Core &core)
{
  return nullptr;
//  return new game::DemoFrame(core);
  //return new game::MainMenuFrame(core, std::make_shared<game::DemoFrame>(core));
}


class MySimpleUpdate : public ScriptComponent {
  uptr<Component> clone() const override
  {
    return uptr<Component>(new MySimpleUpdate());
  }

public:
  void update() override
  {
    Mat4f t = entity().transform() * Mat4f::translation(0, 0, 0.1f);
    entity().set_transform(t);
  }
};



uptr<Entity> create_test_object(World &world, Core &core)
{
  uptr<Entity> entity(new Entity(world, core));
  // suzanne
  uptr<MaterialComponent> material(new MaterialComponent("animal"));
  uptr<MeshComponent> mesh(new MeshComponent("animal"));
  uptr<SkeletonComponent> skeleton(new SkeletonComponent("animal"));
  uptr<RenderComponent> render(new RenderComponent());
  uptr<ScriptComponent> script(new AnimalScript());
  entity->add_component(std::move(material));
  entity->add_component(std::move(mesh));
  entity->add_component(std::move(skeleton));
  entity->add_component(std::move(render));
  entity->add_component(std::move(script));
  return entity;
}

uptr<Entity> create_suzanne(World &world, Core &core)
{
  uptr<Entity> entity(new Entity(world, core));
  // suzanne
  uptr<MaterialComponent> material(new MaterialComponent("test2"));
  uptr<MeshComponent> mesh(new MeshComponent("suzanne"));
  uptr<RenderComponent> render(new RenderComponent());
  entity->add_component(std::move(material));
  entity->add_component(std::move(mesh));
  entity->add_component(std::move(render));
  return entity;
}

std::vector<EntityCreator> create_object_creators(Core &)
{
  std::vector<EntityCreator> creators;

  creators.push_back(EntityCreator("TestObject", create_test_object));
  creators.push_back(EntityCreator("Suzanne", create_suzanne));
//  creators.push_back(EntityCreator("Box", game::create_box));
//  creators.push_back(EntityCreator("Static Box", game::create_static_box));
//  creators.push_back(EntityCreator("Platform", game::create_simple_static_platform));
//  creators.push_back(EntityCreator("Moveable", game::MovingPlatform::create));
//  creators.push_back(EntityCreator("FallingPlatform", game::FallingPlatform::create));
//  creators.push_back(EntityCreator("Sticker", game::create_sticker));
//  creators.push_back(EntityCreator("Automatic Camera", game::create_automatic_camera));
//  creators.push_back(EntityCreator("Trampoline", game::Trampoline::create));
  return creators;
}

GameAPI game_api = {
  create_first_frame,
  create_object_creators
};

}
}

extern "C" {

const EntryPoint* entry_point()
{
  return &game_api;
}

}
