#pragma once
// error.hpp

#include <expected>
#include <string>

namespace ms {

// the two types below are meant to be used when exceptions are undesirable
struct error {
	std::string w;
};

template <typename T = void>
using maybe = std::expected<T, error>;

} // namespace ms