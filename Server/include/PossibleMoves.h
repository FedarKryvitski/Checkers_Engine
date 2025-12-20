#pragma once
#define mytype signed char
typedef mytype TField[8][8];
typedef mytype TAM[100][4];

typedef enum MOVE_TYPE {
	MOVE,
	BEAT
};

extern "C" mytype GetMode(mytype, mytype, mytype, mytype, mytype);
extern "C" bool CheckCoord(mytype, mytype);
extern "C" void BInit(TField&);
extern "C" void BCopy(TField&, TField&);
extern "C" void Move(TField&, mytype, mytype, mytype, mytype);
extern "C" void Beat(TField&, mytype, mytype, mytype, mytype);
extern "C" bool SMCheck(TField&, mytype, mytype, mytype, mytype);
extern "C" bool SBCheck(TField&, mytype, mytype, mytype, mytype, bool);
extern "C" int getAssess(TField&);
extern "C" bool NTBDamka(TField&, mytype, mytype, bool, mytype);
extern "C" void DamkaBeat(TField&, mytype, mytype, mytype, mytype, mytype);
extern "C" mytype amountOfDamka(TField&);

bool NTBDamkaOneMore(TField& field, mytype x, mytype y, bool turn, mytype mode);

bool PMFill(TField&, MOVE_TYPE, TAM&, mytype*, bool type, mytype x, mytype y, mytype vector);
