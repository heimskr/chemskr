#include "chemskr/Chemskr.h"

#include <cassert>
#include <iomanip>
#include <iostream>

int main(int argc, char **argv) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <formula>\n";
		return 1;
	}

	const std::string formula = argv[1];

	Chemskr::parser.parse(formula)->debug();

	for (const auto &[element, count]: Chemskr::count(formula))
		std::cout << std::setw(2) << count << " x " << element << std::endl;
}
