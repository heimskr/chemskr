#pragma once

#include <cstdint>
#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace Chemskr {
	constexpr double Mp = 938.272081358; // MeV/c^2
	constexpr double Me = 0.51099894613; // MeV/c^2
	constexpr double Mn = 939.565413358; // MeV/c^2
	constexpr double DALTON_TO_MEV = 931.49402823; // (MeV/c^2) / Da

	class Atom {
		public:
			uint16_t protons{};
			uint16_t neutrons{};

			Atom(uint16_t protons_, uint16_t neutrons_);

			/** In MeV */
			double calculateBindingEnergy() const;
			/** In Daltons */
			double getMass() const;

			std::string getName() const;

			inline auto operator<=>(const Atom &other) const = default;
	};
}

namespace std {
	template<>
	struct hash<Chemskr::Atom> {
		size_t operator()(const Chemskr::Atom &atom) const {
			return (size_t(atom.protons) << 16) | atom.neutrons;
		}
	};
}

namespace Chemskr {
	/** In Daltons */
	extern std::unordered_map<Atom, double> nuclideMasses;
	extern std::array<std::string, 119> atomicSymbols;
	extern std::unordered_set<std::string> atomicSymbolSet;
}

template <>
struct std::formatter<Chemskr::Atom> {
	constexpr auto parse(auto &ctx) {
		return ctx.begin();
	}

	auto format(const auto &atom, auto &ctx) const {
		return std::format_to(ctx.out(), "{}", atom.getName());
	}
};
