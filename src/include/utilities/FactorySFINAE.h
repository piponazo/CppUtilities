#pragma once

#include <memory>
#include <type_traits>
#include <utility>

namespace ut
{
    template <typename U>
    std::true_type test(U);

    template <typename T, typename... Ts>
    std::false_type test_has_ctor(...);

    template <typename T, typename... Ts>
    auto test_has_ctor(T*) -> decltype(test(std::declval<decltype(T(std::declval<Ts>()...))>()));

    template <typename Concrete, typename... Ts>
    std::enable_if_t<decltype(test_has_ctor<Concrete, Ts...>(nullptr))::value, std::unique_ptr<Concrete> >
    constructArgs(Ts&&... params)
    {
        return std::make_unique<Concrete>(std::forward<Ts>(params)...);
    }

    template <typename Concrete, typename... Ts>
    std::unique_ptr<Concrete> constructArgs(...)
    {
        return nullptr;
    }

    template <typename Concrete, typename... Ts>
    std::unique_ptr<Concrete> constructNoArgs(void)
    {
        return std::unique_ptr<Concrete>(new Concrete);
    }

    template <typename Concrete, typename... Ts>
    std::unique_ptr<Concrete> constructNoArgs(Ts&&...)
    {
        return nullptr;
    }
}  // namespace ut
