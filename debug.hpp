#ifndef MY_DEBUG_HPP
#define MY_DEBUG_HPP

#ifndef DNDEBUG
#include <iostream>
#include <typeinfo>
#define CERR std::cerr << __FILE__ << ":" << \
  std::dec << __LINE__ << ":" <<__FUNCTION__ << "() : "
#define CCERR std::cerr << __FILE__ << ":" << \
  std::dec << __LINE__ << ":" << typeid(*this).name() << "::"  <<__FUNCTION__ << "() : "
#else
#include <iostream>
#define CERR std::cerr
#define CCERR std::cerr
#endif // DNDEBUG

#endif // header guard
