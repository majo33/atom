#include <iostream>
#include <core/corefwd.h>
#include <core/frame_processor.h>
#include <core/core.h>
#include <core/log.h>
#include <core/game_entry.h>
#include <core/file_watch.h>
#include <core/frame.h>
#include <SDL/SDL.h>

using namespace atom;

extern "C" {
const atom::GameEntry* game_entry();
}

void update_gl_buffers(void *)
{
  SDL_GL_SwapBuffers();
}

int main(int argc, char *argv[])
{
  assert(argc > 0); // prvy parameter musi urcovat spustenu hru

  Core &core = Core::init(InitMode::STANDALONE, game_entry());

  const GameEntry *game_api = game_entry();

  if (game_api == nullptr) {
    log_error("Can't find game entry point");
    return EXIT_FAILURE;
  }

  FramePtr first_frame(game_api->make_first_frame(core));
  if (first_frame == nullptr) {
    log_error("The game library doesn't contain start frame");
    return EXIT_FAILURE;
  }

  FrameProcessor fp(core);
  fp.set_post_frame_callback(update_gl_buffers);
  fp.run(first_frame);
  first_frame = nullptr;
  return 0;
}

