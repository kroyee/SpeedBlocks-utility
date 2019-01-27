#ifndef PACKETMACRO_HPP
#define PACKETMACRO_HPP

#include "../Trigger/Trigger.hpp"

// inline static CallbackCollection<type> callbacks;

#define PACKET(TYPE, N)      \
    struct TYPE;             \
    template <>              \
    struct os::PacketID<N> { \
        using type = TYPE;   \
    };                       \
    struct TYPE

#endif  // PACKETMACRO_HPP