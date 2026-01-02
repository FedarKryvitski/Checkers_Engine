#pragma once
#include <array>
#include <cstdint>
#include <memory>

using Field = std::array<std::array<uint8_t, 8>, 8>;
using TAllMoves = std::array<std::array<uint8_t, 4>, 100>;

enum class MoveType { MOVE, BEAT };

enum class MoveDirection { NONE, UP_RIGHT, DOWN_RIGHT, DOWN_LEFT, UP_LEFT };

struct Vector4u {
	uint8_t x1{};
	uint8_t y1{};
	uint8_t x2{};
	uint8_t y2{};

	bool operator==(const Vector4u &other) const {
		return x1 == other.x1 && y1 == other.y1 && x2 == other.x2 && y2 == other.y2;
	}

	void operator=(const Vector4u &other) {
		x1 = other.x1;
		y1 = other.y1;
		x2 = other.x2;
		y2 = other.y2;
	}
};

struct MoveData {
	Field &field;
	MoveType type{MoveType::MOVE};
	Vector4u coord;
	uint8_t x{};
	uint8_t y{};
	MoveDirection direction{MoveDirection::NONE};
	bool turn{true};
};

extern "C" MoveDirection GetMode(uint8_t, uint8_t, uint8_t, uint8_t, MoveDirection);
extern "C" bool CheckCoord(uint8_t, uint8_t);
extern "C" void BInit(Field &);
extern "C" void Move(Field &, uint8_t, uint8_t, uint8_t, uint8_t);
extern "C" void Beat(Field &, uint8_t, uint8_t, uint8_t, uint8_t);
extern "C" bool SMCheck(Field &, uint8_t, uint8_t, uint8_t, uint8_t);
extern "C" bool SBCheck(Field &, uint8_t, uint8_t, uint8_t, uint8_t, bool);
extern "C" int getAssess(Field &);
extern "C" bool NTBDamka(Field &, uint8_t, uint8_t, bool, uint8_t);
extern "C" void DamkaBeat(Field &, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
extern "C" uint8_t amountOfDamka(Field &);

bool NTBDamkaOneMore(Field &field, uint8_t x, uint8_t y, bool turn, uint8_t mode);

bool PMFill(Field &, MoveType, TAllMoves &, uint8_t *, bool type, uint8_t x, uint8_t y, uint8_t vector);
