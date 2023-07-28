#include "chemskr/ASTNode.h"
#include "chemskr/Parser.h"

#include <cassert>
#include <iostream>

int main() {
	Chemskr::parser.parse("3(CaCO3)H2O")->debug();
}
