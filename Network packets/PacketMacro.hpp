#ifndef PACKETMACRO_HPP
#define PACKETMACRO_HPP

#include <functional>
#include <string>

// inline static CallbackCollection<type> callbacks;

#define PACKET(TYPE)                                                            \
    struct TYPE;                                                                \
    template <class PacketClass>                                                \
    template <bool Dummy>                                                       \
    struct os::PacketManager<PacketClass>::PacketID<__COUNT##ER__ + 1, Dummy> { \
        using type = TYPE;                                                      \
        static inline std::function<void(PacketClass&)> target;                 \
        static inline const std::string name = #TYPE;                           \
    };                                                                          \
    struct TYPE

#define REGISTER(TYPE)                                                                   \
    template <class PacketClass>                                                         \
    template <bool Dummy>                                                                \
    struct os::PacketManager<PacketClass>::template PacketID<__COUNT##ER__ + 1, Dummy> { \
        using type = TYPE;                                                               \
        static inline std::function<void(PacketClass&)> target;                          \
        static inline const std::string name = #TYPE;                                    \
    }

#define SIGNAL(TYPE)                                                            \
    struct TYPE {};                                                             \
    template <class PacketClass>                                                \
    template <bool Dummy>                                                       \
    struct os::PacketManager<PacketClass>::PacketID<__COUNT##ER__ + 1, Dummy> { \
        using type = TYPE;                                                      \
        static inline std::function<void(PacketClass&)> target;                 \
        static inline const std::string name = #TYPE;                           \
    }

#endif  // PACKETMACRO_HPP