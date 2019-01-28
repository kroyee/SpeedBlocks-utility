#ifndef PACKETMACRO_HPP
#define PACKETMACRO_HPP

#include "../Trigger/Trigger.hpp"

// inline static CallbackCollection<type> callbacks;

#define PACKET(TYPE)                         \
    struct TYPE;                             \
    template <>                              \
    struct os::PacketID<__COUNT##ER__ + 1> { \
        using type = TYPE;                   \
    };                                       \
    struct TYPE

#endif  // PACKETMACRO_HPP