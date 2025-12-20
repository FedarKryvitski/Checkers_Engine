#pragma once
#include "Engine.h"
#include <vector>

using std::vector, std::tuple;

class GameController final {
public:
    TField field;
    float assess{0.f};
    int curr{ 0 }, head{ 0 };
    bool turn{true};

    vector<AssessMoveData> gameMoves;
    GameController();
    [[nodiscard]] MOVE_RESULT playerMove(tuple<uint8_t, uint8_t, uint8_t, uint8_t>);
    [[nodiscard]] MOVE_RESULT engineMove(uint8_t depth);
    void getPrev();
    void getNext();
    void getCurr();

private:
    Engine engine;
    MOVE_TYPE type{MOVE};
    uint8_t x{0}, y{0};
    MOVE_DIRECTION direction{NONE};
    bool locked{ false };

    void getData(AssessMoveData& source);
    void setData(AssessMoveData& dest);

};

class AnalysicsController final {
public:
    TField field{};
    float assess{0.f};
    MOVE_STATUS comment{FORCED};
    uint8_t x1{0}, y1{0}, x2{0}, y2{0};

    vector<AssessMoveData> gameMoves;
    AnalysicsController();
    void evaluate(int index, int depth);
    void setMoves(vector<AssessMoveData>& tgameMoves);
    void getPrev();
    void getNext();
    void getCurr();

private:
    bool turn;

    Engine engine;
    MOVE_TYPE type{MOVE};
    uint8_t x{ 0 }, y{0};
    MOVE_DIRECTION direction{ NONE };
    int curr{ 0 }, head{0};

    void getData(AssessMoveData& source);
};
