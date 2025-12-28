#pragma once
#include "controls.h"
#include "GameControllers.h"

#include <SFML/Network.hpp>
#include <SFML/Graphics.hpp>

#include <array>
#include <mutex>
#include <vector>
#include <memory>

extern sf::Font font;
extern sf::Image icon;

namespace Forms {
    
namespace Keyboard = sf::Keyboard;

using RenderWindow = sf::RenderWindow;
using Vector2f = sf::Vector2f;
using Vector2i = sf::Vector2i;
using Vector2u = sf::Vector2u;
using RectangleShape = sf::RectangleShape;
using Color = sf::Color;

using Choice = Controls::Choice;
using Label = Controls::Label;

constexpr int CURRENT_FPS = 150;

static bool turn = true;
static bool mode = false;
static int depth = 12;
//extern TcpSocket socket;
//extern TcpListener listener;

class Form {
public:
    Form(Vector2u windowSize, const std::string& title);

    virtual ~Form();

    void poll();

protected:
    mutable RenderWindow window;
    RectangleShape background;
    
    void draw();
    
    virtual void onCreate();
    
    virtual void onDraw() const {};

    virtual void onClose();

    virtual void onKeyDown(Keyboard::Key key);

    virtual void onLeftButtonPress(Vector2i position);

    virtual void onLeftButtonRelease(Vector2i position);

    virtual void onChar(char symbol);
};

class TAnalysicsForm final : public Form {
public:
    TAnalysicsForm(std::vector<AssessMoveData>& data);

protected:
    void onDraw() const override;

    //void onLeftButtonPress(Vector2i mousePosition) override;

private:
    Controls::Button exitB;
    Controls::Board board;
    Controls::AssessBar bar;
    Controls::Button flipB;
    Controls::ProgressBar pbar;
    AnalysicsController control;
    Controls::CommentSection section;
};

class TEngineForm final: public Form {
public:
    TEngineForm();
    ~TEngineForm();

protected:
    void onDraw() const override;

    void onClose() override;

    void onLeftButtonPress(sf::Vector2i position) override;

    void onLeftButtonRelease(sf::Vector2i position) override;

private:
    Controls::Button exitButton, flipButton, analysicsButton;
    Controls::Label resultLabel, timeLabel;
    Controls::Board board;
    GameController control;
    std::unique_ptr<std::thread> engineThread;
    std::mutex labelMutex;

    bool LeftPressed{ false };
    sf::Vector2i LeftPressedPosition{};
    
    void engineMove();
};

class TPvpForm final : public Form {
public:
    TPvpForm();
    ~TPvpForm();

protected:
    void onDraw() const override;

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

class StartForm final : public Form {
public:
    StartForm();
    ~StartForm();

protected:
    void onCreate() override;
    void onDraw() const override;

    void onLeftButtonPress(Vector2i mousePosition) override;
    void onChar(char symbol) override;

private:
    std::vector<Controls::Label> vLabel;
    std::vector<Controls::Choice> vChoice;
    std::vector<Controls::Input> vInput;
    Controls::Button startB, exitB;

    std::unique_ptr<TEngineForm> engineForm;
    std::unique_ptr<TPvpForm> pvpForm;
    bool pvp{ false };
    bool turn{ true };
};

} // namespace Forms