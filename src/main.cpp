#include "chemskr/Chemskr.h"

#include <cassert>
#include <iostream>

int main() {
	const std::string formula = "3(CaCO3)H2O";

	Chemskr::parser.parse(formula)->debug();

	for (const auto &[element, count]: Chemskr::count(formula))
		std::cout << count << " x " << element << std::endl;
}
