#pragma once

#include "chemskr/ASTNode.h"
#include "chemskr/Lexer.h"
#include "chemskr/Parser.h"

#include <cassert>
#include <map>

namespace Chemskr {
	struct InvalidEquationError: std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	struct UnbalancedEquationError: std::runtime_error {
		using std::runtime_error::runtime_error;
	};

	template <template <typename...> typename M = std::map>
	void customCount(const ASTNode &node, M<std::string, size_t> &map, size_t multiplier) {
		if (node.symbol == CHEMSKRTOK_ARROW) {
			assert(node.size() == 2);
			customCount(*node.front(), map, 1);
			customCount(*node.back(), map, 1);
		} else if (node.symbol == CHEMSKRTOK_ELEMENT) {
			if (node.empty())
				map[*node.text] += multiplier;
			else
				map[*node.text] += multiplier * node.front()->atoi();
		} else if (node.symbol == CHEMSKR_LIST || node.symbol == CHEMSKR_SUM) {
			for (const ASTNode *subnode: node)
				customCount(*subnode, map, multiplier);
		} else if (node.symbol == CHEMSKRTOK_INT) {
			assert(node.size() == 1);
			customCount(*node.front(), map, node.atoi() * multiplier);
		} else {
			throw std::invalid_argument("Couldn't handle node of type " + std::string(node.getName()));
		}
	}

	template <template <typename...> typename M = std::map>
	M<std::string, size_t> customCount(const std::string &formula) {
		std::unique_ptr<ASTNode> root = Parser::parse(formula);
		M<std::string, size_t> out;
		customCount(*root->front(), out, 1);
		return out;
	}

	std::map<std::string, size_t> count(const std::string &formula);

	std::map<std::string, size_t> countLeft(const std::string &equation);
	std::map<std::string, size_t> countRight(const std::string &equation);
	bool isBalanced(const std::string &equation);
	bool balanceAndCount(const std::string &equation, std::map<std::string, size_t> &counts_out);
}
