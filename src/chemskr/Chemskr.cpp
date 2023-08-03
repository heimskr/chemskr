#include "chemskr/Chemskr.h"

namespace Chemskr {
	namespace {
		template <typename K, typename V>
		bool operator==(const std::map<K, V> &left, const std::map<K, V> &right) {
			if (&left == &right)
				return true;
			if (left.size() != right.size())
				return false;
			for (const auto &[key, value]: left)
				if (auto iter = right.find(key); iter == right.end() || iter->second != value)
					return false;
			return true;
		}
	}

	std::map<std::string, size_t> count(const std::string &formula) {
		return customCount<std::map>(formula);
	}

	bool isBalanced(const std::string &equation) {
		std::unique_ptr<ASTNode> root = Parser::parse(equation);
		assert(root);
		assert(root->size() == 1);
		assert(root->front()->symbol == CHEMSKRTOK_ARROW);
		std::map<std::string, size_t> left;
		std::map<std::string, size_t> right;
		customCount(*root->front()->at(0), left,  1);
		customCount(*root->front()->at(1), right, 1);
		return left == right;
	}
}
