#ifndef PACKSPLIT_HPP
#define PACKSPLIT_HPP

#include <tuple>

namespace detail {

template <class T, class Tuple, std::size_t... I>
constexpr T make_from_tuple_impl(Tuple&& t, std::index_sequence<I...>) {
    return T{std::get<I>(std::forward<Tuple>(t))...};
}

template <class T, class Tuple>
constexpr T make_from_tuple(Tuple&& t) {
    return detail::make_from_tuple_impl<T>(std::forward<Tuple>(t), std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
}

template <class Arg, class Tuple>
constexpr auto add_to_element(Arg&& arg, Tuple&& tuple) {
    return std::make_tuple(std::tuple_cat(std::forward_as_tuple(arg), std::get<0>(tuple)), std::get<1>(tuple), std::get<2>(tuple));
}

template <int size, class Arg, class Tuple>
constexpr auto add_to_pack(Arg&& arg, Tuple&& tuple) {
    if constexpr (std::tuple_size_v<std::decay_t<decltype(std::get<0>(tuple))>> < size - 1) {
        return std::make_tuple(std::tuple<>{}, std::tuple<>{},
                               std::tuple_cat(std::make_tuple(std::tuple_cat(std::forward_as_tuple(arg), std::get<0>(tuple))), std::get<1>(tuple)));
    } else {
        return std::make_tuple(std::tuple<>{}, std::tuple_cat(std::make_tuple(std::tuple_cat(std::forward_as_tuple(arg), std::get<0>(tuple))), std::get<1>(tuple)),
                               std::get<2>(tuple));
    }
}

template <int size, int count, class... Rest>
constexpr auto make_tuple_from_pack(Rest&&... rest) {
    return std::make_tuple(std::tuple<>{}, std::tuple<>{}, std::tuple<>{});
}

template <int size, int count, class First, class... Rest>
constexpr auto make_tuple_from_pack(First&& first, Rest&&... rest) {
    if constexpr (count == 0) {
        return add_to_pack<size>(std::forward<First>(first), make_tuple_from_pack<size, size - 1>(std::forward<Rest>(rest)...));
    } else {
        return add_to_element(std::forward<First>(first), make_tuple_from_pack<size, count - 1>(std::forward<Rest>(rest)...));
    }
}

}  // namespace detail

template <class Pack, class Remainder = std::tuple<>>
struct iterate_tuple {
    iterate_tuple(Pack p, Remainder r) : pack(p), m_remainder(r) {}
    iterate_tuple(Pack p) : pack(p) {}

    constexpr static std::size_t remainder_size = std::tuple_size_v<Remainder>;

    auto& remainder() { return m_remainder; }

    auto begin() { return array.begin(); }
    auto end() { return array.end(); }

   private:
    Pack pack;

    using array_type = std::array<std::decay_t<decltype(std::get<0>(std::declval<Pack>()))>, std::tuple_size_v<Pack>>;
    array_type array = detail::make_from_tuple<array_type>(pack);
    Remainder m_remainder;
};

template <int size, class... Args>
constexpr auto pack_split(Args&&... args) {
    if constexpr (size == 0) {
        return iterate_tuple(std::tie(args...));
    } else {
        auto v = detail::make_tuple_from_pack<size, 0>(std::forward<Args>(args)...);
        return iterate_tuple(std::get<1>(v), std::get<0>(std::get<2>(v)));
    }
}

#endif  // PACKSPLIT_HPP