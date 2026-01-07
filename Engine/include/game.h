#pragma once
#include "engine.h"

#include <memory>
#include <vector>

namespace Checkers {

class Game {
public:
	Game() noexcept = default;

	bool makeMove(const MoveVector &move);
	void reset();

	Player getCurrentPlayer() const { return currentPlayer_; }
	GameState getGameState() const { return state_; }
	const Board &getBoard() const;

	bool isValidMove(const MoveVector &move) const;

	std::vector<MoveVector> getValidMoves(bool player);
	std::vector<MoveVector> getValidMoves() const { return getValidMoves(currentPlayer_); }

private:
	void initializeBoard();
	bool applyMove(const MoveVector &move);
	void updateGameState();

	Engine engine_;
	Player currentPlayer_{Player::White};
	GameState state_{GameState::InProgress};
};

} // namespace Checkers