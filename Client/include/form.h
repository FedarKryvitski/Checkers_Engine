#pragma once
#include "controls.h"
#include "GameControllers.h"

#include <SFML/Graphics.hpp>

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
    void draw();
    
    virtual void onCreate();
    virtual void onDraw() {};
    virtual void onClose();
    virtual void onKeyDown(Keyboard::Key key);
    virtual void onLeftButtonPress(Vector2i position);
    virtual void onLeftButtonRelease(Vector2i position);
    virtual void onChar(char symbol);

protected:
    mutable RenderWindow window;
    RectangleShape background;
};

} // namespace Forms