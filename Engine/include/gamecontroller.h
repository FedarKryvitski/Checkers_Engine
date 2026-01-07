#pragma once
#include "Engine.h"

class GameController final {
public:
	GameController() noexcept;

	MoveResult makeMove(const MoveVector &move);

	void setState(const StateData &state) { state_ = state; };
	StateData state() const { return state_; };

private:
	void init();

private:
	Engine engine_;
	StateData state_;
};
