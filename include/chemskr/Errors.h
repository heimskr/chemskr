#pragma once

#include "chemskr/ASTNode.h"

#include <stdexcept>

namespace Chemskr {
	struct SyntaxError: std::runtime_error {
		ASTLocation location;
		using std::runtime_error::runtime_error;
		template <typename... Args>
		explicit SyntaxError(const ASTLocation &location_, Args &&...args):
			std::runtime_error(std::forward<Args>(args)...), location(location_) {}
	};
}
