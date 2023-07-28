#pragma once

#include <charconv>
#include <concepts>
#include <stdexcept>
#include <string>
#include <vector>

namespace Chemskr {
	std::vector<std::string> split(const std::string &str, const std::string &delimiter, bool condense = false);
	std::string unescapeString(std::string_view);

	template <std::integral I>
	I parseNumber(std::string_view view, int base = 10) {
		I out{};
		auto result = std::from_chars(view.begin(), view.end(), out, base);
		if (result.ec == std::errc::invalid_argument)
			throw std::invalid_argument("Not an integer: \"" + std::string(view) + "\"");
		return out;
	}
}
