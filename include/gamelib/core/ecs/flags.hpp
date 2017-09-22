#ifndef GAMELIB_ENTITY_FLAGS_HPP
#define GAMELIB_ENTITY_FLAGS_HPP

#include "gamelib/utils/bitflags.hpp"

namespace gamelib
{
    constexpr unsigned int entity_exportcomponents = 1;      // export components in save files
    constexpr unsigned int entity_persistent       = 1 << 1; // unimplemented
    constexpr unsigned int entity_static           = 1 << 2; // unimplemented
}

#endif