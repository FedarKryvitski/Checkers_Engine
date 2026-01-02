#pragma once
#include "controls.h"
#include "GameControllers.h"
#include "form.h"

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <array>
#include <mutex>
#include <vector>
#include <memory>

extern sf::Font font;
extern sf::Image icon;

namespace Forms {
    
class PvpForm final : public Form {
public:
    PvpForm();
    ~PvpForm();

protected:
    void onDraw() override;

private:
    bool turn{ true };
    bool LeftPressed{ false };
    bool LeftReleased{ false };
    bool connected{ false };

    std::vector<int> vMoves;

    Controls::CheckersClock clock1, clock2;
    Controls::Button exitB, flipB, analysicsB, drawB, resignB;
    Controls::Board board;
    Controls::TWait wait;
    GameController control;
    Controls::Label lDraw, lLose, lWin;

    enum Type {
        INIT,
        MOVEREQ,
        DRAWREQ,
        RESIGNREQ
    };

    void resign();
    void offerdraw();
    void sendMove(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
    void receive();
    void loading();
};

} // namespace Forms