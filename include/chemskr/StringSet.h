#pragma once

#include <string>
#include <unordered_set>

namespace Chemskr {
	struct StringSet {
		StringSet();

		static std::unordered_set<std::string> set;
		static const std::string * intern(const char *);
		static const std::string * intern(const std::string &);
		static const std::string * unquote(const std::string *);
	};
}
