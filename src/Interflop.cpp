#include <iostream>
#include <iomanip>

extern size_t __nsan_shadow_scale;


// Warning : since we're in a module constructor, some objects may still be uninitialized, like std::cout
extern "C"
void __interflop_init()
{
  std::srand(time(NULL));
  // __nsan_shadow_scale = 4; // We could dynamically change the shadow size
  // We should probably call a runtime constructor here
}
