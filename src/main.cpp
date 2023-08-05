#include "chemskr/Chemskr.h"
#include "chemskr/Nuclear.h"

#include <cassert>
#include <cstring>
#include <iomanip>
#include <iostream>

namespace {
	void usage(const char *executable) {
		std::cerr << "Usage: " << executable << " -c <formula>\n"
		             "       " << executable << " -e <equation>\n";
	}
}

int testNuclear();

int main(int argc, char **argv) {
	if (argc == 2 && strcmp(argv[1], "-n") == 0)
		return testNuclear();

	if (argc != 3) {
		usage(argv[0]);
		return 1;
	}

	if (strcmp(argv[1], "-c") == 0) {

		const std::string formula = argv[2];

		Chemskr::parser.parse(formula)->debug();

		for (const auto &[element, count]: Chemskr::count(formula))
			std::cout << std::setw(2) << count << " x " << element << std::endl;

	} else if (strcmp(argv[1], "-e") == 0) {

		Chemskr::Equation equation{argv[2]};
		std::cout << "Balanced: " << std::boolalpha << equation.isBalanced() << std::endl;
		std::cout << "Reactants:\n";
		for (const auto &[reactant, count]: equation.getReactants()) {
			if (count == 1)
				std::cout << "- " << reactant << '\n';
			else
				std::cout << "- " << count << '*' << reactant << '\n';
		}
		std::cout << "Products:\n";
		for (const auto &[product, count]: equation.getProducts()) {
			if (count == 1)
				std::cout << "- " << product << '\n';
			else
				std::cout << "- " << count << '*' << product << '\n';
		}

	} else {
		usage(argv[0]);
		return 2;
	}
}

int testNuclear() {
	size_t i = 0;
	for (const auto &[atom, daltons]: std::map(Chemskr::nuclideMasses.begin(), Chemskr::nuclideMasses.end())) {
		std::cout << atom << " binding energy: " << atom.calculateBindingEnergy() << " MeV\n";
		if (++i == 15)
			break;
	}

	return 0;
}
