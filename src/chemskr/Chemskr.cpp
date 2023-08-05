#include "chemskr/Chemskr.h"

#include <sstream>

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

	Equation::Equation(std::string equation):
	text(std::move(equation)),
	root(Parser::parse(text)) {
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

		if (!leftCounts) {
			leftCounts.emplace();
			customCount(*root->front()->at(0), *leftCounts,  1);
		}

		if (!rightCounts) {
			rightCounts.emplace();
			customCount(*root->front()->at(1), *rightCounts, 1);
		}

		return *(balanced = *leftCounts == *rightCounts);
	}

	std::string Equation::assemble(const ASTNode &node) {
		if (node.symbol == CHEMSKR_LIST) {
			std::stringstream ss;
			for (const ASTNode *subnode: node)
				ss << assemble(*subnode);
			return ss.str();
		}

		if (node.symbol == CHEMSKRTOK_INT) {
			std::stringstream ss;
			ss << *node.text << '(' << assemble(*node.front()) << ')';
			return ss.str();
		}

		if (node.symbol == CHEMSKRTOK_ELEMENT) {
			if (node.empty())
				return *node.text;
			return *node.text + std::to_string(node.front()->atoi());
		}

		if (node.symbol == CHEMSKRTOK_STAR) {
			std::stringstream ss;
			ss << *node.at(0)->text << '*' << assemble(*node.at(1));
			return ss.str();
		}

		return "?";
	}

	bool Equation::balanceAndCount(std::map<std::string, size_t> &counts_out) {
		if (balanced) {
			if (*balanced) {
				counts_out = *leftCounts;
				return true;
			}

			counts_out = {};
			return false;
		}

		if (!leftCounts) {
			leftCounts.emplace();
			customCount(*root->front()->at(0), *leftCounts,  1);
		}

		if (!rightCounts) {
			rightCounts.emplace();
			customCount(*root->front()->at(1), *rightCounts, 1);
		}

		if (*leftCounts == *rightCounts) {
			balanced = true;
			counts_out = *leftCounts;
			return true;
		}

		balanced = false;
		counts_out = {};
		return false;
	}

	const std::map<std::string, size_t> & Equation::countLeft() {
		if (!leftCounts) {
			leftCounts.emplace();
			customCount(*root->front()->at(0), *leftCounts, 1);
		}

		return *leftCounts;
	}

	const std::map<std::string, size_t> & Equation::countRight() {
		if (!rightCounts) {
			rightCounts.emplace();
			customCount(*root->front()->at(0), *rightCounts, 1);
		}

		return *rightCounts;
	}

	const ChemicalCounts & Equation::getSide(std::optional<ChemicalCounts> &optional, size_t index) const {
		if (!optional) {
			optional.emplace();
			const ASTNode *list = root->front()->at(index);
			optional->reserve(list->size());
			for (const ASTNode *node: *list) {
				if (node->symbol == CHEMSKRTOK_STAR)
					optional->emplace_back(assemble(*node->at(1)), node->at(0)->atoi());
				else
					optional->emplace_back(assemble(*node), 1);
			}
		}

		return *optional;
	}

	const ChemicalCounts & Equation::getReactants() {
		return getSide(reactants, 0);
	}

	const ChemicalCounts & Equation::getProducts() {
		return getSide(products, 1);
	}

	size_t Equation::getAtomCount() {
		if (!atomCount) {
			atomCount = 0;
			for (const auto &[atom, count]: countLeft())
				*atomCount += count;
		}

		return *atomCount;
	}
}
