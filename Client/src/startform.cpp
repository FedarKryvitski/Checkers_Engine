#include "forms.h"

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

void StartForm::onDraw() const {
    std::ranges::for_each(vLabel, [this](const auto& label) {
        label.draw(window);
    });
    std::ranges::for_each(vChoice, [this](const auto& choice) {
        choice.draw(window);
    });
    std::ranges::for_each(vInput, [this](const auto& input) {
        input.draw(window);
    });
    startB.draw(window);
    exitB.draw(window);
}

StartForm::StartForm() : Form(START_FORM_SIZE, "Start form") {

    background.setSize(Vector2f(window.getSize()));
    background.setFillColor(Color::White);

    vLabel.reserve(13);

    vLabel.push_back(Label("Choose mode:", Vector2f(320, 30)));
    vLabel.push_back(Label("vs Player", Vector2f(245, 90)));
    vLabel.push_back(Label("vs Engine", Vector2f(245, 140)));

    vLabel.push_back(Label("Choose your color:", Vector2f(300, 220), false));
    vLabel.push_back(Label("White", Vector2f(245, 280), false));
    vLabel.push_back(Label("Black", Vector2f(245, 330), false));
    vLabel.push_back(Label("Choose the difficulty:", Vector2f(289, 410), false));
    vLabel.push_back(Label("Easy", Vector2f(245, 470), false));
    vLabel.push_back(Label("Medium", Vector2f(245, 520), false));
    vLabel.push_back(Label("Hard", Vector2f(245, 570), false));
    vLabel.push_back(Label("Impossible", Vector2f(245, 620), false));

    vLabel.push_back(Label("Enter server IP", Vector2f(300, 425)));
    vLabel.push_back(Label("Enter server port", Vector2f(300, 525)));

    turn = true;
    depth = IMPOSSIBLE_DEPTH;

    Controls::Input tempI;
    tempI.setThickness(2);
    tempI.setVisible(true);
    tempI.setSize({ 300, 35 });

    tempI.setPosition({ 225, 475 });
    tempI.setLimit(20);
    tempI.setLimit(15);
    vInput.push_back(tempI);
    tempI.setPosition({ 225, 575 });
    tempI.setLimit(5);
    vInput.push_back(tempI);

    vChoice.reserve(8);

    vChoice.push_back(Choice({ 445, 97 }, [&]() {
        vChoice[0].setStatus(true);
        vChoice[1].setStatus(false);
        for (int i = 2; i < 8; ++i) {
            vChoice[i].setVisible(false);
        }
        for (int i = 3; i < 11; ++i) {
            vLabel[i].setVisible(false);
        }
        vInput[0].setVisible(true);
        vInput[1].setVisible(true);
        vLabel[11].setVisible(true);
        vLabel[12].setVisible(true);
        pvp = true;
        }, true));

    vChoice.push_back(Choice({ 445, 147 }, [&]() {
        vChoice[1].setStatus(true);
        vChoice[0].setStatus(false);
        for (int i = 2; i < 8; ++i) {
            vChoice[i].setVisible(true);
        }
        for (int i = 3; i < 11; ++i) {
            vLabel[i].setVisible(true);
        }
        vInput[0].setVisible(false);
        vInput[1].setVisible(false);
        vLabel[11].setVisible(false);
        vLabel[12].setVisible(false);
        pvp = false;
        }, false));

    vChoice.push_back(Choice({ 445, 285 }, [&]() {
        turn = true;
        vChoice[2].setStatus(true);
        vChoice[3].setStatus(false);
        }, true, false));

    vChoice.push_back(Choice({ 445, 335 }, [&]() {
        turn = false;
        vChoice[2].setStatus(false);
        vChoice[3].setStatus(true);
        }, false, false));

    auto setDifficultyLevel = [this](const int index) {
        auto start = std::next(vChoice.begin(), 4);
        auto end = std::next(vChoice.begin(), 8);
        std::for_each(start, end, [i = 0, index](auto& choice) mutable {
            bool status = i == index;
            choice.setStatus(status);
            i++;
            });
        };

    vChoice.push_back(Choice({ 445, 475 }, [=]() {
        setDifficultyLevel(0);
        depth = EASY_DEPTH;
        }, false, false));

    vChoice.push_back(Choice({ 445, 525 }, [=]() {
        setDifficultyLevel(1);
        depth = MEDIUM_DEPTH;
        }, false, false));

    vChoice.push_back(Choice({ 445, 575 }, [=]() {
        setDifficultyLevel(2);
        depth = HARD_DEPTH;
        }, false, false));

    vChoice.push_back(Choice({ 445, 625 }, [=]() {
        setDifficultyLevel(3);
        depth = IMPOSSIBLE_DEPTH;
        }, true, false));

    startB.setCaption("Start");
    startB.setPosition({ 240, 720 });
    startB.setOnPress([&]() {
        window.setVisible(false);
        //engineForm.reset(new EngineForm());

        //engineForm->poll();
        window.setVisible(true);
        });

    exitB.setCaption("Exit");
    exitB.setPosition({ 400, 720 });
    // exitB.setOnPress([&]() {
    //     window.close();
    // });

    draw();
}

StartForm::~StartForm() { }

void StartForm::onCreate() { }

void StartForm::onLeftButtonPress(Vector2i mousePosition) {
    if (startB.isPressed(mousePosition)) {
        startB.onPress();
    }
    else if (exitB.isPressed(mousePosition)) {
        exitB.onPress();
    }
    else if (auto choiceIt = std::ranges::find_if(vChoice, [mousePosition](const auto& choice) {
        return choice.isPressed(mousePosition);
        });
        choiceIt != vChoice.end()) {
        choiceIt->onPress();
    }
    else if (vInput[0].isPressed(mousePosition)) {
        vInput[0].onPress();
        vInput[1].onRelease();
    }
    else if (vInput[1].isPressed(mousePosition)) {
        vInput[1].onPress();
        vInput[0].onRelease();
    }
}

void StartForm::onChar(char symbol) {
    vInput[0].onKeyPress(symbol);
    vInput[1].onKeyPress(symbol);
}

} // namespace Forms;