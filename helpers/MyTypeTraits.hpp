#ifndef MYTYPETRAITS_HPP
#define MYTYPETRAITS_HPP

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

template <class... T>
struct TypePack {};

template <typename T>
struct FunctionSignature {};

template <typename P_ReturnType, typename P_Class, typename P_Arg>
struct FunctionSignature<P_ReturnType (P_Class::*)(P_Arg)> {
    using ReturnType = P_ReturnType;
    using Class = P_Class;
    using Arg = P_Arg;
};

template <typename P_ReturnType, typename P_Class, typename P_Arg>
struct FunctionSignature<P_ReturnType (P_Class::*)(P_Arg) const> {
    using ReturnType = P_ReturnType;
    using Class = P_Class;
    using Arg = P_Arg;
};

template <typename P_ReturnType, typename P_Class, typename... P_Args>
struct FunctionSignature<P_ReturnType (P_Class::*)(P_Args...)> {
    using ReturnType = P_ReturnType;
    using Class = P_Class;
    using Args = TypePack<P_Args...>;
};

template <typename P_ReturnType, typename P_Class, typename... P_Args>
struct FunctionSignature<P_ReturnType (P_Class::*)(P_Args...) const> {
    using ReturnType = P_ReturnType;
    using Class = P_Class;
    using Args = TypePack<P_Args...>;
};

template <class T>
using LambdaSignature = FunctionSignature<decltype(T::operator())>;

}  // namespace os

#endif  // MYTYPETRAITS_HPP