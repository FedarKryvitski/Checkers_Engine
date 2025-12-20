#pragma once
#include "PossibleMoves.h"
#include <vector>
#include <any>
#include "ThreadPool.h"

#define THREADS

using std::vector;

enum MOVE_STATUS {
    FORCED,
    BEST,
    GOOD,
    INACCURACY,
    BLUNDER,
};

struct AssessMoveData {
    AssessMoveData(TField& _field) : field(_field) {};

    TField& field;
    MOVE_TYPE type : 1 {MOVE};
    Coord coord;
    uint8_t x : 3 {0};
    uint8_t y : 3 {0};
    MOVE_DIRECTION direction : 3 {NONE};
    bool turn : 1 {true};
    int16_t assess{0};

    void operator=(const AssessMoveData& other) {
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

enum MOVE_RESULT {
    INVALID_COORD,
    ONE_MORE,
    SUCCESS,
    WIN,
    DRAW,
    LOSE,
};

class Engine {
public:
    Engine();
    [[nodiscard]] MOVE_RESULT PlayerMove(AssessMoveData& data);
    [[nodiscard]] MOVE_RESULT EngineMove(AssessMoveData& data, uint8_t depth);
    //void evaluate(AssessMoveData& data, uint8_t depth);

private:
#ifdef THREADS
    ThreadPool<AssessMoveData> threadPool;
#endif
    vector<AssessMoveData> bestMoves;

    uint8_t find(Coord coord);
    void fill(AssessMoveData& moveData, uint8_t depth);
    void fill(AssessMoveData& moveData);
    int16_t mmAB(AssessMoveData& moveData, int16_t alpha, int16_t beta, uint8_t depth);
};


