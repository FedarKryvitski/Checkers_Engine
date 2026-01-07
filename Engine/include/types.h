#pragma once
#include <array>
#include <cstdint>

namespace {

constexpr int kAllMovesSize = 100;

} // namespace

struct Vector4u {
	uint8_t x1{};
	uint8_t y1{};
	uint8_t x2{};
	uint8_t y2{};

	bool operator==(const Vector4u &other) const {
		return x1 == other.x1 && y1 == other.y1 && x2 == other.x2 && y2 == other.y2;
	}
};

using Field = std::array<std::array<uint8_t, 8>, 8>;
using MoveVector = Vector4u;
using TAllMoves = std::array<MoveVector, kAllMovesSize>;

enum MoveType : uint8_t { MOVE, BEAT };
enum MoveDirection : uint8_t { NONE, UP_RIGHT, DOWN_RIGHT, DOWN_LEFT, UP_LEFT };
enum MoveStatus : uint8_t {
	FORCED,
	BEST,
	GOOD,
	INACCURACY,
	BLUNDER,
};

enum MoveResult : uint8_t {
	INVALID_COORD,
	ONE_MORE,
	SUCCESS,
	WIN,
	DRAW,
	LOSE,
};

enum class GameState : uint8_t { InProgress, WhiteWins, BlackWins, Draw, Stalemate };

struct StateData {
	Field field;
	MoveType type{MoveType::MOVE};
	MoveVector coord;
	uint8_t x{};
	uint8_t y{};
	MoveDirection direction{MoveDirection::NONE};
	bool turn{true};
};

struct AssessedData : StateData {
	int assessment;
};