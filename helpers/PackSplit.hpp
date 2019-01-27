#ifndef PACKSPLIT_HPP
#define PACKSPLIT_HPP

#include <tuple>

namespace detail {

template <std::size_t INCREMENT, class F, class ArgsTuple, std::size_t... Is>
void pack_for_each_impl(F&& f, ArgsTuple&& args, std::index_sequence<Is...>&& is) {
    if constexpr (sizeof...(Is) + INCREMENT <= std::tuple_size_v<ArgsTuple>) {
        f(std::forward<std::tuple_element_t<Is + INCREMENT, ArgsTuple>>(std::get<Is + INCREMENT>(args))...);
        pack_for_each_impl<INCREMENT + sizeof...(Is)>(std::forward<F>(f), std::forward<ArgsTuple>(args), std::make_index_sequence<sizeof...(Is)>{});
    }
}

}  // namespace detail

template <std::size_t SIZE, class F, class... Args>
void pack_for_each(F&& f, Args&&... args) {
    detail::pack_for_each_impl<0>(std::forward<F>(f), std::forward_as_tuple(std::forward<Args>(args)...), std::make_index_sequence<SIZE>{});
}

#endif  // PACKSPLIT_HPP