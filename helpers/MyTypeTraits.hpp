namespace os {

namespace detail {
template <template <class...> class CheckTemplate, class = void, class... Object>
struct detect_impl : std::false_type {};

template <template <class...> class CheckTemplate, class... Object>
struct detect_impl<CheckTemplate, std::void_t<CheckTemplate<Object...>>, Object...> : std::true_type {};
}  // namespace detail

template <template <class...> class CheckTemplate, class... Object>
constexpr bool detect_v = detail::detect_impl<CheckTemplate, void, Object...>{};

struct any_type {
    template <class T>
    constexpr operator T();
};

}  // namespace os