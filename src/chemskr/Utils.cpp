#include "chemskr/Utils.h"

#include <sstream>

namespace Chemskr {
	std::vector<std::string> split(const std::string &str, const std::string &delimiter, bool condense) {
		if (str.empty())
			return {};

		size_t next = str.find(delimiter);
		if (next == std::string::npos)
			return {str};

		std::vector<std::string> out {};
		const size_t delimiter_length = delimiter.size();
		size_t last = 0;

		out.push_back(str.substr(0, next));

		while (next != std::string::npos) {
			last = next;
			next = str.find(delimiter, last + delimiter_length);
			std::string sub = str.substr(last + delimiter_length, next - last - delimiter_length);
			if (!sub.empty() || !condense)
				out.push_back(std::move(sub));
		}

		return out;
	}

	std::string unescapeString(std::string_view str) {
		const size_t size = str.size();
		if (size == 0)
			return "";
		std::stringstream out;
		for (size_t i = 0; i < size; ++i) {
			char ch = str[i];
			if (ch == '\\') {
				if (i == size - 1)
					throw std::runtime_error("Backslash at end of string");
				switch (str[++i]) {
					case 'n':  out << '\n'; break;
					case 'r':  out << '\r'; break;
					case 'a':  out << '\a'; break;
					case 't':  out << '\t'; break;
					case 'b':  out << '\b'; break;
					case 'e':  out << '\e'; break;
					case '0':  out << '\0'; break;
					case '\\': out << '\\'; break;
					case '"':  out << '"';  break;
					case 'x': {
						if (size <= i + 2)
							throw std::runtime_error("Hexadecimal escape is too close to end of string");
						const char first  = str[++i];
						const char second = str[++i];
						if (isxdigit(first) == 0 || isxdigit(second) == 0)
							throw std::runtime_error(std::string("Invalid hexadecimal escape: \\x") + first + second);
						out << char(strtol((std::string(1, first) + second).c_str(), nullptr, 16));
						break;
					}
					default: throw std::runtime_error("Unrecognized character: \\" + std::string(1, str[i]));
				}
			} else
				out << ch;
		}
		return out.str();
	}
}
