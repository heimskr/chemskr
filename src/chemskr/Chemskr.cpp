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

		std::map<std::string, size_t> countIndex(const std::string &equation, size_t index) {
			std::unique_ptr<ASTNode> root = Parser::parse(equation);
			if (!root)
				throw InvalidEquationError("Couldn't parse equation");

			if (root->size() != 1 || root->symbol == CHEMSKRTOK_ARROW)
				throw InvalidEquationError("Invalid equation");

			std::map<std::string, size_t> counts;
			customCount(*root->at(index), counts, 1);
			return counts;
		}
	}

	std::map<std::string, size_t> count(const std::string &formula) {
		return customCount<std::map>(formula);
	}

	std::map<std::string, size_t> countLeft(const std::string &equation) {
		return countIndex(equation, 0);
	}

	std::map<std::string, size_t> countRight(const std::string &equation) {
		return countIndex(equation, 1);
	}

	bool isBalanced(const std::string &equation) {
		std::unique_ptr<ASTNode> root = Parser::parse(equation);

		if (!root)
			throw InvalidEquationError("Couldn't parse equation");

		if (root->size() != 1 || root->symbol == CHEMSKRTOK_ARROW)
			throw InvalidEquationError("Invalid equation");

		std::map<std::string, size_t> left;
		std::map<std::string, size_t> right;
		customCount(*root->front()->at(0), left,  1);
		customCount(*root->front()->at(1), right, 1);
		return left == right;
	}

	bool balanceAndCount(const std::string &equation, std::map<std::string, size_t> &counts_out) {
		std::unique_ptr<ASTNode> root = Parser::parse(equation);

		if (!root)
			throw InvalidEquationError("Couldn't parse equation");

		if (root->size() != 1 || root->symbol == CHEMSKRTOK_ARROW)
			throw InvalidEquationError("Invalid equation");

		std::map<std::string, size_t> left;
		std::map<std::string, size_t> right;

		customCount(*root->front()->at(0), left,  1);
		customCount(*root->front()->at(1), right, 1);
		if (left == right) {
			counts_out = std::move(left);
			return true;
		}

		return false;
	}
}
