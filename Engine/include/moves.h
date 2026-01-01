#pragma once
#include <cstdint>
#include <memory>

typedef uint8_t TField[8][8];
typedef uint8_t TAM[100][4];

enum MOVE_TYPE {
	MOVE,
	BEAT
};

enum MOVE_DIRECTION {
	NONE,
	UP_RIGHT,
	DOWN_RIGHT,
	DOWN_LEFT,
	UP_LEFT
};

struct Coord {
	uint8_t x1 : 3 {0};
	uint8_t y1 : 3 {0};
	uint8_t x2 : 3 {0};
	uint8_t y2 : 3 {0};

	bool operator==(const Coord& other) const {
		return !memcmp(this, &other, sizeof(Coord));
	}

	void operator=(const uint8_t coord[4]) {
		x1 = coord[0];
		y1 = coord[1];
		x2 = coord[2];
		y2 = coord[3];
	}
};

struct MoveData {
	MoveData(TField& _field) : field(_field) {};

	TField& field;
	MOVE_TYPE type : 1 {MOVE};
	Coord coord;
	uint8_t x : 3 {0};
	uint8_t y : 3 {0};
	MOVE_DIRECTION direction : 3 {NONE};
	bool turn : 1 {true};
};

extern "C" MOVE_DIRECTION GetMode(uint8_t, uint8_t, uint8_t, uint8_t, MOVE_DIRECTION);
extern "C" bool CheckCoord(uint8_t, uint8_t);
extern "C" void BInit(TField&);
extern "C" void Move(TField&, uint8_t, uint8_t, uint8_t, uint8_t);
extern "C" void Beat(TField&, uint8_t, uint8_t, uint8_t, uint8_t);
extern "C" bool SMCheck(TField&, uint8_t, uint8_t, uint8_t, uint8_t);
extern "C" bool SBCheck(TField&, uint8_t, uint8_t, uint8_t, uint8_t, bool);
extern "C" int getAssess(TField&);
extern "C" bool NTBDamka(TField&, uint8_t, uint8_t, bool, uint8_t);
extern "C" void DamkaBeat(TField&, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
extern "C" uint8_t amountOfDamka(TField&);

bool NTBDamkaOneMore(TField& field, uint8_t x, uint8_t y, bool turn, uint8_t mode);

bool PMFill(TField&, MOVE_TYPE, TAM&, uint8_t*, bool type, uint8_t x, uint8_t y, uint8_t vector);
