#pragma once

#include <vector>
#include "core/corefwd.h"
#include "core/math/mat.h"
#include "processor.h"

namespace atom {

class Box2DDebugDraw;

class PhysicsProcessor : public Processor {
public:
  PhysicsProcessor(VideoService &vs, ResourceService &rs);
  ~PhysicsProcessor();

  void poll() override;

  void start() override;

private:
private:
  bool                              my_is_running;
  bool                              my_is_debug_enabled;
};

}
