#pragma once
#include <ThreadPool.h>

#include <vector>

#include "moves.h"

enum MoveStatus {
	FORCED,
	BEST,
	GOOD,
	INACCURACY,
	BLUNDER,
};

enum MoveResult {
	INVALID_COORD,
	ONE_MORE,
	SUCCESS,
	WIN,
	DRAW,
	LOSE,
};

struct AssessMoveData {
	Field field;
	MoveType type{MoveType::MOVE};
	Vector4u coord;
	uint8_t x{};
	uint8_t y{};
	MoveDirection direction{MoveDirection::NONE};
	int16_t assess{};
	bool turn{true};

	void operator=(const AssessMoveData &other) {
		memcpy(field, other.field, 64);
		type = other.type;
		coord = other.coord;
		x = other.x;
		y = other.y;
		direction = other.direction;
		turn = other.turn;
		assess = other.assess;
	}
};

class Engine final {
   public:
	Engine() noexcept;

	MoveResult PlayerMove(AssessMoveData &data);
	MoveResult EngineMove(AssessMoveData &data, int depth);
	// void evaluate(AssessMoveData& data, uint8_t depth);

   private:
	void fill(AssessMoveData &moveData, uint8_t depth);
	void fill(AssessMoveData &moveData);

	int16_t mmAB(AssessMoveData &moveData, int16_t alpha, int16_t beta, uint8_t depth);
	uint8_t find(Vector4u coord);

   private:
	ThreadPool threadPool;
	std::vector<AssessMoveData> bestMoves;
};
