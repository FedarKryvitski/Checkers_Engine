#include "Engine.h"

mytype Engine::find(mytype x1, mytype y1, mytype x2, mytype y2) {
	for (int i = 0; i < moves.size(); i++) {
		if (moves[i].coord[0] == x1 && moves[i].coord[1] == y1 && moves[i].coord[2] == x2 && moves[i].coord[3] == y2) {
			return i;
		}
	}
	return -1;
}
float Engine::mmAB(TField& field, int depth, float alpha, float beta, bool turn) {

	TAM AllMoves;
	mytype len = 0;
	bool ntb = PMFill(field, MOVE, AllMoves, &len, turn, 0, 0, 0);

	if (len == 0) {
		if (turn) {
			return -100;
		}
		return 100;
	}

	if ((depth <= 0) && (!ntb)) {
		return (float)getAssess(field) / 10;
	}
	if (turn) {
		float maxEval = -100;
		float eval;
		for (int i = 0; i < len; i++) {
			mytype tempVector = 0;
			TField TempBoard;
			BCopy(TempBoard, field);

			mytype x1, y1, x2, y2;
			mytype* temp = AllMoves[i];
			x1 = temp[0];
			y1 = temp[1];
			x2 = temp[2];
			y2 = temp[3];
			if (ntb) {
				if (TempBoard[x1][y1] >= 3) {
					DamkaBeat(TempBoard, x1, y1, x2, y2, tempVector);
				}
				else {
					Beat(TempBoard, x1, y1, x2, y2);
				}
				tempVector = GetMode(x1, y1, x2, y2, tempVector);
				eval = mmAB(TempBoard, x2, y2, tempVector, depth, alpha, beta, turn);
			}
			else {
				Move(TempBoard, x1, y1, x2, y2);
				eval = mmAB(TempBoard, depth - 1, alpha, beta, !turn);
			}
			maxEval = std::max(maxEval, eval);
			alpha = std::max(alpha, eval);
			if (beta <= alpha) {
				break;
			}
		}
		return maxEval;
	}
	else {
		float minEval = 100;
		float eval;
		for (int i = 0; i < len; i++) {
			mytype tempVector = 0;
			TField TempBoard;
			BCopy(TempBoard, field);

			mytype x1, y1, x2, y2;
			mytype* temp = AllMoves[i];
			x1 = temp[0];
			y1 = temp[1];
			x2 = temp[2];
			y2 = temp[3];
			if (ntb) {
				if (TempBoard[x1][y1] >= 3) {
					DamkaBeat(TempBoard, x1, y1, x2, y2, tempVector);
				}
				else {
					Beat(TempBoard, x1, y1, x2, y2);
				}
				tempVector = GetMode(x1, y1, x2, y2, tempVector);
				eval = mmAB(TempBoard, x2, y2, tempVector, depth, alpha, beta, turn);
			}
			else {
				Move(TempBoard, x1, y1, x2, y2);
				eval = mmAB(TempBoard, depth - 1, alpha, beta, !turn);
			}

			minEval = std::min(minEval, eval);
			beta = std::min(beta, eval);
			if (beta <= alpha) {
				break;
			}
		}
		return minEval;
	}
}
float Engine::mmAB(TField& field, mytype x, mytype y, mytype vector, int depth, float alpha, float beta, bool turn) {

	TAM AllMoves;
	mytype len = 0;
	bool ntb = PMFill(field, BEAT, AllMoves, &len, turn, x, y, vector);

	if (len == 0) {
		return mmAB(field, depth - 1, alpha, beta, !turn);
	}

	if ((depth <= 0) && (!ntb)) {
		return (float)getAssess(field) / 10;
	}
	if (turn) {
		float maxEval = -100;
		float eval;
		for (int i = 0; i < len; i++) {
			mytype tempVector = vector;
			TField TempBoard;
			BCopy(TempBoard, field);

			mytype x1, y1, x2, y2;
			mytype* temp = AllMoves[i];
			x1 = temp[0];
			y1 = temp[1];
			x2 = temp[2];
			y2 = temp[3];
			if (ntb) {
				if (TempBoard[x1][y1] >= 3) {
					DamkaBeat(TempBoard, x1, y1, x2, y2, tempVector);
				}
				else {
					Beat(TempBoard, x1, y1, x2, y2);
				}
				tempVector = GetMode(x1, y1, x2, y2, tempVector);
				eval = mmAB(TempBoard, x2, y2, tempVector, depth, alpha, beta, turn);
			}
			else {
				Move(TempBoard, x1, y1, x2, y2);
				eval = mmAB(TempBoard, depth - 1, alpha, beta, !turn);
			}
			maxEval = std::max(maxEval, eval);
			alpha = std::max(alpha, eval);
			if (beta <= alpha) {
				break;
			}
		}
		return maxEval;
	}
	else {
		float minEval = 100;
		float eval;
		for (int i = 0; i < len; i++) {
			mytype tempVector = vector;
			TField TempBoard;
			BCopy(TempBoard, field);

			mytype x1, y1, x2, y2;
			mytype* temp = AllMoves[i];
			x1 = temp[0];
			y1 = temp[1];
			x2 = temp[2];
			y2 = temp[3];
			if (ntb) {
				if (TempBoard[x1][y1] >= 3) {
					DamkaBeat(TempBoard, x1, y1, x2, y2, tempVector);
				}
				else {
					Beat(TempBoard, x1, y1, x2, y2);
				}
				tempVector = GetMode(x1, y1, x2, y2, tempVector);
				eval = mmAB(TempBoard, x2, y2, tempVector, depth, alpha, beta, turn);
			}
			else {
				Move(TempBoard, x1, y1, x2, y2);
				eval = mmAB(TempBoard, depth - 1, alpha, beta, !turn);
			}

			minEval = std::min(minEval, eval);
			beta = std::min(beta, eval);
			if (beta <= alpha) {
				break;
			}
		}
		return minEval;
	}
}
void Engine::fill(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, bool turn) {
	TAM AllMoves;
	mytype len = 0;
	bool ntb = PMFill(field, type, AllMoves, &len, turn, x, y, vector);
	moves.clear();

	MoveData temp;

	for (mytype i = 0; i < len; i++) {
		BCopy(temp.field, field);

		mytype x1, y1, x2, y2;
		x1 = AllMoves[i][0];
		y1 = AllMoves[i][1];
		x2 = AllMoves[i][2];
		y2 = AllMoves[i][3];

		temp.coord[0] = x1;
		temp.coord[1] = y1;
		temp.coord[2] = x2;
		temp.coord[3] = y2;
		temp.type = type;

		if (ntb) {
			if (temp.field[x1][y1] >= 3) {
				DamkaBeat(temp.field, x1, y1, x2, y2, vector);
			}
			else {
				Beat(temp.field, x1, y1, x2, y2);
			}
			temp.x = x2;
			temp.y = y2;
			temp.type = BEAT;
			temp.vector = GetMode(x1, y1, x2, y2, vector);
		}
		else {
			Move(temp.field, x1, y1, x2, y2);
			temp.x = 0;
			temp.y = 0;
			temp.type = MOVE;
			temp.vector = 0;
		}
		temp.assess = 0;
		moves.push_back(temp);
	}
}
void Engine::fill(TField& field, MOVE_TYPE type, mytype x, mytype y, mytype vector, bool turn, int depth) {

	TAM AllMoves;
	mytype len = 0;
	bool ntb = PMFill(field, type, AllMoves, &len, turn, x, y, vector);
	moves.clear();

	MoveData temp;

	for (mytype i = 0; i < len; i++) {
		BCopy(temp.field, field);

		mytype x1, y1, x2, y2;

		x1 = AllMoves[i][0];
		y1 = AllMoves[i][1];
		x2 = AllMoves[i][2];
		y2 = AllMoves[i][3];

		temp.coord[0] = x1;
		temp.coord[1] = y1;
		temp.coord[2] = x2;
		temp.coord[3] = y2;

		if (ntb) {
			if (temp.field[x1][y1] >= 3) {
				DamkaBeat(temp.field, x1, y1, x2, y2, vector);
			}
			else {
				Beat(temp.field, x1, y1, x2, y2);
			}
			temp.x = x2;
			temp.y = y2;
			temp.type = BEAT;
			temp.vector = GetMode(x1, y1, x2, y2, vector);
			temp.assess = mmAB(temp.field, x2, y2, temp.vector, depth, -100, 100, turn);
		}
		else {
			Move(temp.field, x1, y1, x2, y2);
			temp.x = 0;
			temp.y = 0;
			temp.type = MOVE;
			temp.vector = 0;
			temp.assess = mmAB(temp.field, depth - 1, -100, 100, !turn);
		}
		moves.push_back(temp);
	}

	for (int i = 0; i < len; i++) {
		for (int j = len - 1; j > i; j--) {
			if (moves[j].assess < moves[j - 1].assess) {
				temp = moves[j];
				moves[j] = moves[j - 1];
				moves[j - 1] = temp;
			}
		}
	}
	if (turn) {
		for (int i = 0; i < len / 2; i++) {
			temp = moves[i];
			moves[i] = moves[len - i - 1];
			moves[len - i - 1] = temp;
		}
	}
}

MOVE_RESULT Engine::PlayerMove(MoveData& data) {

	TField field;
	BCopy(field, data.field);
	MOVE_TYPE type = data.type;
	mytype x = data.x;
	mytype y = data.y;
	mytype vector = data.vector;
	float assess = 0;
	bool turn = data.turn;

	fill(field, type, x, y, vector, turn);

	mytype x1 = data.coord[0];
	mytype y1 = data.coord[1];
	mytype x2 = data.coord[2];
	mytype y2 = data.coord[3];

	mytype index = find(x1, y1, x2, y2);
	if (index != -1) {
		MoveData move = moves[index];
		type = move.type;
		vector = move.vector;
		BCopy(field, move.field);

		if (type == BEAT) {

			vector = GetMode(x1, y1, x2, y2, vector);
			x = x2;
			y = y2;

			data.type = BEAT;
			data.vector = vector;
			data.x = x;
			data.y = y;
			BCopy(data.field, field);

			fill(field, type, x, y, vector, turn);

			if (moves.size() > 0) {

				return ONE_MORE;
			}
			else {
				data.x = 0;
				data.y = 0;
				data.type = MOVE;
				data.vector = 0;
			}
		}
		else {
			data.type = MOVE;
			data.vector = 0;
			data.x = 0;
			data.y = 0;
			BCopy(data.field, field);
		}

		fill(field, MOVE, 0, 0, 0, !turn);
		if (moves.size() == 0) {
			return WIN;
		}

		return SUCCESS;
	}
	return INVALID_COORD;
}
MOVE_RESULT Engine::EngineMove(MoveData& data, mytype depth) {

	TField field;
	BCopy(field, data.field);
	MOVE_TYPE type = data.type;
	mytype x = data.x;
	mytype y = data.y;
	mytype vector = data.vector;
	float assess = 0;
	bool turn = data.turn;

	fill(field, type, x, y, vector, turn);

	if (moves.size() > 1) {
		fill(field, type, x, y, vector, turn, depth);
	}
	if (moves.size() > 0) {
		MoveData move = moves[0];

		mytype x1 = data.coord[0] = move.coord[0];
		mytype y1 = data.coord[1] = move.coord[1];
		mytype x2 = data.coord[2] = move.coord[2];
		mytype y2 = data.coord[3] = move.coord[3];

		type = move.type;
		vector = move.vector;
		BCopy(field, move.field);

		if (type == BEAT) {

			vector = GetMode(x1, y1, x2, y2, vector);
			x = x2;
			y = y2;

			data.type = BEAT;
			data.vector = vector;
			data.x = x;
			data.y = y;
			BCopy(data.field, field);

			fill(field, type, x, y, vector, turn);

			if (moves.size() > 0) {

				return ONE_MORE;
			}
			else {
				data.x = 0;
				data.y = 0;
				data.type = MOVE;
				data.vector = 0;
			}
		}
		else {
			data.type = MOVE;
			data.vector = 0;
			data.x = 0;
			data.y = 0;
			BCopy(data.field, field);
		}

		fill(field, MOVE, 0, 0, 0, !turn);
		if (moves.size() == 0) {
			return WIN;
		}

		return SUCCESS;
	}
	return INVALID_COORD;
}

void Engine::evaluate(MoveData& data, mytype depth) {

	float assess = 0;
	int index = -1;
	bool turn = data.turn;

	fill(data.oldfield, data.type, data.x, data.y, data.vector, turn, depth);
	index = find(data.coord[0], data.coord[1], data.coord[2], data.coord[3]);
	assess = moves[index].assess;
	float temp_assess = moves[0].assess;

	if (moves.size() == 1) {
		data.comment = FORCED;
	}
	else if (index == 0) {
		data.comment = BEST;
	}
	else if ((temp_assess < assess + 0.3 || (temp_assess < assess + 1.0 && assess > 5)) && turn || (temp_assess > assess - 0.3 || (temp_assess > assess - 1.0 && assess > 5)) && !turn) {
		data.comment = GOOD;
	}
	else if ((temp_assess < assess + 0.9 || (temp_assess < assess + 3.0 && assess > 5)) && turn || (temp_assess > assess - 0.9 || (temp_assess > assess - 3.0 && assess > 5)) && !turn) {
		data.comment = INACCURACY;
	}
	else {
		data.comment = BLUNDER;
	}
	data.assess = assess;
}