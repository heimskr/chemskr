#include "chemskr/Nuclear.h"

namespace Chemskr {
	std::array<std::string, 119> atomicSymbols {
		"n", "H", "He", "Li", "Be", "B", "C", "N", "O", "F", "Ne", "Na", "Mg", "Al", "Si", "P", "S", "Cl", "Ar", "K",
		"Ca", "Sc", "Ti", "V", "Cr", "Mn", "Fe", "Co", "Ni", "Cu", "Zn", "Ga", "Ge", "As", "Se", "Br", "Kr", "Rb", "Sr",
		"Y", "Zr", "Nb", "Mo", "Tc", "Ru", "Rh", "Pd", "Ag", "Cd", "In", "Sn", "Sb", "Te", "I", "Xe", "Cs", "Ba", "La",
		"Ce", "Pr", "Nd", "Pm", "Sm", "Eu", "Gd", "Tb", "Dy", "Ho", "Er", "Tm", "Yb", "Lu", "Hf", "Ta", "W", "Re", "Os",
		"Ir", "Pt", "Au", "Hg", "Tl", "Pb", "Bi", "Po", "At", "Rn", "Fr", "Ra", "Ac", "Th", "Pa", "U", "Np", "Pu", "Am",
		"Cm", "Bk", "Cf", "Es", "Fm", "Md", "No", "Lr", "Rf", "Db", "Sg", "Bh", "Hs", "Mt", "Ds", "Rg", "Uub", "Uut",
		"Uuq", "Uup", "Uuh", "Uus", "Uuo"
	};

	std::unordered_set<std::string> atomicSymbolSet{atomicSymbols.begin() + 1, atomicSymbols.end()};

	Atom::Atom(uint16_t protons_, uint16_t neutrons_):
		protons(protons_), neutrons(neutrons_) {}

	double Atom::calculateBindingEnergy() const {
		return protons * (Mp + Me) + neutrons * Mn - getMass() * DALTON_TO_MEV;
	}

	double Atom::getMass() const {
		return nuclideMasses.at(*this);
	}

	std::string Atom::getName() const {
		if (atomicSymbols.size() <= protons)
			return std::to_string(protons) + '-' + std::to_string(protons + neutrons);
		return atomicSymbols[protons] + '-' + std::to_string(protons + neutrons);
	}
}

std::ostream & operator<<(std::ostream &os, Chemskr::Atom atom) {
	return os << atom.getName();
}
