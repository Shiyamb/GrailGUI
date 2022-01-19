#include "util/EnumCounter.hh"
#include <iostream>

enum class Type { UINT8, UINT16 };

template <typename E, E V>
constexpr void f() {
#ifdef _MSC_VER
#ifndef __INTEL_COMPILER
  // I believe this should act similarly to gcc/clang's __PRETTY_FUNCTION__ but
  // testing needs to be done to check
  const char* s = __FUNCSIG__;
#endif
#else
  const char* s = __PRETTY_FUNCTION__;
#endif
}

int main() {
  f<Type, Type::UINT16>();
  f<Type, (Type)3>();

  std::cout << "Type has an element at index 0? " << EnumCounter::is_valid<Type, (Type)0>()
            << std::endl;
  std::cout << "Type has an element at index 120? "
            << EnumCounter::is_valid<Type, (Type)120>() << std::endl;
  std::cout << "Type has " << EnumCounter::element_count<Type>::val << " elements\n";
  std::cout << "Type has " << EnumCounter::enum_class_size<Type>() << " elements\n";
}