#include "Engine.h"

#include <ThreadPoolManager.h>
#include <algorithm>
#include <future>
#include <vector>

std::vector<StateData> Engine::fill(const StateData &state) {
	auto &[field, type, coord, x, y, direction, turn] = state;

	TAllMoves AllMoves;
	uint8_t len = 0;
	bool needToBeat = PMFill(field, type, AllMoves, &len, turn, x, y, direction);

	std::vector<StateData> result;

	for (uint8_t i = 0; i < len; i++) {
		StateData temp;
		temp.field = field;
		temp.coord = AllMoves[i];

		const auto &[x1, y1, x2, y2] = AllMoves[i];

		if (needToBeat) {
			if (temp.field[x1][y1] >= 3) {
				DamkaBeat(temp.field, x1, y1, x2, y2, direction);
			} else {
				Beat(temp.field, x1, y1, x2, y2);
			}
			temp.x = x2;
			temp.y = y2;
			temp.type = MoveType::BEAT;
			temp.direction = GetMode(x1, y1, x2, y2, direction);
		} else {
			Move(temp.field, x1, y1, x2, y2);
			temp.x = 0;
			temp.y = 0;
			temp.type = MoveType::MOVE;
			temp.direction = MoveDirection::NONE;
			temp.turn = !temp.turn;
		}
		result.push_back(std::move(temp));
	}

	return result;
}

std::vector<AssessedData> Engine::fill(const StateData &moveData, int depth) {
	auto &[field, type, coord, x, y, direction, turn] = moveData;

	TAllMoves AllMoves;
	uint8_t len = 0;
	bool needToBeat = PMFill(field, type, AllMoves, &len, turn, x, y, direction);

	std::vector<std::future<AssessedData>> futures;
	auto threadPool = ThreadPoolManager::instance();

	for (uint8_t i = 0; i < len; i++) {
		futures.emplace_back(threadPool->enqueue([=, &field]() -> AssessedData {
			AssessedData temp;
			temp.field = field;
			temp.coord = AllMoves[i];

			const auto &[x1, y1, x2, y2] = AllMoves[i];

			if (needToBeat) {
				if (temp.field[x1][y1] >= 3) {
					DamkaBeat(temp.field, x1, y1, x2, y2, direction);
				} else {
					Beat(temp.field, x1, y1, x2, y2);
				}
				temp.x = x2;
				temp.y = y2;
				temp.type = MoveType::BEAT;
				temp.direction = GetMode(x1, y1, x2, y2, direction);
				temp.assessment = minmaxAB(temp, depth);
			} else {
				Move(temp.field, x1, y1, x2, y2);
				temp.x = 0;
				temp.y = 0;
				temp.type = MoveType::MOVE;
				temp.direction = MoveDirection::NONE;
				temp.turn = !temp.turn;
				temp.assessment = minmaxAB(temp, depth - 1);
			}
			return temp;
		}));
	}

	return {};

	// moves_.clear();
	// std::transform(futures.begin(), futures.end(), moves_.begin(),
	// 			   [](auto &future) -> StateData { return future.get(); });

	// std::sort(moves_.begin(), moves_.end(), [](const auto &f, const auto &s) { return f.assessment < s.assessment;
	// });
}

int Engine::minmaxAB(const StateData &moveData, int depth, int alpha, int beta) {
	TAllMoves AllMoves;
	uint8_t len = 0;

	auto &[field, type, coord, x, y, direction, turn] = moveData;

	bool needToBeat = PMFill(field, type, AllMoves, &len, turn, x, y, direction);

	if (len == 0) {
		if (type == BEAT) {
			StateData nextTurn;
			nextTurn.field = field;
			nextTurn.turn = !turn;
			return minmaxAB(nextTurn, depth - 1, alpha, beta);
		}
		return turn ? kBlackWinAssess : kWhiteWinAssess;
	}

	if (depth <= 0 && !needToBeat) {
		return getAssess(field);
	}

	int minmaxEval = turn ? kBlackWinAssess : kWhiteWinAssess;
	int eval = 0;

	for (int i = 0; i < len; i++) {
		MoveDirection tempDirection{MoveDirection::NONE};
		Field tempBoard;
		tempBoard = field;

		uint8_t x1, y1, x2, y2;
		auto &coord = AllMoves[i];
		x1 = coord.x1;
		y1 = coord.y1;
		x2 = coord.x2;
		y2 = coord.y2;

		StateData newMoveData(tempBoard);
		newMoveData.x = x2;
		newMoveData.y = y2;

		if (needToBeat) {
			if (tempBoard[x1][y1] >= 3) {
				DamkaBeat(tempBoard, x1, y1, x2, y2, tempDirection);
			} else {
				Beat(tempBoard, x1, y1, x2, y2);
			}
			newMoveData.direction = GetMode(x1, y1, x2, y2, tempDirection);
			eval = minmaxAB(newMoveData, depth, alpha, beta);
		} else {
			Move(tempBoard, x1, y1, x2, y2);
			newMoveData.turn = !newMoveData.turn;
			eval = minmaxAB(newMoveData, depth - 1, alpha, beta);
		}

		if (turn) {
			minmaxEval = std::max(minmaxEval, eval);
			alpha = std::max(alpha, eval);
		} else {
			minmaxEval = std::min(minmaxEval, eval);
			beta = std::min(beta, eval);
		}

		if (beta <= alpha) {
			break;
		}
	}
	return minmaxEval;
}