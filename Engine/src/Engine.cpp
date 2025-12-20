#include "Engine.h"
#include <algorithm>
#include <future>
#include <vector>

using std::max, std::min, std::vector, std::future;

constexpr int16_t WHITE_WIN_ASSESS = 1000;
constexpr int16_t BLACK_WIN_ASSESS = -1000;

#ifdef THREADS
Engine::Engine() : threadPool(12) { }
void Engine::fill(AssessMoveData& moveData, uint8_t depth) {
	auto& [field, type, coord, x, y, direction, turn, assess] = moveData;

	TAM AllMoves;
	uint8_t len = 0;
	bool ntb = PMFill(field, type, AllMoves, &len, turn, x, y, direction);

	vector<future<AssessMoveData>> futures;

	for (uint8_t i = 0; i < len; i++) {
		futures.emplace_back(threadPool.add_task([=, &field]() -> AssessMoveData {
			AssessMoveData temp(field);
			memcpy(temp.field, field, 64);

			const auto& move = AllMoves[i];
			uint8_t x1 = move[0];
			uint8_t y1 = move[1];
			uint8_t x2 = move[2];
			uint8_t y2 = move[3];

			temp.coord = move;

			if (ntb) {
				if (temp.field[x1][y1] >= 3) {
					DamkaBeat(temp.field, x1, y1, x2, y2, direction);
				}
				else {
					Beat(temp.field, x1, y1, x2, y2);
				}
				temp.x = x2;
				temp.y = y2;
				temp.type = BEAT;
				temp.direction = GetMode(x1, y1, x2, y2, direction);
				temp.assess = mmAB(temp, BLACK_WIN_ASSESS, WHITE_WIN_ASSESS, depth);
			}
			else {
				Move(temp.field, x1, y1, x2, y2);
				temp.x = 0;
				temp.y = 0;
				temp.type = MOVE;
				temp.direction = NONE;
				temp.turn = !temp.turn;
				temp.assess = mmAB(temp, BLACK_WIN_ASSESS, WHITE_WIN_ASSESS, depth - 1);
			}
			return temp;
			}));
	}

	bestMoves.clear();
	std::transform(futures.begin(), futures.end(), bestMoves.begin(), [](auto& future) -> AssessMoveData {
		return future.get();
		});

	std::sort(bestMoves.begin(), bestMoves.end(), [](const auto& f, const auto& s) {
		return f.assess < s.assess;
		});

	if (turn) {
		std::reverse(bestMoves.begin(), bestMoves.end());
	}
}

#else

Engine::Engine() { }

void Engine::fill(TField& field, MOVE_TYPE type, uint8_t x, uint8_t y, uint8_t direction, bool turn, int depth) {
	TAM AllMoves;
	uint8_t len = 0;
	bool ntb = PMFill(field, type, AllMoves, &len, turn, x, y, direction);

	std::direction<std::future<MoveData>> futures;

	for (uint8_t i = 0; i < len; i++) {
		MoveData temp;
		memcpy(temp.field, field, 64);

		uint8_t x1 = AllMoves[i][0];
		uint8_t y1 = AllMoves[i][1];
		uint8_t x2 = AllMoves[i][2];
		uint8_t y2 = AllMoves[i][3];

		temp.coord[0] = x1;
		temp.coord[1] = y1;
		temp.coord[2] = x2;
		temp.coord[3] = y2;

		if (ntb) {
			if (temp.field[x1][y1] >= 3) {
				DamkaBeat(temp.field, x1, y1, x2, y2, direction);
			}
			else {
				Beat(temp.field, x1, y1, x2, y2);
			}
			temp.x = x2;
			temp.y = y2;
			temp.type = BEAT;
			temp.direction = GetMode(x1, y1, x2, y2, direction);
			temp.assess = mmAB(temp.field, x2, y2, temp.direction, depth, -100, 100, turn);
		}
		else {
			Move(temp.field, x1, y1, x2, y2);
			temp.x = 0;
			temp.y = 0;
			temp.type = MOVE;
			temp.direction = 0;
			temp.assess = mmAB(temp.field, depth - 1, -100, 100, !turn);
		}
		moves.push_back(temp);
	}

	std::sort(moves.begin(), moves.end(), [](const auto& f, const auto& s) {
		return f.assess < s.assess;
		});
	if (turn) {
		std::reverse(moves.begin(), moves.end());
	}
}

#endif

uint8_t Engine::find(Coord coordinates) {
	auto& moves = bestMoves;
	auto it = std::find_if(moves.begin(), moves.end(), [&](const auto& move) -> bool {
		return move.coord == coordinates;
		});

	int index = it == moves.end() 
		? -1 : 
		static_cast<int>(std::distance(it, moves.begin()));

	return index;
}

int16_t Engine::mmAB(AssessMoveData& moveData, int16_t alpha, int16_t beta, uint8_t depth) {

	TAM AllMoves;
	uint8_t len = 0;

	auto& [field, type, coord, x, y, direction, turn, assess] = moveData;

	bool ntb = PMFill(field, type, AllMoves, &len, turn, x, y, direction);

	if (len == 0) {
		if (type == BEAT) {
			AssessMoveData nextTurn(field);
			nextTurn.turn = !turn;
			return mmAB(nextTurn, alpha, beta, depth - 1);
		}
		return turn ? BLACK_WIN_ASSESS : WHITE_WIN_ASSESS;
	}

	if (depth <= 0 && !ntb) {
		return getAssess(field);
	}

	int16_t minmaxEval = turn ? BLACK_WIN_ASSESS : WHITE_WIN_ASSESS;
	int16_t eval = 0;

	for (int i = 0; i < len; i++) {
		MOVE_DIRECTION tempDirection{NONE};
		TField tempBoard;
		memcpy(tempBoard, field, 64);

		uint8_t x1, y1, x2, y2;
		auto& coord = AllMoves[i];
		x1 = coord[0];
		y1 = coord[1];
		x2 = coord[2];
		y2 = coord[3];

		AssessMoveData newMoveData(tempBoard);
		newMoveData.x = x2;
		newMoveData.y = y2;

		if (ntb) {
			if (tempBoard[x1][y1] >= 3) {
				DamkaBeat(tempBoard, x1, y1, x2, y2, tempDirection);
			}
			else {
				Beat(tempBoard, x1, y1, x2, y2);
			}
			newMoveData.direction = GetMode(x1, y1, x2, y2, tempDirection);
			eval = mmAB(newMoveData, alpha, beta, depth);
		}
		else {
			Move(tempBoard, x1, y1, x2, y2);
			newMoveData.turn = !newMoveData.turn;
			eval = mmAB(newMoveData, alpha, beta, depth - 1);
		}

		if (turn) {
			minmaxEval = max(minmaxEval, eval);
			alpha = max(alpha, eval);
		}
		else {
			minmaxEval = min(minmaxEval, eval);
			beta = min(beta, eval);
		}

		if (beta <= alpha) {
			break;
		}
	}
	return minmaxEval;
}

void Engine::fill(AssessMoveData& moveData) {
	TAM AllMoves;
	uint8_t len = 0;

	auto& [field, type, coord, x, y, direction, turn, assess] = moveData;

	bool ntb = PMFill(field, type, AllMoves, &len, turn, x, y, direction);
	bestMoves.clear();
	bestMoves.reserve(len);

	AssessMoveData temp(field);

	for (uint8_t i = 0; i < len; i++) {
		memcpy(temp.field, field, 64);

		uint8_t x1, y1, x2, y2;
		x1 = AllMoves[i][0];
		y1 = AllMoves[i][1];
		x2 = AllMoves[i][2];
		y2 = AllMoves[i][3];

		temp.coord = AllMoves[i];
		temp.type = type;

		if (ntb) {
			if (temp.field[x1][y1] >= 3) {
				DamkaBeat(temp.field, x1, y1, x2, y2, direction);
			}
			else {
				Beat(temp.field, x1, y1, x2, y2);
			}
			temp.x = x2;
			temp.y = y2;
			temp.type = BEAT;
			temp.direction = GetMode(x1, y1, x2, y2, direction);
		}
		else {
			Move(temp.field, x1, y1, x2, y2);
			temp.x = 0;
			temp.y = 0;
			temp.type = MOVE;
			temp.direction = NONE;
		}
		temp.assess = 0;
		bestMoves.push_back(temp);
	}
}


MOVE_RESULT Engine::PlayerMove(AssessMoveData& moveData) {

	auto& [field, type, coord, x, y, direction, turn, assess] = moveData;
	fill(moveData);

	auto& [x1, y1, x2, y2] = moveData.coord;

	uint8_t index = find(moveData.coord);
	if (index != -1) {
		AssessMoveData move = bestMoves[index];
		type = move.type;
		direction = move.direction;
		memcpy(field, move.field, 64);

		if (type == BEAT) {

			direction = GetMode(x1, y1, x2, y2, direction);
			x = x2;
			y = y2;

			fill(moveData);

			if (bestMoves.size() > 0) {
				return ONE_MORE;
			}
			else {
				x = 0;
				y = 0;
				type = MOVE;
				direction = NONE;
			}
		}
		else {
			direction = NONE;
			x = 0;
			y = 0;
		}
		turn = !turn;
		fill(moveData);
		if (bestMoves.size() == 0) {
			return WIN;
		}

		return SUCCESS;
	}
	return INVALID_COORD;
}

MOVE_RESULT Engine::EngineMove(AssessMoveData& moveData, uint8_t depth) {

	auto& [field, type, coord, x, y, direction, turn, assess] = moveData;
	fill(moveData);

	if (bestMoves.size() > 1) {
		fill(moveData, depth);
	}
	if (bestMoves.size() > 0) {
		AssessMoveData move = bestMoves[0];

		moveData.coord = move.coord;
		auto& [x1, y1, x2, y2] = moveData.coord;

		type = move.type;
		direction = move.direction;
		memcpy(field, move.field, 64);

		if (type == BEAT) {

			direction = GetMode(x1, y1, x2, y2, direction);
			x = x2;
			y = y2;

			fill(moveData);

			if (bestMoves.size() > 0) {

				return ONE_MORE;
			}
			else {
				x = 0;
				y = 0;
				type = MOVE;
				direction = NONE;
			}
		}
		else {
			direction = NONE;
			x = 0;
			y = 0;
		}
		turn = !turn;
		fill(moveData);
		if (bestMoves.size() == 0) {
			return WIN;
		}

		return SUCCESS;
	}
	return INVALID_COORD;
}

//void Engine::evaluate(AssessMoveData& data, uint8_t depth) {
//
//	float assess = 0;
//	int index = -1;
//	bool turn = data.turn;
//
//	fill(data.oldfield, data.type, data.x, data.y, data.direction, turn, depth);
//	index = find(data.coord);
//	assess = bestMoves[index].assess;
//	float temp_assess = bestMoves[0].assess;
//
//	if (bestMoves.size() == 1) {
//		data.comment = FORCED;
//	}
//	else if (index == 0) {
//		data.comment = BEST;
//	}
//	else if ((temp_assess < assess + 0.3 || (temp_assess < assess + 1.0 && assess > 5)) && turn || (temp_assess > assess - 0.3 || (temp_assess > assess - 1.0 && assess > 5)) && !turn) {
//		data.comment = GOOD;
//	}
//	else if ((temp_assess < assess + 0.9 || (temp_assess < assess + 3.0 && assess > 5)) && turn || (temp_assess > assess - 0.9 || (temp_assess > assess - 3.0 && assess > 5)) && !turn) {
//		data.comment = INACCURACY;
//	}
//	else {
//		data.comment = BLUNDER;
//	}
//	data.assess = assess;
//}

