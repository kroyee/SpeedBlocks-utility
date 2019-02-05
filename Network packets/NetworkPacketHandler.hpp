#ifndef NETWORKPACKETHANDLER_HPP
#define NETWORKPACKETHANDLER_HPP

#include <functional>
#include <string>
#include <type_traits>
#include <vector>
#include "../Serialize/Serialize.hpp"
#include "../Trigger/Trigger.hpp"
#include "../helpers/MyTypeTraits.hpp"
#include "PacketMacro.hpp"

namespace os {

template <typename PacketClass>
class PacketManager {
   public:
    template <uint8_t N, bool dummy = true>  // Dummy is only there to allow partial specialization on N (needed in PacketMacro)
    struct PacketID {
        using type = void;
    };

    static void init() { get_registered_functions(); }

    void read() {
        uint8_t id;
        while (m_packet >> id) {
            packet_array_stream_right[id](packet);
        }
    }

    template <typename Packet>
    void write(Packet&& data) {
        if constexpr (std::is_empty_v<Packet>) {
            m_packet << get_packet_id<Packet>();
        } else {
            m_packet << get_packet_id<Packet>() << std::forward<Packet>(data);
        }
    }

    template <class AsType, class Data>
    void write_as(Data&& data) {
        if constexpr (std::is_empty_v<AsType>) {
            m_packet << get_packet_id<AsType>();
        } else {
            m_packet << get_packet_id<AsType>();
            m_packet.template operator<<<Data, AsType>(data);
        }
    }

    template <typename Packet>
    void write() {
        static_assert(std::is_empty_v<Packet>, "write called without data on non-empty Packet type");
        m_packet << get_packet_id<Packet>();
    }

    template <class Func>
    static auto handle_packet(Func f) {
        using T = std::decay_t<typename LambdaSignature<Func>::Arg>;
        return Event<T>::add(std::move(f));
    }

    template <class T, class Func>
    static auto handle_packet(Func f) {
        return Event<T>::add(std::move(f));
    }

    template <class T, int N = 255>
    static void set_target(std::function<void(PacketClass&)> f) {
        if constexpr (N == 0) {
            static_assert(N != 0, "Failed to set target");
        } else if constexpr (std::is_same_v<typename PacketID<N>::type, T>) {
            PacketID<N>::target = f;
        } else {
            set_target<T, N - 1>(std::move(f));
        }
    }

    PacketClass& get_packet() { return m_packet; }
    typename PacketClass::StreamType& get_base_packet() { return m_packet; }
    operator typename PacketClass::StreamType&() { return m_packet; }

    void clear() { m_packet = PacketClass{}; }

   private:
    template <int N = 255>
    static void get_registered_functions() {
        if constexpr (!std::is_same_v<typename PacketID<N>::type, void>) {
            if constexpr (std::is_empty_v<typename PacketID<N>::type>) {
                packet_array_stream_right[N] = [](PacketClass&) { Event<typename PacketID<N>::type>::trigger(); };
            } else {
                packet_array_stream_right[N] = [](PacketClass& packet) {
                    if (static_cast<bool>(PacketID<N>::target)) {
                        PacketID<N>::target(packet);
                    } else {
                        typename PacketID<N>::type val;
                        packet >> val;
                        Event<typename PacketID<N>::type>::trigger(val);
                    }
                };
            }
        }
        if constexpr (N != 0) get_registered_functions<N - 1>();
    }

    template <typename T, int N = 255>
    static constexpr uint8_t get_packet_id() {
        if constexpr (N == 0) {
            return 0;
        } else if constexpr (std::is_same_v<typename PacketID<N>::type, std::decay_t<T>>) {
            return N;
        } else {
            return get_packet_id<T, N - 1>();
        }
    }

    inline static std::array<std::function<void(PacketClass&)>, 256> packet_array_stream_right;

    PacketClass m_packet;
};

}  // namespace os

#endif  // NETWORKPACKETHANDLER_HPP