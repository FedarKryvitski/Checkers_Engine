#pragma once
#include "Engine.h"

class GameController {
protected:
    Engine engine;
    MOVE_TYPE type;
    mytype x, y, vector;
    TField field;
    bool locked = false;

    void getData(MoveData& source);
    void setData(MoveData& dest);

public:
    bool turn;

    GameController();
    MOVE_RESULT PlayerMove(mytype x1, mytype y1, mytype x2, mytype y2);
    MOVE_RESULT EngineMove(mytype depth);
};

