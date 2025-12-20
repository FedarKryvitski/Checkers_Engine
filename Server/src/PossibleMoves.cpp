#include "PossibleMoves.h"

extern "C" void asmAdd(TAM&, mytype*, mytype, mytype, mytype, mytype);
extern "C" void FillDamkaMoves(TField&, TAM&, mytype*, mytype, mytype);

bool NTBDamkaOneMore(TField& field, mytype x, mytype y, bool turn, mytype mode) {
	if (NTBDamka(field, x, y, turn, mode)) {
		return true;
	}
	if (mode == 1) {
		x++;
		y++;
		while (CheckCoord(x, y) && (field[x][y] == 0)) {
			if (NTBDamka(field, x++, y++, turn, 1)) {
				return true;
			}
		}
	}
	else if (mode == 2) {
		x--;
		y++;
		while (CheckCoord(x, y) && (field[x][y] == 0)) {
			if (NTBDamka(field, x--, y++, turn, 2)) {
				return true;
			}
		}
	}
	else if (mode == 3) {
		x++;
		y--;
		while (CheckCoord(x, y) && (field[x][y] == 0)) {
			if (NTBDamka(field, x++, y--, turn, 3)) {
				return true;
			}
		}
	}
	else if (mode == 4) {
		x--;
		y--;
		while (CheckCoord(x, y) && (field[x][y] == 0)) {
			if (NTBDamka(field, x--, y--, turn, 4)) {
				return true;
			}
		}
	}
	return false;
}

void FillDamkaBeatsDiag(TField& field, bool turn, TAM& AllMoves, mytype* len, mytype x0, mytype y0, mytype x, mytype y, mytype mode) {
	mytype temp = 1;
	if (turn) {
		temp = 2;
	}
	if (mode == 1) {
		while (CheckCoord(++x, ++y)) {
			if (field[x][y] != 0) {
				break;
			}
		}
		if (CheckCoord(x + 1, y + 1)) {
			if ((field[x + 1][y + 1] == 0) && ((field[x][y] == temp) || (field[x][y] == temp + 2))) {
				asmAdd(AllMoves, len, x0, y0, ++x, ++y);
				if (!NTBDamkaOneMore(field, x++, y++, turn, 1)) {
					while (CheckCoord(x, y) && (field[x][y] == 0)) {
						asmAdd(AllMoves, len, x0, y0, x++, y++);
					}
				}
			}
		}
	}
	else if (mode == 2) {
		while (CheckCoord(--x, ++y)) {
			if (field[x][y] != 0) {
				break;
			}
		}
		if (CheckCoord(x - 1, y + 1)) {
			if ((field[x - 1][y + 1] == 0) && ((field[x][y] == temp) || (field[x][y] == temp + 2))) {
				asmAdd(AllMoves, len, x0, y0, --x, ++y);
				if (!NTBDamkaOneMore(field, x--, y++, turn, 2)) {
					while (CheckCoord(x, y) && (field[x][y] == 0)) {
						asmAdd(AllMoves, len, x0, y0, x--, y++);
					}
				}
			}
		}
	}
	else if (mode == 3) {
		while (CheckCoord(++x, --y)) {
			if (field[x][y] != 0) {
				break;
			}
		}
		if (CheckCoord(x + 1, y - 1)) {
			if ((field[x + 1][y - 1] == 0) && ((field[x][y] == temp) || (field[x][y] == temp + 2))) {
				asmAdd(AllMoves, len, x0, y0, ++x, --y);
				if (!NTBDamkaOneMore(field, x++, y--, turn, 3)) {
					while (CheckCoord(x, y) && (field[x][y] == 0)) {
						asmAdd(AllMoves, len, x0, y0, x++, y--);
					}
				}
			}
		}
	}
	else if (mode == 4) {
		while (CheckCoord(--x, --y)) {
			if (field[x][y] != 0) {
				break;
			}
		}
		if (CheckCoord(x - 1, y - 1)) {
			if ((field[x - 1][y - 1] == 0) && ((field[x][y] == temp) || (field[x][y] == temp + 2))) {
				asmAdd(AllMoves, len, x0, y0, --x, --y);
				if (!NTBDamkaOneMore(field, x--, y--, turn, 4)) {
					while (CheckCoord(x, y) && (field[x][y] == 0)) {
						asmAdd(AllMoves, len, x0, y0, x--, y--);
					}
				}
			}
		}
	}
}	
void FillDamkaBeatsForOne(TField& field, bool turn, TAM& AllMoves, mytype* len, mytype x, mytype y, mytype mode) {
	mytype x0 = x;
	mytype y0 = y;
	if (mode != 1) {
		FillDamkaBeatsDiag(field, turn, AllMoves, len, x0, y0, x, y, 4);
	}
	if (mode != 2) {
		FillDamkaBeatsDiag(field, turn, AllMoves, len, x0, y0, x, y, 3);
	}
	if (mode != 3) {
		FillDamkaBeatsDiag(field, turn, AllMoves, len, x0, y0, x, y, 2);
	}
	if (mode != 4) {
		FillDamkaBeatsDiag(field, turn, AllMoves, len, x0, y0, x, y, 1);
	}
	if (mode == 1) {
		while (CheckCoord(++x, ++y) && (field[x][y] == 0)) {
			FillDamkaBeatsDiag(field, turn, AllMoves, len, x0, y0, x, y, 2);
			FillDamkaBeatsDiag(field, turn, AllMoves, len, x0, y0, x, y, 3);
		}
	}
	else if (mode == 2) {
		while (CheckCoord(--x, ++y) && (field[x][y] == 0)) {
			FillDamkaBeatsDiag(field, turn, AllMoves, len, x0, y0, x, y, 1);
			FillDamkaBeatsDiag(field, turn, AllMoves, len, x0, y0, x, y, 4);
		}
	}
	else if (mode == 3) {
		while (CheckCoord(++x, --y) && (field[x][y] == 0)) {
			FillDamkaBeatsDiag(field, turn, AllMoves, len, x0, y0, x, y, 1);
			FillDamkaBeatsDiag(field, turn, AllMoves, len, x0, y0, x, y, 4);
		}
	}
	else if (mode == 4) {
		while (CheckCoord(--x, --y) && (field[x][y] == 0)) {
			FillDamkaBeatsDiag(field, turn, AllMoves, len, x0, y0, x, y, 2);
			FillDamkaBeatsDiag(field, turn, AllMoves, len, x0, y0, x, y, 3);
		}
	}
}
void FillBeatsForOne(TField& field, bool turn, TAM& AllMoves, mytype* len, mytype x, mytype y) {
	if (SBCheck(field, x, y, x + 2, y + 2, turn)) {
		asmAdd(AllMoves, len, x, y, x + 2, y + 2);
	}
	if (SBCheck(field, x, y, x - 2, y + 2, turn)) {
		asmAdd(AllMoves, len, x, y, x - 2, y + 2);
	}
	if (SBCheck(field, x, y, x + 2, y - 2, turn)) {
		asmAdd(AllMoves, len, x, y, x + 2, y - 2);
	}
	if (SBCheck(field, x, y, x - 2, y - 2, turn)) {
		asmAdd(AllMoves, len, x, y, x - 2, y - 2);
	}
}
bool FillMoves(TField& field, bool turn, TAM& AllMoves, mytype* len) {
	mytype temp = 2;
	if (turn) {
		temp = 1;
	}
	for (mytype i = 0; i < 8; ++i) {
		for (mytype j = 0; j < 8; ++j) {
			if (field[i][j] == temp){
				FillBeatsForOne(field, turn, AllMoves, len, i, j);
			}
			else if (field[i][j] == temp + 2) {
				FillDamkaBeatsForOne(field, turn, AllMoves, len, i, j, 0);

			}
		}
	}
	if (*len != 0) {
		return true;
	}
	mytype dy = -1;
	if (turn) {
		dy = 1;
	}
	for (mytype x = 0; x < 8; ++x) {
		for (mytype y = 0; y < 8; ++y) {
			if (field[x][y] == temp) {
				if (SMCheck(field, x, y, x + 1, y + dy)) {
					asmAdd(AllMoves, len, x, y, x + 1, y + dy);
				}
				if (SMCheck(field, x, y, x - 1, y + dy)) {
					asmAdd(AllMoves, len, x, y, x - 1, y + dy);
				}
			}
			else if (field[x][y] == temp + 2) {
				FillDamkaMoves(field, AllMoves, len, x, y);
			}
		}
	}
	return false;
}

bool PMFill(TField& field, MOVE_TYPE type, TAM& AllMoves, mytype* len, bool turn, mytype x, mytype y, mytype vector) {
	if (type) {
		if (field[x][y] >= 3) {
			FillDamkaBeatsForOne(field, turn, AllMoves, len, x, y, vector);
		}
		else {
			FillBeatsForOne(field, turn, AllMoves, len, x, y);
		}
		return *len > 0;
	}
	else {
		return FillMoves(field, turn, AllMoves, len);
	}
}

