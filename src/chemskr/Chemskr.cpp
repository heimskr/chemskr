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

	Equation::Equation(const std::string &equation):
	root(Parser::parse(equation)) {
		validateRoot();
	}

	void Equation::validateRoot() const {
		if (!root)
			throw InvalidEquationError("Couldn't parse equation");

		if (root->size() != 1 || root->symbol == CHEMSKRTOK_ARROW)
			throw InvalidEquationError("Invalid equation");
	}

	bool Equation::isBalanced() {
		if (balanced)
			return *balanced;

		validateRoot();

		customCount(*root->front()->at(0), leftCounts,  1);
		customCount(*root->front()->at(1), rightCounts, 1);
		return *(balanced = leftCounts == rightCounts);
	}

	bool Equation::balanceAndCount(std::map<std::string, size_t> &counts_out) {
		if (balanced) {
			if (*balanced) {
				counts_out = leftCounts;
				return true;
			}

			counts_out = {};
			return false;
		}

		validateRoot();

		customCount(*root->front()->at(0), leftCounts,  1);
		customCount(*root->front()->at(1), rightCounts, 1);

		if (leftCounts == rightCounts) {
			balanced = true;
			counts_out = leftCounts;
			return true;
		}

		balanced = false;
		counts_out = {};
		return false;
	}

	const std::map<std::string, size_t> & Equation::countLeft() {
		if (leftCounts.empty())
			customCount(*root->front()->at(0), leftCounts, 1);
		return leftCounts;
	}

	const std::map<std::string, size_t> & Equation::countRight() {
		if (rightCounts.empty())
			customCount(*root->front()->at(0), rightCounts, 1);
		return rightCounts;
	}
}
