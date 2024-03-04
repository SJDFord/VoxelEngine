#include <functional>
#include <glm/glm.hpp>

// TODO: Better name for this
class WindowLibrary
{
public:
  static void pollEvents();

  WindowLibrary() = delete;
};