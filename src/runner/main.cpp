#include <iostream>
#include <core/corefwd.h>
#include <core/frame_processor.h>
#include <core/system/core.h>
#include <core/log.h>
#include <core/system/game_api.h>
#include <core/system/file_watch.h>
#include <core/frame.h>
#include <SDL/SDL.h>

using namespace atom;

extern "C" {
const atom::GameAPI* entry_point();
}

void update_gl_buffers(void *)
{
  SDL_GL_SwapBuffers();
}

int main(int argc, char *argv[])
{
  assert(argc > 0); // prvy parameter musi urcovat spustenu hru

  try {
    Core &core = Core::init(InitMode::STANDALONE, entry_point());

    const EntryPoint *game_api = entry_point();

      assert(game_api != nullptr);  //QQQ pridat if
      FramePtr first_frame(game_api->make_first_frame(core));
      if (first_frame == nullptr) {
        log::error("The game library doesn't contain start frame");
        return EXIT_FAILURE;
      }

      FrameProcessor fp(core);
      fp.set_post_frame_callback(update_gl_buffers);
      //QQQ memory leak
      fp.run(first_frame);
      first_frame = nullptr;

  } catch (std::exception &e) {
    std::cerr << "Error occured! - " << e.what() << std::endl;
    return -1;
  }

  return 0;
}
