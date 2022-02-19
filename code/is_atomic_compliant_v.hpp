#pragma once
#include <type_traits>

template <class T>
constexpr bool is_atomic_compliant_v = std::conjunction_v<
    std::is_trivially_copy_constructible<T>,
    std::negation<std::is_polymorphic<T>>,
    std::is_trivial<T>
>;

