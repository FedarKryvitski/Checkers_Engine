#include "analysicsform.h"

// #include <SFML/Network.hpp>
#include <chrono>
#include <mutex>

namespace Forms {

#define ANALYSICS_FORM_SIZE Vector2u{1300, 900}

constexpr int tileSize = 100;

using Controls::Color;
using Controls::RenderWindow;
using Controls::Vector2f;
using Controls::Vector2i;
using Controls::Vector2u;

using namespace Controls;

#define EASY_DEPTH 4
#define MEDIUM_DEPTH 8
#define HARD_DEPTH 10
#define IMPOSSIBLE_DEPTH 12

namespace Style = sf::Style;
namespace Mouse = sf::Mouse;

using Event = sf::Event;
using Packet = sf::Packet;

void AnalysicsForm::onDraw() {
	// bar.draw(window);
	board.draw(window);
	exitB.draw(window);
	flipB.draw(window);
	// section.draw(window);
	// pbar.draw(window);
}

AnalysicsForm::AnalysicsForm(std::vector<StateData> &data) : Form(ANALYSICS_FORM_SIZE, "Analysics form") {

	control.setMoves(data);

	board.setField(control.gameMoves[0].field);
	board.setPosition({100, 50});

	// bar.setSize({ 46, 8 * tileSize - 4 });
	// bar.setPosition({ 100 + 8 * tileSize + 2, 52 });
	// bar.setValue(0);
	// bar.setThickness(2);

	// pbar.setSize({ 600, 50 });
	// pbar.setPosition({ 200, 400 });
	// pbar.setThickness(2);
	// pbar.setValue(0);

	exitB.setSize({100, 50});
	exitB.setThickness(2);
	exitB.setColor(Color::Green);
	exitB.setCaption("Exit");
	exitB.setPosition({1130, 798});

	flipB.setSize({100, 50});
	flipB.setColor(Color::Green);
	flipB.setCaption("Flip");
	flipB.setPosition({1010, 798});
	flipB.setThickness(2);

	// section.setPosition({ 1000, 50 });

	// bar.setValue(0.0);
	// draw();

	for (int i = 1, size = static_cast<int>(control.gameMoves.size()); i < size; i++) {
		control.evaluate(i, 10);
		// const float progress = static_cast<float>(i) / size;
		// pbar.setValue(progress);
		// draw();
	}
	section.setValues(control.gameMoves);
	draw();
}

/*
void AnalysicsForm::poll() {

	Vector2f LPPos, LRPos;

	while (win.isOpen())
	{
		while (const std::optional event = win.pollEvent())
		{
			if (event->is<Event::Closed>()) {
				onClose();
				win.close();
				open = false;
			}
			else if (event->is<Event::MouseButtonPressed>()) {
				onLeftButtonPress();
				Vector2f pos = Vector2f(Mouse::getPosition(win));
				if (exitB.isPressed(pos)) {
					win.close();
					open = true;
				}
				else if (flipB.isPressed(pos)) {
					board.flip();
					bar.flip();
				}
			}
			else if (Keyboard::isKeyPressed(Keyboard::Key::Up)) {
				onKeyDown();
				control.getCurr();
				board.setField(control.field);
				board.setComment(control.comment, control.x1, control.y1, control.x2, control.y2);
				bar.setValue(control.assess);
			}
			else if (Keyboard::isKeyPressed(Keyboard::Key::Left)) {
				control.getPrev();
				board.setField(control.field);
				board.setComment(control.comment, control.x1, control.y1, control.x2, control.y2);
				bar.setValue(control.assess);
			}
			else if (Keyboard::isKeyPressed(Keyboard::Key::Right)) {
				control.getNext();
				board.setField(control.field);
				board.setComment(control.comment, control.x1, control.y1, control.x2, control.y2);
				bar.setValue(control.assess);
			}
		}
		draw();
	}
}
*/

} // namespace Forms