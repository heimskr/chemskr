#include "chemskr/Chemskr.h"

namespace Chemskr {
	std::map<std::string, size_t> count(const std::string &formula) {
		return customCount<std::map>(formula);
	}
}
