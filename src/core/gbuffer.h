#pragma once

#include "corefwd.h"
#include "noncopyable.h"
#include "framebuffer.h"
#include "texture.h"

namespace atom {

class GBuffer : NonCopyable {
public:
  explicit GBuffer(VideoService &vs);

  void set_resolution(int width, int height);

  Framebuffer& get_render_framebuffer();

  void blit();

private:
  VideoService &my_vs;
  bool          my_is_ready;
  Framebuffer   my_depth_pass;
  Framebuffer   my_render_pass;
  Texture       my_albedo;
  Texture       my_material;
  Texture       my_normal;
  Texture       my_depth_stencil;
};

}
