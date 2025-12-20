#pragma once
#include <SFML/Network.hpp>
#include "controls.h"
#include "GameControllers.h"
#include <array>
#include <mutex>
#include <vector>
#include <memory>

extern Font font;
extern Image icon;

#define CURRENT_FPS 150

static bool turn = true;
static bool mode = false;
static int depth = 12;
//extern TcpSocket socket;
//extern TcpListener listener;

using std::vector, std::array, std::thread, std::mutex, std::unique_ptr, std::optional, std::string;

class TForm {
public:
    TForm(Vector2u windowSize, const string& title);

    virtual ~TForm();

    void poll();

protected:
    mutable RenderWindow window;
    RectangleShape background;
    
    void draw();
    
    virtual void onCreate();
    
    virtual void onDraw() const;

    virtual void onClose();

    virtual void onKeyDown(Keyboard::Key key);

    virtual void onLeftButtonPress(Vector2i position);

    virtual void onLeftButtonRelease(Vector2i position);

    virtual void onChar(char symbol);
};

class TAnalysicsForm final : public TForm {
public:
    TAnalysicsForm(vector<AssessMoveData>& data);

protected:
    void onDraw() const override;

    //void onLeftButtonPress(Vector2i mousePosition) override;

private:
    Button exitB;
    Board board;
    AssessBar bar;
    Button flipB;
    ProgressBar pbar;
    AnalysicsController control;
    CommentSection section;
};

class TEngineForm final: public TForm {
public:
    TEngineForm();
    ~TEngineForm();

protected:
    void onDraw() const override;

    void onClose() override;

    void onLeftButtonPress(Vector2i position) override;

    void onLeftButtonRelease(Vector2i position) override;

private:

    Button exitButton, flipButton, analysicsButton;
    Label resultLabel, timeLabel;
    Board board;
    GameController control;
    unique_ptr<thread> engineThread;
    mutex labelMutex;

    bool LeftPressed{ false };
    Vector2i LeftPressedPosition{};
    
    void engineMove();
};

class TPvpForm final : public TForm {
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

    vector<int> vMoves;

    CheckersClock clock1, clock2;
    Button exitB, flipB, analysicsB, drawB, resignB;
    Board board;
    TWait wait;
    GameController control;
    Label lDraw, lLose, lWin;

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

class TStartForm final : public TForm {
public:
    TStartForm();
    ~TStartForm();

protected:
    void onCreate() override;
    void onDraw() const override;

    void onLeftButtonPress(Vector2i mousePosition) override;
    void onChar(char symbol) override;

private:
    vector<Label> vLabel;
    vector<Choice> vChoice;
    vector<Input> vInput;
    Button startB, exitB;

    unique_ptr<TEngineForm> engineForm;
    unique_ptr<TPvpForm> pvpForm;
    bool pvp{ false };
    bool turn{ true };
};