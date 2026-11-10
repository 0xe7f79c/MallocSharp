#pragma once
// error.hpp

#include <expected>
#include <string>

namespace ms {

template <typename T = void>
using maybe = std::expected<T, std::string>;

} // namespace ms