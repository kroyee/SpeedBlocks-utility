#ifndef SERIALIZE
#define SERIALIZE

#include <type_traits>
#include "../helpers/BindToMembers.hpp"
#include "../helpers/MyTypeTraits.hpp"

namespace Stream {

template <class Object>
using has_serialize_impl = decltype(std::declval<Object>().serialize(1));

template <class T>
constexpr bool has_serialize_v = os::detect_v<has_serialize_impl, T>;

template <class LHS, class RHS>
using has_left_stream = decltype(std::declval<LHS>() << std::declval<RHS>());

template <class LHS, class RHS>
constexpr bool has_left_stream_v = os::detect_v<has_left_stream, LHS, RHS>;

template <class LHS, class RHS>
using has_right_stream = decltype(std::declval<LHS>() >> std::declval<RHS>());

template <class LHS, class RHS>
constexpr bool has_right_stream_v = os::detect_v<has_left_stream, LHS, RHS>;

template <class Stream>
struct To {
    To(Stream& stream) : m_stream(stream) {}
    To(Stream&& stream) : m_stream(stream) {}

    template <class Object>
    To& operator<<(Object&& object) {
        if constexpr (has_serialize_v<Object>) {
            object.serialize([&](const auto&... args) { (m_stream << ... << args); });
        } else if constexpr (has_left_stream_v<Stream, Object>) {
            m_stream << object;
        } else {
            os::bind_to_members([&](const auto&... args) { (m_stream << ... << args); }, object);
        }
        return *this;
    }

    template <class Object>
    To& operator>>(Object&& object) {
        if constexpr (has_serialize_v<Object>) {
            object.serialize([&](const auto&... args) { (m_stream >> ... >> args); });
        } else if constexpr (has_left_stream_v<Stream, Object>) {
            m_stream >> object;
        } else {
            os::bind_to_members([&](const auto&... args) { (m_stream >> ... >> args); }, object);
        }
        return *this;
    }

   private:
    Stream& m_stream;
};
}  // namespace Stream

#endif
