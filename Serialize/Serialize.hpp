#ifndef SERIALIZE
#define SERIALIZE

#include "../helpers/BindToMembers.hpp"
#include "../helpers/MyTypeTraits.hpp"

#ifdef DEBUG
#include <algorithm>
#include <typeindex>
#include <typeinfo>
#endif

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

template <class T, class U>
struct StreamAs {
    StreamAs(U& o) : object(o) {}
    U& object;
};

template <typename T>
struct ContainerType_impl {};

template <typename T>
using ContainerType = ContainerType_impl<typename std::decay_t<T>::value_type>;

template <class T>
using GetType = std::conditional_t<is_container_v<T>, ContainerType<T>, T>;

#ifdef DEBUG

template <class Stream>
struct TestStreamAs {
    template <typename Object>
    TestStreamAs& operator<<(Object&& object) {
        if constexpr (is_container_v<Object>) {
            ContainerType<Object> tmp;
            types.push_back(std::type_index(typeid(tmp)));
        } else if constexpr (has_serialize_v<Object>) {
            object.serialize([&](auto&&... args) { (*this << ... << args); });
        } else if constexpr (has_left_stream_v<Stream, Object>) {
            types.push_back(std::type_index(typeid(object)));
        } else {
            os::bind_to_members([&](auto&&... args) { (*this << ... << args); }, object);
        }
        return *this;
    }

    TestStreamAs& sort() {
        std::sort(types.begin(), types.end());
        return *this;
    }

    bool operator==(const TestStreamAs& other) {
        if (types.size() == other.types.size()) {
            if (std::includes(types.begin(), types.end(), other.types.begin(), other.types.end())) return true;
        }
        return false;
    }

    bool operator!=(const TestStreamAs& other) { return !(*this == other); }

   private:
    std::vector<std::type_index> types;
};

template <class T, class U>
auto As(U&& u) {
    return StreamAs<T, U>(u);
}

#else  // ifdef DEBUG

template <class T, class U>
auto&& As(U&& u) {
    return std::forward<U>(u);
}

#endif  // ifdef DEBUG

template <class Stream>
struct StreamTypes {
    template <class T>
    static auto get() {
        if constexpr (is_container_v<T>) {
            return ContainerType<T>{};
        } else if constexpr (has_serialize_v<T>) {
            return decltype(std::declval<T>().serialize([&](auto&&... args) {
                (*this << ... << args);
                return TypePack<GetType<decltype(args)>...>{};
            })){};
        } else if constexpr (has_left_stream_v<Stream, T>) {
            return T{};
        } else {
            return decltype(os::bind_to_members(
                [&](auto&&... args) {
                    (*this << ... << args);
                    return TypePack<GetType<decltype(args)>...>{};
                },
                std::declval<T>())){};
        }
    }
}

template <class Stream, class LHS, class RHS>
constexpr bool stream_compatible_v = std::is_same_v<decltype(StreamTypes<Stream>::get<LHS>()), decltype(StreamTypes<Stream>::get<RHS>())>;

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
    To& operator<<(StreamAs<AsType, Object>&& object) {
#ifdef DEBUG
        TestStreamAs<Stream> t1, t2;
        AsType o;
        t1 << o;
        t2 << object.object;
        if (t1.sort() != t2.sort()) {
            std::cout << "Stream::As with invalid objects: " << typeid(o).name() << " and " << typeid(object.object).name() << std::endl;
        }
#endif
        *this ::operator<<<Object, AsType>(object.object);
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
    To& operator>>(StreamAs<AsType, Object>&& object) {
#ifdef DEBUG
        TestStreamAs<Stream> t1, t2;
        AsType o;
        t1 << o;
        t2 << object.object;
        if (t1.sort() != t2.sort()) {
            std::cout << "Stream::As with invalid objects: " << typeid(o).name() << " and " << typeid(object.object).name() << std::endl;
        }
#endif
        *this ::operator>><Object, AsType>(object.object);
    }

    operator bool() { return m_stream; }

   private:
    Stream m_stream;
};

}  // namespace Stream

#endif
