#pragma once
#include "types.h"

extern "C" MoveDirection GetMode(uint8_t, uint8_t, uint8_t, uint8_t, MoveDirection);
extern "C" bool CheckCoord(uint8_t, uint8_t);
extern "C" void BInit(Field &);
extern "C" void Move(Field &, uint8_t, uint8_t, uint8_t, uint8_t);
extern "C" void Beat(Field &, uint8_t, uint8_t, uint8_t, uint8_t);
extern "C" bool SMCheck(const Field &, uint8_t, uint8_t, uint8_t, uint8_t);
extern "C" bool SBCheck(const Field &, uint8_t, uint8_t, uint8_t, uint8_t, bool);
extern "C" int getAssess(const Field &);
extern "C" bool NTBDamka(const Field &, uint8_t, uint8_t, bool, uint8_t);
extern "C" void DamkaBeat(Field &, uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
extern "C" uint8_t amountOfDamka(const Field &);

bool NTBDamkaOneMore(const Field &field, uint8_t x, uint8_t y, bool turn, uint8_t mode);
bool PMFill(const Field &, MoveType, TAllMoves &, uint8_t *, bool type, uint8_t x, uint8_t y, uint8_t vector);
