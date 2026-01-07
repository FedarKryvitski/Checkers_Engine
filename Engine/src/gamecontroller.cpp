#include "gamecontroller.h"

GameController::GameController() noexcept { init(); }

void GameController::init() {
	BInit(state_.field);
	state_.type = MoveType::MOVE;
	state_.coord = {};
	state_.x = 0;
	state_.y = 0;
	state_.direction = MoveDirection::NONE;
	state_.turn = true;
}

MoveResult GameController::makeMove(const MoveVector &move) {
	/*
	StateData data(field);
	setData(data);
	auto [x1, y1, x2, y2] = move;
	data.coord.x1 = x1;
	data.coord.y1 = y1;
	data.coord.x2 = x2;
	data.coord.y2 = y2;

	StateData temp = data;
	MoveResult result = engine.makeMove(data);
	if (result != INVALID_COORD) {
		temp.field = data.field;
		getData(data);
		if (result == MoveResult::SUCCESS) {
			turn = !turn;
		}
	}
	return result;

	auto possibleMoves = engine.possibleMoves(moveData);

	uint8_t index = find(moveData.coord);
	if (index != -1) {
		AssessMoveData move = bestMoves[index];
		type = move.type;
		direction = move.direction;
		field = move.field;

		if (type == MoveType::BEAT) {
			direction = GetMode(x1, y1, x2, y2, direction);
			x = x2;
			y = y2;

			fill(moveData);

			if (bestMoves.size() > 0) {
				return MoveResult::ONE_MORE;
			} else {
				x = 0;
				y = 0;
				type = MoveType::MOVE;
				direction = MoveDirection::NONE;
			}
		} else {
			direction = MoveDirection::NONE;
			x = 0;
			y = 0;
		}
		turn = !turn;
		fill(moveData);
		if (bestMoves.size() == 0) {
			return MoveResult::WIN;
		}

		return MoveResult::SUCCESS;
	}
		*/
	return MoveResult::SUCCESS;
}
