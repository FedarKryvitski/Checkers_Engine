#include "game.h"

namespace Checkers {

bool Game::makeMove(const MoveVector &move) {}
void Game::reset() {}

const Board &Game::getBoard() const { return board_; }

bool Game::isValidMove(const MoveVector &move) const;

std::vector<MoveVector> Game::getValidMoves(bool player) {}

std::vector<MoveVector> Game::getValidMoves() const { return getValidMoves(currentPlayer_); }

void Game::initializeBoard() {}

} // namespace Checkers