#pragma once
#include "moves.h"

#include <vector>

class Engine final {
public:
	Engine() noexcept = default;

	auto fill(const StateData &state) -> std::vector<StateData>;
	auto fill(const StateData &state, int depth) -> std::vector<AssessedData>;

private:
	int minmaxAB(const StateData &state, int depth, int alpha = kBlackWinAssess, int beta = kWhiteWinAssess);

private:
	static constexpr int kWhiteWinAssess{1000};
	static constexpr int kBlackWinAssess{-1000};
};
