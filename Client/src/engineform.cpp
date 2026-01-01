#include "engineform.h"
#include "form.h"
#include "analysicsform.h"

namespace Forms {

using namespace Controls;

void EngineForm::onDraw() {
    board.draw(window);
    exitButton.draw(window);
    flipButton.draw(window);
    analysicsButton.draw(window);
    resultLabel.draw(window);
    timeLabel.draw(window);
}

void EngineForm::onClose() {
    //engineThread->join();
    Form::onClose();
}

void EngineForm::onLeftButtonPress(Vector2i mousePosition) {
    if (exitButton.isPressed(mousePosition)) {
        onClose();
    }
    else if (flipButton.isPressed(mousePosition)) {
        board.flip();
    }
    else if (analysicsButton.isPressed(mousePosition)) {    
        auto analysicsForm = std::make_unique<AnalysicsForm>(control.gameMoves);
        analysicsForm->poll();
    }
    else if (board.isPressed(mousePosition)) {
        LeftPressedPosition = mousePosition;
        board.catchPiece(mousePosition);
        LeftPressed = true;
    }
}

void EngineForm::onLeftButtonRelease(Vector2i mousePosition) {
    
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

EngineForm::EngineForm() : 
    Form(ENGINE_FORM_SIZE, "Engine Form") {

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
    //engineThread = new Thread(&EngineForm::engineMove, this);

    if (!turn) {
        board.flip();
        //engineThread->launch();
    }
    
}

EngineForm::~EngineForm() { }

void EngineForm::engineMove() {
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

//void EngineForm::poll() {
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
//                    std::unique_ptr<AnalysicsForm> analysicsForm{ new AnalysicsForm(*analysicsWindow.get(), control.gameMoves)};
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

} // namespace Forms