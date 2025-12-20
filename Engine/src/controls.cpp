#include "controls.h"
#include <algorithm>

using std::string, std::to_string, std::round, std::swap;


Board::Board() {

    static const array<string, 5> texturesPaths = {
        "Image/best.png",
        "Image/blunder.png",
        "Image/forced.png",
        "Image/good.png",
        "Image/inaccurasy.png"
    };

    std::ranges::for_each(moveStatusTextures, [&, i=0](auto& texture) mutable {
        auto status = moveStatusTextures[0].loadFromFile(texturesPaths[i]);
        i++;
    });

    size = {8 * tileSize, 8 * tileSize};
}

void Board::setPosition(Vector2f position) {
    Object::setPosition(position);
}

void Board::setField(TField& toSet) {
    memcpy(field, toSet, 64);
    if (flipped) {
        flip();
        flipped = true;
    }
    isCaptured = false;
}

tuple<uint8_t, uint8_t, uint8_t, uint8_t> Board::getMoveCoordinates() {
    uint8_t startX = static_cast<uint8_t>(moveStart.x);
    uint8_t startY = static_cast<uint8_t>(moveStart.y);
    uint8_t endX = static_cast<uint8_t>(moveEnd.x);
    uint8_t endY = static_cast<uint8_t>(moveEnd.y);
    return {startX, startY, endX, endY};
}

void Board::flip() {
    std::reverse(field, field + 8);
    std::for_each(field, field + 8, [](auto row[]) {
        std::reverse(row, row + 8);
    });
    flipped = !flipped;
}

void Board::draw(RenderWindow& window) const {

    float posx = 0;
    float posy = 0;    //Make class fields

    constexpr float checkerRadius = tileSize / 2.f - 10;
    constexpr float innerRadius = checkerRadius / 2.f;
    
    RectangleShape cell({ tileSize, tileSize });
    CircleShape checkerCircle(checkerRadius);
    CircleShape innerCircle(innerRadius);

    constexpr Color whiteColor(230, 230, 230);
    constexpr Color blackColor(30, 30, 30);
    constexpr Color boardFirstColor(233, 237, 204);
    constexpr Color boardSecondColor(119, 153, 84);

    const auto& [x, y] = position;

    for (int row = 0; row < 8; ++row) {
        for (int column = 0; column < 8; ++column) {

            const auto currentPiece{ field[row][column] };

            if ((row + column) % 2 == 1) {
                cell.setFillColor(boardFirstColor);
            }
            else {
                cell.setFillColor(boardSecondColor);
            }

            const float cellX = x + row * tileSize;
            const float cellY = y + (7 - column) * tileSize;

            cell.setPosition({cellX, cellY});
            window.draw(cell);

            if (currentPiece != 0) {

                checkerCircle.setPosition({ cellX + 10, cellY + 10 });

                if (currentPiece == 1 || currentPiece == 3) {
                    checkerCircle.setFillColor(whiteColor);
                }
                else {
                    checkerCircle.setFillColor(blackColor);
                }
                if (!isCaptured || row != cx || column != cy) {
                    window.draw(checkerCircle);
                    
                    if (currentPiece == 3 || currentPiece == 4) {
                        const float innerCircleX = x + row * tileSize + innerRadius;
                        const float innerCircleY = y + (7 - column) * tileSize + innerRadius;
                        innerCircle.setPosition({ innerCircleX, innerCircleY });
                        if (currentPiece == 3) {
                            innerCircle.setFillColor(blackColor);
                        }
                        else {
                            innerCircle.setFillColor(whiteColor);
                        }
                        window.draw(innerCircle);
                    }
                }
            }
        }
    }
    if (x1 || y1 || x2 || y2) {
        auto& texture = moveStatusTextures[comment];
        Sprite sprite(texture);

        float spriteX, spriteY;
        if (!flipped) {
            spriteX = x + (x2 + 1) * tileSize - 25;
            spriteY = y + (7 - y2) * tileSize - 25;
        }
        else {
            spriteX = x + (8 - x2) * tileSize - 25;
            spriteY = y + y2 * tileSize - 25;
        }
        sprite.setPosition({ spriteX, spriteY });

        const float scaleFactor = 50.0f / sprite.getLocalBounds().size.y;
        sprite.setScale({ scaleFactor, scaleFactor });
        window.draw(sprite);
    }
    if (isCaptured) {

        auto capturedPiece = field[cx][cy];
        if (capturedPiece != 0) {
            if (capturedPiece == 1 || capturedPiece == 3) {
                checkerCircle.setFillColor(whiteColor);
            }
            else {
                checkerCircle.setFillColor(blackColor);
            }
            checkerCircle.setPosition({ posx - checkerRadius, posy - checkerRadius });

            window.draw(checkerCircle);

            if (capturedPiece == 3 || capturedPiece == 4) {
                const float innerCircleX = posx - innerRadius;
                const float innerCircleY = posy - innerRadius;
                innerCircle.setPosition({ innerCircleX, innerCircleY });
                if (capturedPiece == 3) {
                    innerCircle.setFillColor(blackColor);
                }
                else {
                    innerCircle.setFillColor(whiteColor);
                }
                window.draw(innerCircle);
            }
        }
    }
}

Vector2i Board::getPieceCoordinates(Vector2i mousePosition) {
    const auto& [x, y] = position;
    const auto& [mouseX, mouseY] = mousePosition;

    int startX = static_cast<uint8_t>((mouseX - x) / tileSize);
    int startY = static_cast<uint8_t>(8 - (mouseY - y) / tileSize);
    return { startX, startY };
}

void Board::catchPiece(Vector2i mousePosition) {
    moveStart = getPieceCoordinates(mousePosition);
    isCaptured = true;
}

void Board::releasePiece(Vector2i mousePosition) {
    moveEnd = getPieceCoordinates(mousePosition);
    isCaptured = false;
}

void Board::setComment(MOVE_STATUS _comment, uint8_t _x1, uint8_t _y1, uint8_t _x2, uint8_t _y2) {
    x1 = _x1;
    y1 = _y1;
    x2 = _x2;
    y2 = _y2;
    comment = _comment;
}


Label::Label() : text(font) {
    visible = true;
    text.setString("");
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);
    text.setPosition({ 0, 0 });
}

Label::Label(const string& _caption, const Vector2f _position, bool _visible) : text(font) {
    Object::setPosition(_position);
    visible = _visible;

    text.setString(_caption);
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);
    text.setPosition(_position);
}

void Label::setText(const string& _text) {
    text.setString(_text);
}

void Label::setPosition(Vector2f position) {
    Object::setPosition(position);
    text.setPosition(position);
}

void Label::draw(RenderWindow& window) const {
    if (visible) {
        window.draw(text);
    }
}

void Label::setTextThickness(float thick) {
    text.setOutlineThickness(thick);
}

void Label::setFontSize(int fontSize) {
    text.setCharacterSize(fontSize);
}

void Label::setTextColor(Color color) {
    text.setFillColor(color);
}

void Label::setOutlineColor(Color color) {
    text.setOutlineColor(color);
}


Object::Object() {
    background.setFillColor(Color::White);
    background.setPosition(Vector2f( 0, 0 ));
    background.setSize(Vector2f(0, 0));
    background.setOutlineColor(Color(30, 30, 30));
    visible = true;
}

void Object::setPosition(Vector2f _position) {
    position = _position;
    background.setPosition(position);
}

void Object::setSize(Vector2f _size) {
    size = _size;
    background.setSize(size);
}

void Object::setColor(Color color) {
    background.setFillColor(color);
}

void Object::setThickness(float thickness) {
    background.setOutlineThickness(thickness);
}

void Object::draw(RenderWindow& window) const {
    if (visible) {
        window.draw(background);
    }
}

void Object::setVisible(bool _visible) {
    visible = _visible;
}


bool Clickable::isPressed(Vector2i mousePosition) const {
    auto& [posx, posy] = mousePosition;
    auto& [x, y] = position;
    auto& [width, height] = size;

    return visible && 
        ((posx - x) >= 0) && 
        (posx - x <= width) && 
        ((posy - y) >= 0) && 
        (posy - y <= height);
}

void Clickable::setOnPress(const function<void()>& callback) {
    onPressFunction = callback;
}

Clickable::Clickable() : Object() {}

void Clickable::onPress() {
    if (onPressFunction) {
        onPressFunction();
    }
}


void Button::normCaption() {
    const auto& captionString = caption.getString();
    const int captionLength = static_cast<int>(captionString.getSize());

    const auto& [x, y] = position;
    const auto& [width, height] = size;

    const float newX = x + (width - captionLength * fontSize / 2) / 2 + 4;
    const float newY = y + (height - fontSize) / 2 - 4;
    caption.setPosition({ newX, newY });
}

Button::Button() : caption(font) {
    caption.setPosition({0, 0});
    caption.setCharacterSize(fontSize);
    caption.setFillColor(Color::Black);

    setColor(Color::Green);
    setThickness(2);
    setSize({ 125, 50 });
}

Button::Button(const string& _caption, Vector2f _position) 
    : Button() {
    caption.setString(_caption);
    setPosition(_position);
}

Button::Button(const string& _caption, Vector2f _position, Vector2f _size) 
    : Button(_caption, _position) {
    setSize(_size);
}

void Button::setCaption(const string& _caption) {
    caption.setString(_caption);
    normCaption();
}

void Button::setPosition(Vector2f _position) {
    Object::setPosition(_position);
    normCaption();
}

void Button::setSize(Vector2f _size) {
    Object::setSize(_size);
    normCaption();
}

void Button::onPress() {
    //background.setFillColor(Color(50, 170, 50));
    Clickable::onPress();
}

void Button::onRelease() {
    background.setFillColor(Color::Green);
}

void Button::draw(RenderWindow& window) const {
    if (visible) {
        Object::draw(window);
        window.draw(caption);
    }
}


Choice::Choice() {
    status = false;
    setSize({ 20, 20 });
    setThickness(2);
    in.setFillColor(Color(30, 30, 30));
}

Choice::Choice(Vector2f _position, const function<void()>& callback, bool _status, bool _visible) {
    status = _status;
    visible = _visible;
    onPressFunction = callback;
    setPosition(_position);
    setSize({20, 20});
    setThickness(2);
    in.setFillColor(Color(30, 30, 30));
}

void Choice::setPosition(Vector2f _position) {
    Object::setPosition(_position);

    const auto& [x, y] = position;
    const auto& [width, height] = size;

    const float newX = x + width / 4.f;
    const float newY = y + height / 4.f;
    in.setPosition({newX, newY});
}

void Choice::setSize(Vector2f _size) {
    Object::setSize(_size);

    const auto& [x, y] = position;
    const auto& [width, height] = size;

    const float newX = x + width / 4.f;
    const float newY = y + height / 4.f;
    const float newWidth = width / 2.f;
    const float newHeight = height / 2.f;

    in.setPosition({newX, newY});
    in.setSize({ newWidth, newHeight });
}

void Choice::onRelease() { }

void Choice::draw(RenderWindow& window) const {
    if (visible) {
        Object::draw(window);
        if (status) {
            window.draw(in);
        }
    }
}

void Choice::setStatus(bool _status) {
    status = _status;
}

bool Choice::getStatus() {
    return status;
}


Bar::Bar() : text(font) {
    const auto& [x, y] = position;
    const auto& [width, height] = size;

    value = 0.5f;
    posX = width * value;

    first.setPosition(position);
    first.setSize({ posX, height });
    first.setFillColor(Color::Green);

    second.setPosition({ x + posX, y });
    second.setSize({ height - posX, height });
    second.setFillColor(Color::White);

    text.setFillColor(Color::Black);
    text.setPosition({0, 0});
    text.setCharacterSize(fontSize);
}

void Bar::setFirstColor(Color color) {
    first.setFillColor(color);
}

void Bar::setSecondColor(Color color) {
    second.setFillColor(color);
}

void Bar::draw(RenderWindow& window) const {
    if (visible) {
        Object::draw(window);
        window.draw(first);
        window.draw(second);
        window.draw(text);
    }
}

inline void ProgressBar::setWidth() {
    const float width = size.x;
    posX = width * value;
}

inline void ProgressBar::setTextPosition() {
    const auto& [x, y] = position;
    const auto& [width, height] = size;

    const float newX = x + 4;
    const float newY = y + (height - fontSize) / 2.f - 3;
    text.setPosition({ newX, newY });
}

inline void ProgressBar::setString() {
    const int toSet = static_cast<int>(round(abs(value) * 100));
    string result = to_string(toSet) + "%";
    text.setString(result);
}

ProgressBar::ProgressBar() : Bar() { }

void ProgressBar::setPosition(Vector2f _position) {
    Object::setPosition(_position);
    first.setPosition(_position);

    const auto& [x, y] = _position;
    second.setPosition({ x + posX, y });
    setTextPosition();
}

void ProgressBar::setSize(Vector2f _size) {
    Object::setSize(_size);
    first.setSize(_size);

    const auto& [x, y] = position;
    const auto& [width, height] = _size;
    second.setSize({ width - posX, height });
    second.setPosition({ x + posX, y });

    setTextPosition();
}

void ProgressBar::setValue(float _value) {
    value = _value;
    setWidth();
    setString();
    setTextPosition();

    const auto& [x, y] = position;
    const auto& [width, height] = size;

    first.setSize({ posX, height });
    second.setPosition({ x + posX, y });
    second.setSize({ width - posX, height });
}


inline void AssessBar::setHeight() {

    const auto& [x, y] = position;
    const auto& [width, height] = size;

    if (abs(value) < 0.2) {
        posX = height / 2;
    }
    else if (value > 5) {
        posX = height;
    }
    else if (value < -5) {
        posX = 0;
    }
    else {
        posX = height * (value + 5) / 10;
    }
    posX = height - posX;
    first.setSize({ width, posX });
    second.setPosition({ x, y + posX });
    second.setSize({ width, height - posX });
}

inline void AssessBar::setTextColor() {
    if (value < 0) {
        text.setFillColor(second.getFillColor());
    }
    else {
        text.setFillColor(first.getFillColor());
    }
}

inline void AssessBar::setTextPosition() {
    const auto& [x, y] = position;
    const auto& [width, height] = size;

    const float newX = x + 8;
    const float newY = value < 0
        ? y + 4
        : y + height - fontSize - 4;

    text.setPosition({ newX, newY });
}

inline void AssessBar::setString() {
    float toSet = round(abs(value) * 100) / 100;
    string result = "";
    if (toSet > 0.15) {
        if (toSet > 99) {
            result = "win";
        }
        else {
            result = to_string(toSet);
            const int dot = static_cast<int>(result.find('.'));
            result = result.substr(0, dot + 2);
        }
    }
    text.setString(result);
}

AssessBar::AssessBar() : Bar() {
    isFlipped = false;
    text.setCharacterSize(fontSize - 6);
    text.setOutlineThickness(1);
}

void AssessBar::setPosition(Vector2f _position) {
    Object::setPosition(_position);
    first.setPosition(_position);

    const auto& [x, y] = _position;
    second.setPosition({ x, y + posX });
    setTextPosition();
}

void AssessBar::setSize(Vector2f _size) {
    Object::setSize(_size);
    first.setSize(_size);

    const auto& [x, y] = position;
    const auto& [width, height] = _size;
    second.setSize({ width, height - posX });
    second.setPosition({ x, y + posX });
    setTextPosition();
}

void AssessBar::setValue(float toSet) {
    if (isFlipped) {
        value = -toSet;
    }
    else {
        value = toSet;
    }
    setHeight();
    setString();
    setTextColor();
    setTextPosition();
}

void AssessBar::flip() {
    isFlipped = !isFlipped;
    value = -value;
    setHeight();
    setTextPosition();
    Color temp;
    temp = first.getFillColor();
    first.setFillColor(second.getFillColor());
    second.setFillColor(temp);
}


CommentSection::CommentSection() : Object() {
    setThickness(2);
    values.resize(10, 0);

    const auto& [x, y] = position;

    //TODO: CLEAN THIS SHIT
    Text text(font);
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);
    text.setString("Results:");
    text.setPosition(Vector2f( x + 50, y ));
    vText.push_back(text);

    text.setString("0");
    text.setPosition(Vector2f( x, y + 50 ));
    vText.push_back(text);
    text.setString("Best");
    text.setPosition(Vector2f( x + 30, y + 50 ));
    vText.push_back(text);
    text.setString("0");
    text.setPosition(Vector2f( x + 200, y + 50 ));
    vText.push_back(text);

    text.setString("0");
    text.setPosition(Vector2f( x, y + 100 ));
    vText.push_back(text);
    text.setString("Good");
    text.setPosition(Vector2f( x + 30, y + 100 ));
    vText.push_back(text);
    text.setString("0");
    text.setPosition(Vector2f( x + 200, y + 100 ));
    vText.push_back(text);

    text.setString("0");
    text.setPosition(Vector2f( x, y + 150 ));
    vText.push_back(text);
    text.setString("Forced");
    text.setPosition(Vector2f( x + 30, y + 150 ));
    vText.push_back(text);
    text.setString("0");
    text.setPosition(Vector2f( x + 200, y + 150 ));
    vText.push_back(text);

    text.setString("0");
    text.setPosition(Vector2f( x, y + 200 ));
    vText.push_back(text);
    text.setString("Inaccuracy");
    text.setPosition(Vector2f( x + 30, y + 200 ));
    vText.push_back(text);
    text.setString("0");
    text.setPosition(Vector2f( x + 200, y + 200 ));
    vText.push_back(text);

    text.setString("0");
    text.setPosition(Vector2f( x, y + 250 ));
    vText.push_back(text);
    text.setString("Blunder");
    text.setPosition(Vector2f( x + 30, y + 250 ));
    vText.push_back(text);
    text.setString("0");
    text.setPosition(Vector2f( x + 200, y + 250 ));
    vText.push_back(text);

    text.setPosition(Vector2f( x, y + 320 ));
    text.setString("White accuracy:");
    vText.push_back(text);
    text.setPosition(Vector2f( x + 200, y + 320 ));
    text.setString("0.0%");
    vText.push_back(text);

    text.setPosition(Vector2f( x, y + 370 ));
    text.setString("Black accuracy:");
    vText.push_back(text);
    text.setPosition(Vector2f( x + 200, y + 370 ));
    text.setString("0.0%");
    vText.push_back(text);
}

void CommentSection::setPosition(Vector2f _position) {
    Object::setPosition(_position);

    const auto& [x, y] = _position;

    vText[0].setPosition(Vector2f( x + 50, y ));
    vText[1].setPosition(Vector2f( x, y + 50 ));
    vText[2].setPosition(Vector2f( x + 30, y + 50 ));
    vText[3].setPosition(Vector2f( x + 200, y + 50 ));

    vText[4].setPosition(Vector2f( x, y + 100 ));
    vText[5].setPosition(Vector2f( x + 30, y + 100 ));
    vText[6].setPosition(Vector2f( x + 200, y + 100 ));

    vText[7].setPosition(Vector2f( x, y + 150 ));
    vText[8].setPosition(Vector2f( x + 30, y + 150 ));
    vText[9].setPosition(Vector2f( x + 200, y + 150 ));

    vText[10].setPosition(Vector2f( x, y + 200 ));
    vText[11].setPosition(Vector2f( x + 30, y + 200 ));
    vText[12].setPosition(Vector2f( x + 200, y + 200 ));

    vText[13].setPosition(Vector2f( x, y + 250 ));
    vText[14].setPosition(Vector2f( x + 30, y + 250 ));
    vText[15].setPosition(Vector2f( x + 200, y + 250 ));

    vText[16].setPosition(Vector2f( x, y + 320 ));
    vText[17].setPosition(Vector2f( x + 200, y + 320 ));
    vText[18].setPosition(Vector2f( x, y + 370 ));
    vText[19].setPosition(Vector2f( x + 200, y + 370 ));

}

void CommentSection::setValues(vector<AssessMoveData>& vdata) {
    int white, black, sumwhite, sumblack;
    sumblack = sumwhite = white = black = 0;
    /*for (int i = 1; i < vdata.size(); ++i) {
        switch (vdata[i].comment) {
        case BEST:
            if (vdata[i].turn) {
                values[0]++;
                sumwhite += 10;
                white += 10;
            }
            else {
                values[1]++;
                sumblack += 10;
                black += 10;
            }
            break;
        case GOOD:
            if (vdata[i].turn) {
                values[2]++;
                sumwhite += 10;
                white += 10;
            }
            else {
                values[3]++;
                sumblack += 10;
                black += 10;
            }
            break;
        case FORCED:
            if (vdata[i].turn) {
                values[4]++;
                sumwhite += 10;
                white += 10;
            }
            else {
                values[5]++;
                sumblack += 10;
                black += 10;
            }
            break;
        case INACCURACY: 
            if (vdata[i].turn) {
                values[6]++;
                sumwhite += 10;
                white += 3;
            }
            else {
                values[7]++;
                sumblack += 10;
                black += 3;
            }
            break;
        case BLUNDER:
            if (vdata[i].turn) {
                values[8]++;
                sumwhite += 10;
            }
            else {
                values[9]++;
                sumblack += 10;
            }
            break;
        }
    }
    for (int i = 0; i < 5; ++i) {
        vText[3 * i + 1].setString(to_string(values[2 * i]));
        vText[3 * i + 3].setString(to_string(values[2 * i + 1]));
    }
    if (sumwhite) {
        vText[17].setString(to_string(white * 100 / sumwhite) + "." + to_string((white * 1000 / sumwhite) % 10) + "%");
    }
    if (sumblack) {
        vText[19].setString(to_string(black * 100 / sumblack) + "." + to_string((black * 1000 / sumblack) % 10) + "%");
    }*/
}

void CommentSection::draw(RenderWindow& window) const {
    Object::draw(window);
    std::ranges::for_each(vText, [&window](const auto& elem) {
        window.draw(elem);
    });
}


Input::Input() : text(font) {
    text.setPosition(Vector2f( 3, 3 ));
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);
    setColor(Color::White);
    text.setString("");

    isSelected = false;
}

Input::Input(Vector2f _position, MODE mode, int _maxTextLength) : text(font) {
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);

    maxTextLength = _maxTextLength;

    switch (mode) {
    case IP:
        numbers = true;
        dot = true;
        break;
    case PORT: 
        numbers = true;
        break;
    }
}

void Input::onPress() {
    isSelected = visible;
}

void Input::onKeyPress(char symbol) {

    if (isSelected && visible) {
        string currentString = text.getString();
        const int size = static_cast<int>(currentString.size());

        if (size < maxTextLength) {
            if (letters && std::isalpha(symbol)) {
                currentString.push_back(symbol);
            }
            else if (numbers && std::isdigit(symbol)) {
                currentString.push_back(symbol);
            }
            else if (dot && symbol == '.') {
                currentString.push_back(symbol);
            }
        }
        if (symbol == '\b' && !currentString.empty()) {
            currentString.pop_back();
        }
        text.setString(currentString);
    }
}

void Input::onRelease() {
    isSelected = false;
}

void Input::draw(RenderWindow& window) const {
    if (visible) {
        Clickable::draw(window);
        window.draw(text);
    }
}

void Input::setPosition(Vector2f _position) {
    Clickable::setPosition(_position);

    const auto& [x, y] = _position;
    const auto& [width, height] = size;

    const float newX = x + 3;
    const float newY = y + (height - fontSize) / 2.f - 4;
    text.setPosition({newX, newY});
}

void Input::setSize(Vector2f _size) {
    Clickable::setSize(_size);
}

void Input::setLimit(int _maxTextLength) {
    maxTextLength = _maxTextLength;
}

string Input::getText() {
    return text.getString();
}


TWait::TWait() {
    std::ranges::for_each(circlesArray, [](auto& circle) {
        circle.setFillColor(Color::White);
        circle.setRadius(30);
        circle.setOutlineColor(Color::Black);
        circle.setOutlineThickness(3);
        });
    normalizePosition();
}

void TWait::setNext() {
    circlesArray[currentState].setFillColor(Color::White);
    const int next = (currentState + 1) % 6;
    circlesArray[next].setFillColor(Color::Green);
    currentState = next;
}

void TWait::setPosition(Vector2f _position) {
    Object::setPosition(_position);
}

void TWait::setRadius(int _radius) {
    radius = _radius;
    normalizePosition();
}

void TWait::normalizePosition() {
    constexpr float sqrt3 = 1.73205080757f;
    const auto& [x, y] = position;

    circlesArray[0].setPosition({ x + radius * sqrt3 / 2.f, y });
    circlesArray[1].setPosition({ x + radius * sqrt3, y + radius / 2 });
    circlesArray[2].setPosition({ x + radius * sqrt3, y + 3 * radius / 2 });
    circlesArray[3].setPosition({ x + radius * sqrt3 / 2.f, y + 2 * radius });
    circlesArray[4].setPosition({ x, y + 3 * radius / 2 });
    circlesArray[5].setPosition({ x, y + radius / 2 });
}

void TWait::draw(RenderWindow& window) const {
    if (visible) {
        std::ranges::for_each(circlesArray, [&window](const auto& elem) {
            window.draw(elem);
        });
    }
}



void CheckersClock::tictac() {
    while (gameIsGoing && value > 0) {
        sf::sleep(sf::milliseconds(1000));
        if (yourTurn) {
            value--;
            text.setString(getStringTime(value));
        }
    }
}

string CheckersClock::getStringTime(int seconds) {
    return to_string(seconds / 60) + ":" + to_string(seconds % 60 / 10) + to_string(seconds % 10);
}

CheckersClock::CheckersClock() : Object(), text(font) {
    value = 0;
    setThickness(3);
    background.setFillColor(Color::White);
    text.setString(getStringTime(0));
    text.setFillColor(Color::Black);
    text.setCharacterSize(32);
    //thread = new std::thread(&TClock::tictac, this);
    setSize({150, 50});
}

void CheckersClock::update(int seconds) {
    value = seconds;
    text.setString(getStringTime(seconds));
}

void CheckersClock::setPosition(Vector2f _position) {
    Object::setPosition(position);

    const auto& [x, y] = _position;
    text.setPosition({ x + 47, y + 4 });
}

void CheckersClock::start() {
    gameIsGoing = true;
    yourTurn = true;
    //thread->launch();
}

void CheckersClock::pause() {
    background.setFillColor(Color(220, 220, 220));
    yourTurn = false;
}

void CheckersClock::release() {
    background.setFillColor(Color::White);
    yourTurn = true;
}

void CheckersClock::draw(RenderWindow& window) const {
    Object::draw(window);
    window.draw(text);
}

void CheckersClock::stop() {
    gameIsGoing = false;
    //thread->terminate();
}