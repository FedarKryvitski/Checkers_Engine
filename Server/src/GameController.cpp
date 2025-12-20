#include "GameController.h"

void GameController::getData(MoveData& source) {
    BCopy(field, source.field);
    type = source.type;
    vector = source.vector;
    x = source.x;
    y = source.y;
}
void GameController::setData(MoveData& dest) {
    dest.x = x;
    dest.y = y;
    BCopy(dest.field, field);
    BCopy(dest.oldfield, field);
    dest.turn = turn;
    dest.type = type;
    dest.vector = vector;
}
GameController::GameController() {
    type = MOVE;
    x = y = vector = 0;
    turn = true;
    BInit(field);

    MoveData temp;
    setData(temp);
    temp.coord[0] = 0;
    temp.coord[1] = 0;
    temp.coord[2] = 0;
    temp.coord[3] = 0;
}
MOVE_RESULT GameController::PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2) {
    if (!locked) {
        MoveData data;
        setData(data);
        data.coord[0] = x1;
        data.coord[1] = y1;
        data.coord[2] = x2;
        data.coord[3] = y2;

        MoveData temp = data;
        MOVE_RESULT result = engine.PlayerMove(data);
        if (result != INVALID_COORD) {
            BCopy(temp.field, data.field);
            getData(data);
            if (result == SUCCESS) {
                turn = !turn;
            }
        }
        return result;
    }
    return INVALID_COORD;
}
MOVE_RESULT GameController::EngineMove(mytype depth) {
    if (!locked) {
        MoveData data;
        setData(data);

        MoveData temp = data;
        MOVE_RESULT result = engine.EngineMove(data, depth);
        if (result == ONE_MORE || result == SUCCESS) {
            BCopy(temp.field, data.field);
            temp.coord[0] = data.coord[0];
            temp.coord[1] = data.coord[1];
            temp.coord[2] = data.coord[2];
            temp.coord[3] = data.coord[3];
            getData(data);
            if (result == SUCCESS) {
                turn = !turn;
            }
            else if (result == WIN) {
                locked = true;
            }
        }
        return result;
    }
    return INVALID_COORD;
}