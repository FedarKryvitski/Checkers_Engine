#include "pvpform.h"
#include "form.h"

#include <mutex>
#include <chrono>

namespace Forms {

namespace {

constexpr Vector2u kFormSize{1200, 900};
constexpr int tileSize{ 100 };

} // namespace



#define EASY_DEPTH 4
#define MEDIUM_DEPTH 8
#define HARD_DEPTH 10
#define IMPOSSIBLE_DEPTH 12

namespace Style = sf::Style;
namespace Mouse = sf::Mouse;

using Event = sf::Event;
using Packet = sf::Packet;

using sf::Color;
using sf::Vector2f;
using sf::RenderWindow;
using sf::Vector2u;
using sf::Vector2i;

using namespace Controls;

PvpForm::PvpForm() : Form(kFormSize, "PvP form") {

    exitB.setSize({ 150, 60 });
    exitB.setThickness(2);
    exitB.setColor(Color::Green);
    exitB.setCaption("Exit");
    exitB.setPosition({ 900, 250 });
    exitB.setVisible(false);

    board.setField(control.field);
    board.setPosition({ 50, 50 });

    flipB.setSize({ 120, 50 });
    flipB.setColor(Color::Green);
    flipB.setCaption("Flip");
    flipB.setPosition({ 1030, 700 });
    flipB.setThickness(2);

    analysicsB.setSize({ 150, 60 });
    analysicsB.setColor(Color::Green);
    analysicsB.setCaption("Analysics");
    analysicsB.setPosition({ 900, 180 });
    analysicsB.setThickness(2);
    analysicsB.setVisible(false);


    board.setField(control.field);
    wait.setPosition({ 330, 320 });
    wait.setRadius(100);


    clock1.setPosition({ 900, 800 });
    clock2.setPosition({ 900, 50 });

    clock1.update(600);
    clock2.update(600);

    lDraw.setPosition({ 390, 350 });
    lDraw.setFontSize(100);
    lDraw.setColor(Color::White);
    lDraw.setOutlineColor(Color::Black);
    lDraw.setThickness(7);
    lDraw.setVisible(false);
    lDraw.setText("Draw");

    lWin.setPosition({ 320, 350 });
    lWin.setFontSize(100);
    lWin.setColor(Color::White);
    lWin.setOutlineColor(Color::Black);
    lWin.setThickness(7);
    lWin.setVisible(false);
    lWin.setText("You win");

    lLose.setPosition({ 310, 350 });
    lLose.setFontSize(100);
    lLose.setColor(Color::White);
    lLose.setOutlineColor(Color::Black);
    lLose.setThickness(7);
    lLose.setVisible(false);
    lLose.setText("You lose");

    drawB.setPosition({ 900, 640 });
    drawB.setSize({ 180, 50 });
    drawB.setCaption("Offer a draw");
    drawB.setThickness(2);
    drawB.setColor(Color::Green);

    resignB.setPosition({ 900, 700 });
    resignB.setSize({ 120, 50 });
    resignB.setCaption("Resign");
    resignB.setThickness(2);
    resignB.setColor(Color::Green);

    draw();
}

PvpForm::~PvpForm() {

}

void PvpForm::sendMove(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    if (CheckCoord(x1, y1) && CheckCoord(x2, y2)) {
        Packet packet;
        packet << (uint8_t)MOVEREQ << x1 << y1 << x2 << y2;
        /*socket.send(packet);*/
    }
}

void PvpForm::resign() {
    Packet packet;
    packet << (uint8_t)RESIGNREQ;
    /*socket.send(packet);*/
}

void PvpForm::offerdraw() {
    Packet packet;
    packet << (uint8_t)DRAWREQ;
    /*socket.send(packet);*/
}

void PvpForm::receive() {
    Packet packet;
    uint8_t type;
    int time;
    while (/*socket.receive(packet) == Socket::Done*/true) {
        Vector2f pos = Vector2f(Mouse::getPosition(window));
        packet >> type;
        switch (type) {
        case INIT:
            packet >> turn >> time;
            if (!turn) {
                board.flip();
            }
            wait.setVisible(false);
            clock1.update(time);
            clock2.update(time);
            clock1.start();
            clock2.start();
            if (turn) {
                clock2.pause();
            }
            else {
                clock1.pause();
            }
            break;
        case MOVEREQ:
            uint8_t x1, y1, x2, y2;
            uint8_t result;
            packet >> result >> x1 >> y1 >> x2 >> y2;
            
            if (result == DRAW) {
                lDraw.setVisible(true);
            }
            else if (result == INVALID_COORD) {
                vMoves.clear();
            }
            else if (result == LOSE) {
                auto moveResult = control.playerMove({ x1, y1, x2, y2 });
                lLose.setVisible(true);
            }
            else if (result == ONE_MORE || result == SUCCESS) {
                auto moveResult = control.playerMove({ x1, y1, x2, y2 });
            }
            else if (result == WIN) {
                auto moveResult = control.playerMove({ x1, y1, x2, y2 });
                lWin.setVisible(true);
            }

            board.setField(control.field);

            if (control.turn == turn) {
                clock1.release();
                clock2.pause();
            }
            else {
                clock2.release();
                clock1.pause();
            }
            if (result == WIN || result == LOSE || result == DRAW) {
                vMoves.clear();
                clock1.stop();
                clock2.stop();
                analysicsB.setVisible(true);
                exitB.setVisible(true);
            }
            break;
        case DRAWREQ:
            drawB.setColor(Color::Red);
            drawB.setCaption("Accept");
        }
        if (type != DRAWREQ) {
            drawB.setColor(Color::Green);
            drawB.setCaption("Offer a draw");
        }
        if (control.turn == turn && vMoves.size()) {
            int temp = vMoves[0];
            sendMove(temp / 1000, (temp / 100) % 10, (temp / 10) % 10, temp % 10);
            vMoves.erase(vMoves.begin());
        }
    }
}

//void PvpForm::onDraw() const {
//    board.draw(window);
//    exitB.draw(window);
//    flipB.draw(window);
//    analysicsB.draw(window);
//    resignB.draw(window);
//    drawB.draw(window);
//    clock1.draw(window);
//    clock2.draw(window);
//    wait.draw(window);
//    lLose.draw(window);
//    lWin.draw(window);
//    lDraw.draw(window);
//}

void PvpForm::loading() {
    while (!connected) {
        wait.setNext();
        //sleep(milliseconds(300));   // condition variable
    }
}

//void PvpForm::poll() {
//
//    Vector2f LPPos, LRPos;
//
//    std::thread loadingThread(&PvpForm::loading, this);
//
//    std::thread receiveTurn(&PvpForm::receive, this);
//
//    while (win.isOpen())
//    {
//        Vector2f pos = Vector2f(Mouse::getPosition(win));
//
//        while (const std::optional event = win.pollEvent())
//        {
//            if (event->is<Event::Closed>()) {
//                win.close();
//                open = false;
//            }
//            else if (event->is<Event::MouseButtonPressed>()) {
//                board.capture(pos.x, pos.y);
//                if (exitB.isPressed(pos)) {
//                    win.close();
//                    open = true;
//                }
//                else if (flipB.isPressed(pos)) {
//                    board.flip();
//                }
//                else if (analysicsB.isPressed(pos)) {
//                    AnalysicsForm form1(control.gameMoves);
//                    form1.poll();
//                }
//                else if (resignB.isPressed(pos)) {
//                    resign();
//                }
//                else if (drawB.isPressed(pos)) {
//                    offerdraw();
//                }
//                else {
//                    if (Mouse::isButtonPressed(Mouse::Button::Left)) {
//                        LP = true;
//                        LPPos = pos;
//                    }
//                }
//            }
//            else if (event->is<Event::MouseButtonReleased>()) {
//                board.uncatch();
//                if (LP) {
//                    Vector2f pos = Vector2f(Mouse::getPosition(win));
//                    if (Mouse::isButtonPressed(Mouse::Button::Left)) {
//                        LR = true;
//                        LRPos = pos;
//                    }
//                }
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
//                control.getCurr();
//                board.setField(control.field);
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
//                vMoves.clear();
//                control.getPrev();
//                board.setField(control.field);
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
//                control.getNext();
//                board.setField(control.field);
//            }
//            if (LP && LR && control.curr == control.head) {
//                LP = false;
//                LR = false;
//
//                uint8_t coord[4];
//                board.getCoord(LPPos, LRPos, coord);
//                sendMove(coord[0], coord[1], coord[2], coord[3]);
//            }
//        }
//        draw();
//    }
//
//    clock1.stop();
//    clock2.stop();
//    //receiveTurn.terminate();
//    //loadingThread.terminate();
//    /*socket.disconnect();
//    listener.close();*/
//}

void PvpForm::onDraw() {
    board.draw(window);
    exitB.draw(window);
    flipB.draw(window);
    analysicsB.draw(window);
    drawB.draw(window);
    resignB.draw(window);
    wait.draw(window);
    clock1.draw(window);
    clock2.draw(window);
    lDraw.draw(window);
    lLose.draw(window);
    lWin.draw(window);
}

} // namespace Forms;