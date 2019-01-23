#ifndef NETWORKPACKETHANDLER_HPP
#define NETWORKPACKETHANDLER_HPP

#include <functional>
#include <type_traits>
#include <vector>

#define PACKET(TYPE, N)                                                  \
    struct TYPE;                                                         \
    template <>                                                          \
    struct os::PacketID<N> {                                             \
        using type = TYPE;                                               \
        inline static std::vector<std::function<void(type&)>> callbacks; \
    };                                                                   \
    struct TYPE

namespace os {

template <uint8_t N>
struct PacketID {
    using type = void;
};

template <typename T, int N = 255, std::enable_if_t<N == 0, int> = 0>
constexpr int get_packet_id() {
    return 0;
}

template <typename T, int N = 255, std::enable_if_t<N != 0, int> = 0>
constexpr int get_packet_id() {
    if constexpr (std::is_same_v<typename PacketID<N>::type, std::decay_t<T>>) return N;

    return get_packet_id<T, N - 1>();
}

template <class T, class Func>
void handle_packet(Func f) {
    PacketID<get_packet_id<T>()>::callbacks.emplace_back(f);
}

template <typename PacketClass>
class PacketManager {
   public:
    static void init() { get_registered_functions<255>(); }

    static void read(PacketClass& packet) {
        int id;
        while (packet >> id) {
            packet_array_stream_right[id](packet);
        }
    }

    template <typename Packet>
    static void write(Packet&& data) {
        m_packet << get_packet_id<Packet>() << std::forward<Packet>(data);
    }

    static PacketClass& get_packet() { return m_packet; }

    static void clear() { m_packet = PacketClass{}; }

   private:
    template <int N>
    static void get_registered_functions() {
        if constexpr (!std::is_same_v<typename PacketID<N>::type, void>) {
            packet_array_stream_right[N] = [](PacketClass& packet) {
                typename PacketID<N>::type val;
                packet >> val;
                for (auto& callback : PacketID<N>::callbacks) {
                    // if (callback.first) callback.second(val);
                    callback(val);
                }
            };
        }
        if constexpr (N != 0) get_registered_functions<N - 1>();
    }

    inline static std::array<std::function<void(PacketClass&)>, 256> packet_array_stream_right;

    inline static PacketClass m_packet;
};

}  // namespace os

#endif  // NETWORKPACKETHANDLER_HPP