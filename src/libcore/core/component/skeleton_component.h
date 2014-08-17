#pragma once

#include "component.h"

namespace atom {

struct Bone {
  String name;
  Vec3f  local_head;
  Vec3f  x;
  Vec3f  y;
  Vec3f  z;
  i32    parent;
  Quatf  transform;
};


class SkeletonComponent : public Component {
  String             my_mesh_name;
  std::vector<Bone>  my_bones;
  std::vector<Mat4f> my_transforms;

public:
  explicit SkeletonComponent(const String &mesh);

  Slice<Mat4f> get_transforms() const;

  void recalculate_skeleton();

  Bone* find_bone(const String &name);

private:
  void activate() override;

  void deactivate() override;

  uptr<Component> clone() const override;

  Mat4f calculate_bone_matrix(const Bone &bone) const;
};

MAP_COMPONENT_TYPE(SkeletonComponent, SKELETON)

}
