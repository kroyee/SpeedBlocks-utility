#ifndef PACKETMACRO_HPP
#define PACKETMACRO_HPP

#include "../Trigger/Trigger.hpp"

#define PACKET(TYPE, N)                                   \
    struct TYPE;                                          \
    template <>                                           \
    struct os::PacketID<N> {                              \
        using type = TYPE;                                \
        inline static CallbackCollection<type> callbacks; \
    };                                                    \
    struct TYPE

#endif  // PACKETMACRO_HPP