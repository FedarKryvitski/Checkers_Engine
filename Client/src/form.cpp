#include "form.h"

#include <mutex>
#include <chrono>

namespace Forms {

#define kFormSize Vector2u{1200, 900}
#define START_FORM_SIZE Vector2u{760, 850}
#define ENGINE_FORM_SIZE Vector2u{1200, 900}
#define ANALYSICS_FORM_SIZE Vector2u{1300, 900}

constexpr int tileSize = 100;

#define EASY_DEPTH 4
#define MEDIUM_DEPTH 8
#define HARD_DEPTH 10
#define IMPOSSIBLE_DEPTH 12

namespace Style = sf::Style;
namespace Mouse = sf::Mouse;

using Event = sf::Event;
using Packet = sf::Packet;

Form::Form(Vector2u windowSize, const std::string& title)
    : window(sf::VideoMode(windowSize), title, Style::Close) {

    window.setFramerateLimit(CURRENT_FPS);
    window.setVerticalSyncEnabled(true);

    background.setSize(Vector2f(windowSize));
    background.setFillColor(Color::White);
}

Form::~Form() {}

void Form::onCreate() { }

void Form::poll() {

    onCreate();

    while (window.isOpen())
    {
        while (const std::optional<Event> event = window.pollEvent()) {
            auto mousePos = Mouse::getPosition(window);

            if (event->is<Event::Closed>()) {
                onClose();
            }
            else if (event->is<Event::MouseButtonPressed>()) {
                onLeftButtonPress(mousePos);
            }
            else if (event->is<Event::MouseButtonReleased>()) {
                onLeftButtonRelease(mousePos);
            }
            else if (event->is<Event::KeyPressed>()) {
                const auto& keyEvent = event->getIf<Event::KeyPressed>();
                onKeyDown(keyEvent->code);
            }
            else if (event->is<Event::KeyReleased>()) {

            }
            else if (event->is<Event::TextEntered>()) {
                const auto& textEvent = event->getIf<Event::TextEntered>();
                char symbol = static_cast<char>(textEvent->unicode);
                onChar(symbol);
            }
        }
        draw();
    }
}

void Form::draw() {
    window.clear();
    window.draw(background);
    onDraw();
    window.display();
}

void Form::onClose() {
    window.close();
}

void Form::onKeyDown(Keyboard::Key key) {}

void Form::onLeftButtonPress(Vector2i position) {}

void Form::onLeftButtonRelease(Vector2i position) {}

void Form::onChar(char symbol) {}

} // namespace Forms;