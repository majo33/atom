#pragma once

#include "../foundation.h"
#include "processor.h"

namespace atom {

namespace DebugCategory {
  enum {
    PHYSICS = 1,
    BOUNDING_BOX = 2,
    AABB = 4,
    GEOMETRY_CACHE = 8
  };
}

class PhysicsDebugDrawer;

class DebugProcessor : public NullProcessor {
  u32                      my_debug_categories;
  uptr<PhysicsDebugDrawer> my_physics_drawer;
  MaterialResourcePtr      my_wireframe_material;
  MaterialResourcePtr      my_lines_material;
  MaterialResourcePtr      my_bounding_box_material;
  MaterialResourcePtr      my_aabb_material;

public:
  explicit DebugProcessor(World &world);
  ~DebugProcessor();

  void activate() override;

  void poll() override;

  void draw();

  void set_debug(u32 category, bool enable);

  void draw_wireframe(const Slice<Vec3f> &vertices, const Slice<u32> &indices,
    const Vec3f &color, const Mat4f &model);
  
  void draw_lines(const Slice<Vec3f> &line_points, const Vec3f &color);

private:
  void draw_physics();
  void draw_bounding_box();
  void draw_aabb();
  void draw_geometry_cache();
};

}
