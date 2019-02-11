#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>

#ifdef DEBUG
template <class F>
void DEBUG(F&& f) {
    f();
}
#else
template <class F>
void DEBUG(F) {}
#endif

#endif