#include "script_processor.h"
#include <cassert>
#include <algorithm>
#include "script_component.h"

namespace atom {

ScriptProcessor::ScriptProcessor(World &world)
  : NullProcessor(world)
  , my_is_started(false)
{
}

void ScriptProcessor::register_script(ScriptComponent *component)
{
  assert(component != nullptr);
  my_scripts.push_back(component);
}

void ScriptProcessor::unregister_script(ScriptComponent *component)
{
  assert(component != nullptr);
  my_scripts.erase(std::remove(my_scripts.begin(), my_scripts.end(), component),
    my_scripts.end());
}

void ScriptProcessor::activate()
{
  my_is_started = true;
}

void ScriptProcessor::poll()
{
  if (!my_is_started) {
    return;
  }

  for (ScriptComponent *script : my_scripts) {
    script->update();
  }
}

}
