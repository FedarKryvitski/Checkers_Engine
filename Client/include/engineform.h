#pragma once
#include "form.h"

#include <array>
#include <mutex>
#include <vector>
#include <memory>

extern sf::Font font;
extern sf::Image icon;

namespace Forms {
    
class EngineForm final: public Form {
public:
    EngineForm();
    ~EngineForm();

protected:
    void onDraw() override;

    void onClose() override;

    void onLeftButtonPress(sf::Vector2i position) override;

    void onLeftButtonRelease(sf::Vector2i position) override;

private:
    void engineMove();

private:
    Controls::Button exitButton, flipButton, analysicsButton;
    Controls::Label resultLabel, timeLabel;
    Controls::Board board;
    GameController control;
    //std::unique_ptr<std::thread> engineThread;
    //std::mutex labelMutex;

    //bool LeftPressed{ false };
    //sf::Vector2i LeftPressedPosition{};
};

} // namespace Forms