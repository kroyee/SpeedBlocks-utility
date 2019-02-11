#ifndef NETWORKPACKETHANDLER_HPP
#define NETWORKPACKETHANDLER_HPP

#include <functional>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>
#include "../Debug/Debug.hpp"
#include "../Serialize/Serialize.hpp"
#include "../Trigger/Trigger.hpp"
#include "../helpers/MyTypeTraits.hpp"
#include "PacketMacro.hpp"

namespace os {

namespace detail {

template <class Is, class T>
struct PackAsObject;

template <std::size_t... Is, class T>
struct PackAsObject<std::index_sequence<Is...>, T> {
    PackAsObject(T&& t) : tuple(t) {}
    T& tuple;

    template <class F>
    auto serialize(F f) {
        return f(std::get<Is>(tuple)...);
    }
};

template <class T>
PackAsObject(T&& t)->PackAsObject<std::make_index_sequence<std::tuple_size_v<T>>, T>;

}  // namespace detail

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
            DEBUG([&]() {
                if (id != get_packet_id<NP_Ping>() && id != get_packet_id<NP_Gamestate>() && id) {
                    std::cout << "Got packet " << packet_names[id] << std::endl;
                }
            });
            packet_array_stream_right[id](m_packet);
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

    template <class AsType, class... Data>
    void write_as(Data&&... data) {
        m_packet << get_packet_id<AsType>();
        if constexpr (sizeof...(Data) == 1) {
            m_packet.template operator<<<Data..., AsType>(std::forward<Data>(data)...);
        } else {
            auto o = detail::PackAsObject(std::forward_as_tuple(std::forward<Data>(data)...));
            m_packet.template operator<<<decltype(o), AsType>(std::move(o));
        }
    }

    template <typename Packet>
    void write() {
        static_assert(std::is_empty_v<Packet>, "write called without data on non-empty Packet type");
        m_packet << get_packet_id<Packet>();
    }

    template <class AsType, class... Data>
    static PacketManager make(Data&&... data) {
        PacketManager packet;
        packet.m_packet << get_packet_id<AsType>();
        if constexpr (sizeof...(Data) == 1) {
            packet.m_packet.template operator<<<Data..., AsType>(std::forward<Data>(data)...);
        } else if constexpr (sizeof...(Data) > 1) {
            auto o = detail::PackAsObject(std::forward_as_tuple(std::forward<Data>(data)...));
            packet.m_packet.template operator<<<decltype(o), AsType>(std::move(o));
        } else {
            static_assert(std::is_empty_v<AsType>, "AsType is not empty, but no parameters passed in");
        }
        return packet;
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
    operator const typename PacketClass::StreamType&() const { return m_packet; }

    void clear() { m_packet = PacketClass{}; }

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
            packet_names[N] = PacketID<N>::name;
        } else {
            packet_array_stream_right[N] = [](PacketClass&) { std::cout << "Unknown packet " << N << std::endl; };
        }
        if constexpr (N != 0) get_registered_functions<N - 1>();
    }

    inline static std::array<std::function<void(PacketClass&)>, 256> packet_array_stream_right;
    inline static std::array<std::string, 256> packet_names;

    PacketClass m_packet;
};

}  // namespace os

#endif  // NETWORKPACKETHANDLER_HPP