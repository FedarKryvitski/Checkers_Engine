#include "Engine.h"

#include <algorithm>
#include <vector>

namespace {
constexpr int kWhiteWinAssess{1000};
constexpr int kBlackWinAssess{-1000};
constexpr int kThreadsCount{12};
} // namespace

Engine::Engine() noexcept : threadPool(kThreadsCount) {}

void Engine::fill(AssessMoveData &moveData, int depth) {
	auto &[field, type, coord, x, y, direction, turn, assess] = moveData;

	TAllMoves AllMoves;
	uint8_t len = 0;
	bool ntb = PMFill(field, type, AllMoves, &len, turn, x, y, direction);

	std::vector<std::future<AssessMoveData>> futures;

	for (uint8_t i = 0; i < len; i++) {
		futures.emplace_back(threadPool.enqueue([=, &field]() -> AssessMoveData {
			AssessMoveData temp(field);
			temp.field = field;

			const auto &move = AllMoves[i];
			uint8_t x1 = move.x1;
			uint8_t y1 = move.y1;
			uint8_t x2 = move.x2;
			uint8_t y2 = move.y2;

			temp.coord = move;

			if (ntb) {
				if (temp.field[x1][y1] >= 3) {
					DamkaBeat(temp.field, x1, y1, x2, y2, direction);
				} else {
					Beat(temp.field, x1, y1, x2, y2);
				}
				temp.x = x2;
				temp.y = y2;
				temp.type = MoveType::BEAT;
				temp.direction = GetMode(x1, y1, x2, y2, direction);
				temp.assess = mmAB(temp, kBlackWinAssess, kWhiteWinAssess, depth);
			} else {
				Move(temp.field, x1, y1, x2, y2);
				temp.x = 0;
				temp.y = 0;
				temp.type = MoveType::MOVE;
				temp.direction = MoveDirection::NONE;
				temp.turn = !temp.turn;
				temp.assess = mmAB(temp, kBlackWinAssess, kWhiteWinAssess, depth - 1);
			}
			return temp;
		}));
	}

	bestMoves.clear();
	std::transform(futures.begin(), futures.end(), bestMoves.begin(),
				   [](auto &future) -> AssessMoveData { return future.get(); });

	std::sort(bestMoves.begin(), bestMoves.end(), [](const auto &f, const auto &s) { return f.assess < s.assess; });

	if (turn) {
		std::reverse(bestMoves.begin(), bestMoves.end());
	}
}

uint8_t Engine::find(Vector4u coordinates) {
	auto &moves = bestMoves;
	auto it =
		std::find_if(moves.begin(), moves.end(), [&](const auto &move) -> bool { return move.coord == coordinates; });

	int index = it == moves.end() ? -1 : static_cast<int>(std::distance(it, moves.begin()));

	return index;
}

int16_t Engine::mmAB(AssessMoveData &moveData, int16_t alpha, int16_t beta, int depth) {
	TAllMoves AllMoves;
	uint8_t len = 0;

	auto &[field, type, coord, x, y, direction, turn, assess] = moveData;

	bool ntb = PMFill(field, type, AllMoves, &len, turn, x, y, direction);

	if (len == 0) {
		if (type == BEAT) {
			AssessMoveData nextTurn(field);
			nextTurn.turn = !turn;
			return mmAB(nextTurn, alpha, beta, depth - 1);
		}
		return turn ? kBlackWinAssess : kWhiteWinAssess;
	}

	if (depth <= 0 && !ntb) {
		return getAssess(field);
	}

	int16_t minmaxEval = turn ? kBlackWinAssess : kWhiteWinAssess;
	int16_t eval = 0;

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

		AssessMoveData newMoveData(tempBoard);
		newMoveData.x = x2;
		newMoveData.y = y2;

		if (ntb) {
			if (tempBoard[x1][y1] >= 3) {
				DamkaBeat(tempBoard, x1, y1, x2, y2, tempDirection);
			} else {
				Beat(tempBoard, x1, y1, x2, y2);
			}
			newMoveData.direction = GetMode(x1, y1, x2, y2, tempDirection);
			eval = mmAB(newMoveData, alpha, beta, depth);
		} else {
			Move(tempBoard, x1, y1, x2, y2);
			newMoveData.turn = !newMoveData.turn;
			eval = mmAB(newMoveData, alpha, beta, depth - 1);
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

void Engine::fill(AssessMoveData &moveData) {
	TAllMoves AllMoves;
	uint8_t len = 0;

	auto &[field, type, coord, x, y, direction, turn, assess] = moveData;

	bool ntb = PMFill(field, type, AllMoves, &len, turn, x, y, direction);
	bestMoves.clear();
	bestMoves.reserve(len);

	AssessMoveData temp(field);

	for (uint8_t i = 0; i < len; i++) {
		temp.field = field;

		uint8_t x1, y1, x2, y2;
		x1 = AllMoves[i].x1;
		y1 = AllMoves[i].y1;
		x2 = AllMoves[i].x2;
		y2 = AllMoves[i].y2;

		temp.coord = AllMoves[i];
		temp.type = type;

		if (ntb) {
			if (temp.field[x1][y1] >= 3) {
				DamkaBeat(temp.field, x1, y1, x2, y2, static_cast<uint8_t>(direction));
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
		}
		temp.assess = 0;
		bestMoves.push_back(temp);
	}
}

MoveResult Engine::PlayerMove(AssessMoveData &moveData) {
	auto &[field, type, coord, x, y, direction, turn, assess] = moveData;
	fill(moveData);

	auto &[x1, y1, x2, y2] = moveData.coord;

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
	return MoveResult::INVALID_COORD;
}

MoveResult Engine::EngineMove(AssessMoveData &moveData, int depth) {
	auto &[field, type, coord, x, y, direction, turn, assess] = moveData;
	fill(moveData);

	if (bestMoves.size() > 1) {
		fill(moveData, depth);
	}
	if (bestMoves.size() > 0) {
		AssessMoveData move = bestMoves[0];

		moveData.coord = move.coord;
		auto &[x1, y1, x2, y2] = moveData.coord;

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
	return MoveResult::INVALID_COORD;
}

// void Engine::evaluate(AssessMoveData& data, uint8_t depth) {
//
//	float assess = 0;
//	int index = -1;
//	bool turn = data.turn;
//
//	fill(data.oldfield, data.type, data.x, data.y, data.direction, turn,
// depth); 	index = find(data.coord); 	assess = bestMoves[index].assess; float
// temp_assess = bestMoves[0].assess;
//
//	if (bestMoves.size() == 1) {
//		data.comment = FORCED;
//	}
//	else if (index == 0) {
//		data.comment = BEST;
//	}
//	else if ((temp_assess < assess + 0.3 || (temp_assess < assess + 1.0 &&
// assess > 5)) && turn || (temp_assess > assess - 0.3 || (temp_assess > assess
//- 1.0 && assess > 5)) && !turn) { 		data.comment = GOOD;
//	}
//	else if ((temp_assess < assess + 0.9 || (temp_assess < assess + 3.0 &&
// assess > 5)) && turn || (temp_assess > assess - 0.9 || (temp_assess > assess
//- 3.0 && assess > 5)) && !turn) { 		data.comment = INACCURACY;
//	}
//	else {
//		data.comment = BLUNDER;
//	}
//	data.assess = assess;
// }
