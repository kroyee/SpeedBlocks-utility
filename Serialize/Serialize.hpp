#ifndef SERIALIZE
#define SERIALIZE

#include "../helpers/BindToMembers.hpp"
#include "../helpers/MyTypeTraits.hpp"

namespace Stream {

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

namespace detail {

template <class T>
struct ContainerTypeWrap {};

template <typename T, class = void>
struct ContainerType_impl {
    using type = T;
};

template <typename T>
struct ContainerType_impl<T, std::enable_if_t<is_container_v<T>>> {
    using type = ContainerTypeWrap<typename std::decay_t<T>::value_type>;
};

template <typename T>
using ContainerType = typename ContainerType_impl<T>::type;

struct ArgsTypeGetter {
    template <class... Args>
    auto operator()(Args&&...) {
        return os::TypePack<ContainerType<Args>...>{};
    }
};

}  // namespace detail

template <class Object>
using has_serialize = decltype(std::declval<Object>().serialize(detail::ArgsTypeGetter{}));

template <class T>
constexpr bool has_serialize_v = os::detect_v<has_serialize, T>;

template <class T, class U>
struct StreamAs {
    StreamAs(U& o) : object(o) {}
    U& object;
};

template <class T, class U>
auto As(U&& u) {
    return StreamAs<T, U>(u);
}

namespace detail {

template <class Stream>
struct StreamTypes {
    template <class T>
    static auto get() {
        if constexpr (is_container_v<T>) {
            return ContainerType<T>{};
        } else if constexpr (has_serialize_v<T>) {
            return decltype(std::declval<T>().serialize(ArgsTypeGetter{})){};
        } else if constexpr (has_left_stream_v<Stream, T>) {
            return T{};
        } else {
            return decltype(os::bind_to_members(ArgsTypeGetter{}, std::declval<T>())){};
        }
    }
};

template <class Stream, class LHS, class RHS, class = void>
struct stream_compatible {
    static constexpr bool value = std::is_same_v<decltype(StreamTypes<Stream>::template get<LHS>()), decltype(StreamTypes<Stream>::template get<RHS>())>;
};

template <class Stream, class LHS, class RHS>
struct stream_compatible<Stream, LHS, RHS, std::enable_if_t<std::is_same_v<LHS, RHS>>> {
    static constexpr bool value = true;
};

}  // namespace detail

template <class Stream, class LHS, class RHS>
constexpr bool stream_compatible_v = detail::stream_compatible<Stream, LHS, RHS>::value;

template <class Stream>
struct To {
    template <class Object, class AsType = Object>
    To& operator<<(Object&& object) {
        static_assert(stream_compatible_v<Stream, Object, AsType>, "Stream::As with incompatible types");
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

    template <class AsType, class Object>
    To& operator<<(StreamAs<AsType, Object> object) {
        To::operator<<<Object&, AsType>(object.object);
    }

    template <class Object, class AsType = Object>
    To& operator>>(Object&& object) {
        static_assert(stream_compatible_v<Stream, Object, AsType>, "Stream::As with incompatible types");
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

    template <class AsType, class Object>
    To& operator>>(StreamAs<AsType, Object> object) {
        To::operator>><Object&, AsType>(object.object);
    }

    operator Stream&() { return m_stream; }

    Stream* operator->() { return &m_stream; }

    operator bool() { return m_stream; }

   private:
    Stream m_stream;
};

}  // namespace Stream

#endif
