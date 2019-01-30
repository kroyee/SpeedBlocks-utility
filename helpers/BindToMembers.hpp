#ifndef BINDTOMEMBERS_HPP
#define BINDTOMEMBERS_HPP

#include <type_traits>
#include "MyTypeTraits.hpp"

namespace os {

namespace detail {

template <class T, class... TArgs>
decltype(void(T{std::declval<TArgs>()...}), std::true_type{}) test_is_braces_constructible(int);
template <class, class...>
std::false_type test_is_braces_constructible(...);
template <class T, class... TArgs>
using is_braces_constructible = decltype(test_is_braces_constructible<T, TArgs...>(0));

}  // namespace detail

template <class FuncObj, class T>
decltype(auto) bind_to_members(FuncObj&& Func, T&& object) noexcept {
    using type = std::decay_t<T>;
    using at = any_type;
    using detail::is_braces_constructible;
    if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17, p18);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16, p17);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15, p16);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14, p15);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at, at, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6, p7, p8, p9] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6, p7, p8] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6, p7, p8);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6, p7] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6, p7);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5, p6] = object;
        return Func(p0, p1, p2, p3, p4, p5, p6);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4, p5] = object;
        return Func(p0, p1, p2, p3, p4, p5);
    } else if constexpr (is_braces_constructible<type, at, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3, p4] = object;
        return Func(p0, p1, p2, p3, p4);
    } else if constexpr (is_braces_constructible<type, at, at, at, at>{}) {
        auto&& [p0, p1, p2, p3] = object;
        return Func(p0, p1, p2, p3);
    } else if constexpr (is_braces_constructible<type, at, at, at>{}) {
        auto&& [p0, p1, p2] = object;
        return Func(p0, p1, p2);
    } else if constexpr (is_braces_constructible<type, at, at>{}) {
        auto&& [p0, p1] = object;
        return Func(p0, p1);
    } else if constexpr (is_braces_constructible<type, at>{}) {
        auto&& [p0] = object;
        return Func(p0);
    } else {
        static_assert(is_braces_constructible<type, at>{}, "Failed to bind to members");
    }
}

}  // namespace os

#endif