#pragma once
#include <vector>

#include "Engine.h"

class GameController final {
   public:
	GameController();

	MoveResult playerMove(Vector4u coordinates);
	MoveResult engineMove(int depth);

	void getPrev();
	void getNext();
	void getCurr();

   private:
	void getData(AssessMoveData &source);
	void setData(AssessMoveData &dest);

   private:
	Field field;
	float assess{0.f};
	int curr{0}, head{0};
	bool turn{true};

	std::vector<AssessMoveData> gameMoves;

	Engine engine;
	MoveType type{MoveType::MOVE};
	uint8_t x{}, y{};
	MoveDirection direction{MoveDirection::NONE};
	bool locked{false};
};

class AnalysicsController final {
   public:
	Field field{};
	float assess{0.f};
	MoveStatus comment{MoveStatus::FORCED};
	uint8_t x1{0}, y1{0}, x2{0}, y2{0};

	std::vector<AssessMoveData> gameMoves;
	AnalysicsController();
	void evaluate(int index, int depth);
	void setMoves(std::vector<AssessMoveData> &tgameMoves);
	void getPrev();
	void getNext();
	void getCurr();

   private:
	bool turn;

	Engine engine;
	MoveType type{MOVE};
	uint8_t x{0}, y{0};
	MoveDirection direction{NONE};
	int curr{0}, head{0};

	void getData(AssessMoveData &source);
};
