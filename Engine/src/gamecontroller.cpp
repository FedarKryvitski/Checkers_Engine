#include "gamecontroller.h"

void GameController::getData(AssessMoveData &source) {
	field = source.field;
	type = source.type;
	direction = source.direction;
	x = source.x;
	y = source.y;
}

void GameController::setData(AssessMoveData &dest) {
	MoveData data(field);
	data.x = x;
	data.y = y;
	dest.field = field;
	dest.turn = turn;
	dest.type = type;
	dest.direction = direction;
}

GameController::GameController() {
	turn = true;
	curr = 0;
	head = 0;
	assess = 0;
	BInit(field);

	AssessMoveData moveData(field);
	setData(moveData);

	// gameMoves.push_back(temp);
}

MoveResult GameController::playerMove(Vector4u moveCoordinates) {
	if (curr == head && !locked) {
		AssessMoveData data(field);
		setData(data);
		auto [x1, y1, x2, y2] = moveCoordinates;
		data.coord.x1 = x1;
		data.coord.y1 = y1;
		data.coord.x2 = x2;
		data.coord.y2 = y2;

		AssessMoveData temp = data;
		MoveResult result = engine.PlayerMove(data);
		if (result != INVALID_COORD) {
			temp.field = data.field;
			gameMoves.push_back(temp);
			getData(data);
			if (result == MoveResult::SUCCESS) {
				turn = !turn;
			} else if (result == MoveResult::WIN) {
				locked = true;
			}
			head++;
			curr++;
		}
		return result;
	}
	return MoveResult::INVALID_COORD;
}

MoveResult GameController::engineMove(int depth) {
	if (!locked) {
		getCurr();

		AssessMoveData data(field);
		setData(data);

		AssessMoveData temp = data;
		MoveResult result = engine.EngineMove(data, depth);
		if (result == ONE_MORE || result == SUCCESS || result == WIN) {
			temp.field = data.field;
			temp.coord = data.coord;
			gameMoves.push_back(temp);
			getData(data);
			if (result == SUCCESS) {
				turn = !turn;
			} else if (result == WIN) {
				locked = true;
			}
			curr++;
			head++;
		}
		return result;
	}
	return INVALID_COORD;
}

void GameController::getPrev() {
	if (curr > 0) {
		curr--;
		AssessMoveData temp = gameMoves[curr];
		field = temp.field;
		assess = temp.assess;
	}
}

void GameController::getNext() {
	if (curr < head) {
		curr++;
		AssessMoveData temp = gameMoves[curr];
		field = temp.field;
		assess = temp.assess;
	}
}

void GameController::getCurr() {
	if (curr != head) {
		curr = head;
		AssessMoveData temp = gameMoves[curr];
		field = temp.field;
		assess = temp.assess;
	}
}

void AnalysicsController::getData(AssessMoveData &source) {
	field = source.field;
	assess = source.assess;
	type = source.type;
	direction = source.direction;
	x = source.x;
	y = source.y;
	x1 = source.coord.x1;
	y1 = source.coord.y1;
	x2 = source.coord.x2;
	y2 = source.coord.y2;
	// comment = source.comment;
}

AnalysicsController::AnalysicsController() {
	x1 = x2 = y1 = y2 = 0;
	comment = MoveStatus::FORCED;
	type = MoveType::MOVE;
	x = y = 0;
	direction = MoveDirection::NONE;
	turn = true;
	curr = 0;
	head = 0;
	assess = 0;
	BInit(field);
}

void AnalysicsController::evaluate(int index, int depth) {
	// engine.evaluate(gameMoves[index], depth);
}

void AnalysicsController::setMoves(std::vector<AssessMoveData> &_gameMoves) { gameMoves = _gameMoves; }

void AnalysicsController::getPrev() {
	if (curr > 0) {
		curr--;
		getData(gameMoves[curr]);
	}
}

void AnalysicsController::getNext() {
	if (curr < gameMoves.size() - 1) {
		curr++;
		getData(gameMoves[curr]);
	}
}

void AnalysicsController::getCurr() {
	if (curr != head) {
		curr = head;
		getData(gameMoves[curr]);
	}
}
