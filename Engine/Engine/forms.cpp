#include "forms.h"
#include <mutex>
#include <chrono>

#define PVP_FORM_SIZE Vector2u{1200, 900}
#define START_FORM_SIZE Vector2u{760, 850}
#define ENGINE_FORM_SIZE Vector2u{1200, 900}
#define ANALYSICS_FORM_SIZE Vector2u{1300, 900}

#define EASY_DEPTH 4
#define MEDIUM_DEPTH 8
#define HARD_DEPTH 10
#define IMPOSSIBLE_DEPTH 12

using std::pair, std::lock_guard, std::mutex;

TForm::TForm(Vector2u windowSize, const string& title)
    : window(VideoMode(windowSize), title, Style::Close) {

    window.setFramerateLimit(CURRENT_FPS);
    window.setVerticalSyncEnabled(true);

    background.setSize(Vector2f(windowSize));
    background.setFillColor(Color::White);
}

TForm::~TForm() {}

void TForm::onCreate() { }

void TForm::poll() {

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

void TForm::draw() {
    window.clear();
    window.draw(background);
    onDraw();
    window.display();
}

void TForm::onDraw() const {}

void TForm::onClose() {
    window.close();
}

void TForm::onKeyDown(Keyboard::Key key) {}

void TForm::onLeftButtonPress(Vector2i position) {}

void TForm::onLeftButtonRelease(Vector2i position) {}

void TForm::onChar(char symbol) {}


void TAnalysicsForm::onDraw() const {
    bar.draw(window);
    board.draw(window);
    exitB.draw(window);
    flipB.draw(window);
    section.draw(window);
}

TAnalysicsForm::TAnalysicsForm(std::vector<AssessMoveData>& data) :
    TForm(ANALYSICS_FORM_SIZE, "Analysics form") {

    control.setMoves(data);

    board.setField(control.gameMoves[0].field);
    board.setPosition({ 100, 50 });

    bar.setSize({ 46, 8 * tileSize - 4 });
    bar.setPosition({ 100 + 8 * tileSize + 2, 52 });
    bar.setValue(0);
    bar.setThickness(2);

    pbar.setSize({ 600, 50 });
    pbar.setPosition({ 200, 400 });
    pbar.setThickness(2);
    pbar.setValue(0);

    exitB.setSize({ 100, 50 });
    exitB.setThickness(2);
    exitB.setColor(Color::Green);
    exitB.setCaption("Exit");
    exitB.setPosition({ 1130, 798 });

    flipB.setSize({ 100, 50 });
    flipB.setColor(Color::Green);
    flipB.setCaption("Flip");
    flipB.setPosition({ 1010, 798 });
    flipB.setThickness(2);

    section.setPosition({ 1000, 50 });

    bar.setValue(0.0);
    draw();

    for (int i = 1, size = static_cast<int>(control.gameMoves.size()); i < size; i++) {
        control.evaluate(i, 10);
        const float progress = static_cast<float>(i) / size;
        pbar.setValue(progress);
        draw();
    }
    section.setValues(control.gameMoves);
    draw();
}

//void TAnalysicsForm::poll() {
//
//    Vector2f LPPos, LRPos;
//
//    while (win.isOpen())
//    {
//        while (const std::optional event = win.pollEvent())
//        {
//            if (event->is<Event::Closed>()) {
//                onClose();
//                /*win.close();
//                open = false;*/
//            }
//            else if (event->is<Event::MouseButtonPressed>()) {
//                onLeftButtonPress();
//                /*Vector2f pos = Vector2f(Mouse::getPosition(win));
//                if (exitB.isPressed(pos)) {
//                    win.close();
//                    open = true;
//                }
//                else if (flipB.isPressed(pos)) {
//                    board.flip();
//                    bar.flip();
//                }*/
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
//                onKeyDown();
//                /*control.getCurr();
//                board.setField(control.field);
//                board.setComment(control.comment, control.x1, control.y1, control.x2, control.y2);
//                bar.setValue(control.assess);*/
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
//                /*control.getPrev();
//                board.setField(control.field);
//                board.setComment(control.comment, control.x1, control.y1, control.x2, control.y2);
//                bar.setValue(control.assess);*/
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
//                /*control.getNext();
//                board.setField(control.field);
//                board.setComment(control.comment, control.x1, control.y1, control.x2, control.y2);
//                bar.setValue(control.assess);*/
//            }
//        }
//        draw();
//    }
//}

void TStartForm::onDraw() const {
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

TStartForm::TStartForm() : TForm(START_FORM_SIZE, "Start form") {

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

    Input tempI;
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
        engineForm.reset(new TEngineForm());

        engineForm->poll();
        window.setVisible(true);
        });

    exitB.setCaption("Exit");
    exitB.setPosition({ 400, 720 });
    exitB.setOnPress([&]() {
        window.close();
        });

    draw();
}

TStartForm::~TStartForm() { }

void TStartForm::onCreate() { }

void TStartForm::onLeftButtonPress(Vector2i mousePosition) {
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

void TStartForm::onChar(char symbol) {
    vInput[0].onKeyPress(symbol);
    vInput[1].onKeyPress(symbol);
}



void TEngineForm::onDraw() const {
    board.draw(window);
    exitButton.draw(window);
    flipButton.draw(window);
    analysicsButton.draw(window);
    resultLabel.draw(window);
    timeLabel.draw(window);
}

void TEngineForm::onClose() {
    //engineThread->join();
    TForm::onClose();
}

void TEngineForm::onLeftButtonPress(Vector2i mousePosition) {
    if (exitButton.isPressed(mousePosition)) {
        onClose();
    }
    else if (flipButton.isPressed(mousePosition)) {
        board.flip();
    }
    else if (analysicsButton.isPressed(mousePosition)) {    
        unique_ptr<TAnalysicsForm> analysicsForm{ new TAnalysicsForm(control.gameMoves)};
        analysicsForm->poll();
    }
    else if (board.isPressed(mousePosition)) {
        LeftPressedPosition = mousePosition;
        board.catchPiece(mousePosition);
        LeftPressed = true;
    }
}

void TEngineForm::onLeftButtonRelease(Vector2i mousePosition) {
    
    if (LeftPressed) {
        LeftPressed = false;
        board.releasePiece(mousePosition);
        auto moveCoordinates = board.getMoveCoordinates();
        auto result = control.playerMove(moveCoordinates);
        if (result != INVALID_COORD) {
            board.setField(control.field);
            if (result == SUCCESS) {
                //engineThread->launch();
            }
            else if (result == WIN) {
                resultLabel.setText("You win");
                resultLabel.setVisible(true);
            }
        }
    }
}

TEngineForm::TEngineForm() : 
    TForm(ENGINE_FORM_SIZE, "Engine Form") {

    exitButton.setSize({ 100, 50 });
    exitButton.setCaption("Exit");
    exitButton.setPosition({ 1050, 798 });

    board.setField(control.field);
    board.setPosition({ 100, 50 });

    flipButton.setSize({ 100, 50 });
    flipButton.setCaption("Flip");
    flipButton.setPosition({ 930, 798 });

    analysicsButton.setSize({ 200, 60 });
    analysicsButton.setCaption("Analysics");
    analysicsButton.setPosition({ 950, 100 });

    resultLabel.setPosition({ 310, 350 });
    resultLabel.setFontSize(100);
    resultLabel.setColor(Color::White);
    resultLabel.setOutlineColor(Color::Black);
    resultLabel.setThickness(7);
    resultLabel.setVisible(false);

    timeLabel.setPosition({ 950, 500 });
    timeLabel.setText("Time: ");

    board.setField(control.field);
    //engineThread = new Thread(&TEngineForm::engineMove, this);

    if (!turn) {
        board.flip();
        //engineThread->launch();
    }
    
}

TEngineForm::~TEngineForm() { }

void TEngineForm::engineMove() {
    auto start = std::chrono::high_resolution_clock::now();

    MOVE_RESULT result;
    do {
        result = control.engineMove(depth);
        board.setField(control.field);
    } while (result == ONE_MORE);
    if (result == WIN) {
        resultLabel.setText("You lose");
        resultLabel.setVisible(true);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::string s = "Time: " + std::to_string(duration) + " ms\n";
    timeLabel.setText(s);
}

//void TEngineForm::poll() {
//
//    Vector2f LPPos, LRPos;
//    Vector2f pos;
//    draw();
//
//    while (win.isOpen())
//    {
//        pos = Vector2f(Mouse::getPosition(win));
//        while (const std::optional event = win.pollEvent()) {
//            else if (event->is<Event::MouseButtonPressed>()) {
//                board.capture(pos.x, pos.y);
//                if (exitB.isPressed(pos)) {
//                    win.close();
//                    //engineThread->wait();
//                    open = true;
//                }
//                else if (flipB.isPressed(pos)) {
//                    board.flip();
//                }
//                else if (analysicsB.isPressed(pos)) {
//                    std::unique_ptr<RenderWindow> analysicsWindow{new RenderWindow(VideoMode({ 1300, 900 }), "VOBLA", Style::Close)};
//                    analysicsWindow->setFramerateLimit(150);
//                    analysicsWindow->setVerticalSyncEnabled(true);
//
//                    std::unique_ptr<TAnalysicsForm> analysicsForm{ new TAnalysicsForm(*analysicsWindow.get(), control.gameMoves)};
//                    analysicsForm->poll();
//                }
//                else if (Mouse::isButtonPressed(Mouse::Button::Left)) {
//                    LP = true;
//                    LPPos = pos;
//                }
//            }
//            else if (event->is<Event::MouseButtonReleased>()) {
//                board.uncatch();
//                if (LP && Mouse::isButtonPressed(Mouse::Button::Left)) {
//                    LR = true;
//                    LRPos = pos;
//                }
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
//                control.getCurr();
//                board.setField(control.field);
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
//                control.getPrev();
//                board.setField(control.field);
//            }
//            else if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
//                control.getNext();
//                board.setField(control.field);
//            }
//            if (LP && LR) {
//                LP = false;
//                LR = false;
//
//                if (turn == control.turn) {
//                    uint8_t coord[4];
//                    board.getCoord(LPPos, LRPos, coord);
//                    MOVE_RESULT result = control.PlayerMove(coord[0], coord[1], coord[2], coord[3]);
//                    if (result != INVALID_COORD) {
//                        board.setField(control.field);
//                        if (result == SUCCESS) {
//                            //engineThread->launch();
//                        }
//                        else if (result == WIN) {
//                            resultLabel.setText("You win");
//                            resultLabel.setVisible(true);
//                        }
//                    }
//                }
//            }
//        }
//        board.setTilePos({pos.x, pos.y});
//        draw(pos.x, pos.y);
//    }
//    //engineThread->terminate();
//}



TPvpForm::TPvpForm() : TForm(PVP_FORM_SIZE, "PvP form") {

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

TPvpForm::~TPvpForm() {

}

void TPvpForm::sendMove(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    if (CheckCoord(x1, y1) && CheckCoord(x2, y2)) {
        Packet packet;
        packet << (uint8_t)MOVEREQ << x1 << y1 << x2 << y2;
        /*socket.send(packet);*/
    }
}

void TPvpForm::resign() {
    Packet packet;
    packet << (uint8_t)RESIGNREQ;
    /*socket.send(packet);*/
}

void TPvpForm::offerdraw() {
    Packet packet;
    packet << (uint8_t)DRAWREQ;
    /*socket.send(packet);*/
}

void TPvpForm::receive() {
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
                control.playerMove({ x1, y1, x2, y2 });
                lLose.setVisible(true);
            }
            else if (result == ONE_MORE || result == SUCCESS) {
                control.playerMove({ x1, y1, x2, y2 });
            }
            else if (result == WIN) {
                control.playerMove({ x1, y1, x2, y2 });
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

void TPvpForm::onDraw() const {
    board.draw(window);
    exitB.draw(window);
    flipB.draw(window);
    analysicsB.draw(window);
    resignB.draw(window);
    drawB.draw(window);
    clock1.draw(window);
    clock2.draw(window);
    wait.draw(window);
    lLose.draw(window);
    lWin.draw(window);
    lDraw.draw(window);
}

void TPvpForm::loading() {
    while (!connected) {
        wait.setNext();
        sleep(milliseconds(300));   // condition variable
    }
}

//void TPvpForm::poll() {
//
//    Vector2f LPPos, LRPos;
//
//    std::thread loadingThread(&TPvpForm::loading, this);
//
//    std::thread receiveTurn(&TPvpForm::receive, this);
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
//                    TAnalysicsForm form1(control.gameMoves);
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