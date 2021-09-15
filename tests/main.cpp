#include <array>
#include <iostream>
#include <stdio.h>

template <typename T, size_t size>
void __attribute__((noinline))
init_array(std::array<T, size> &a, const size_t shift) {
  for (int i = 0; i < size; i++) {
    a[i] = i + shift;
  }
}

template <typename iterator>
void __attribute__((noinline))
print_array(iterator begin_iterator, iterator end_iterator) {

  size_t i = 0;
  for (iterator it = begin_iterator; it != end_iterator; ++it) {
    std::cout << i << " : " << *it << "\n";
    i++;
  }
}

int main() {

  constexpr size_t NElem = 16;

  std::array<double, NElem> a;
  std::array<double, NElem> b;

  init_array<double, NElem>(a, 0);
  init_array<double, NElem>(b, NElem);

  std::array<double, NElem> c = {0};
  for (int i = 0; i < NElem; i++) {
    c[i] = a[i] * b[i];
  }

  print_array(c.begin(), c.end());

  return 0;
}