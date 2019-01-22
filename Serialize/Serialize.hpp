#ifndef SERIALIZE
#define SERIALIZE

#include <type_traits>
#include "../helpers/BindToMembers.hpp"
#include "../helpers/MyTypeTraits.hpp"

namespace Stream {

template <class Object>
using has_serialize = decltype(std::declval<Object>().serialize(1));

template <class T>
constexpr bool has_serialize_v = os::detect_v<has_serialize, T>;

template <class LHS, class RHS>
using has_left_stream = decltype(std::declval<LHS>() << std::declval<RHS>());

template <class LHS, class RHS>
constexpr bool has_left_stream_v = os::detect_v<has_left_stream, LHS, RHS>;

template <class LHS, class RHS>
using has_right_stream = decltype(std::declval<LHS>() >> std::declval<RHS>());

template <class LHS, class RHS>
constexpr bool has_right_stream_v = os::detect_v<has_left_stream, LHS, RHS>;

template <class T>
using is_container = decltype(std::declval<T>().begin(), std::declval<T>().end(), std::declval<T>().size());

template <class T>
constexpr bool is_container_v = (os::detect_v<is_container, T> && !std::is_same_v<std::decay_t<T>, std::string>);

template <class Stream>
struct To {
    To(Stream& stream) : m_stream(stream) {}
    To(Stream&& stream) : m_stream(stream) {}

    template <class Object>
    To& operator<<(Object&& object) {
        if constexpr (is_container_v<Object>) {
            *this << object.size();
            for (auto& val : object) {
                *this << val;
            }
        } else if constexpr (has_serialize_v<Object>) {
            object.serialize([&](auto&&... args) { (*this << ... << args); });
        } else if constexpr (has_left_stream_v<Stream, Object>) {
            m_stream << object;
        } else {
            os::bind_to_members([&](auto&&... args) { (*this << ... << args); }, object);
        }
        return *this;
    }

    template <class Object>
    To& operator>>(Object&& object) {
        if constexpr (is_container_v<Object>) {
            std::size_t count;
            *this >> count;
            object.resize(count);
            for (auto& val : object) {
                *this >> val;
            }
        } else if constexpr (has_serialize_v<Object>) {
            object.serialize([&](auto&&... args) { (*this >> ... >> args); });
        } else if constexpr (has_left_stream_v<Stream, Object>) {
            m_stream >> object;
        } else {
            os::bind_to_members([&](auto&&... args) { (*this >> ... >> args); }, object);
        }
        return *this;
    }

   private:
    Stream& m_stream;
};
}  // namespace Stream

#endif
