#include <iostream>
#include <iomanip>

// Warning : since we're in a module constructor, some objects may still be uninitialized, like std::cout
extern "C"
void __interflop_init()
{
  printf("HERE\n");
  std::srand(time(NULL));
  // We should probably call a runtime constructor here
}
