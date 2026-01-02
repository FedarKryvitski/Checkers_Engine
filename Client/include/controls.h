#pragma once
#include <SFML/Graphics.hpp>

#include <array>
#include <string>
#include <thread>
#include <memory>
#include <optional>
#include <functional>

#include "PossibleMoves.h"

extern sf::Font font;

namespace Controls {

using VideoMode = sf::VideoMode;
using Vector2i = sf::Vector2i;
using Vector2f = sf::Vector2f;
using Color = sf::Color;
using RenderWindow = sf::RenderWindow;
using RectangleShape = sf::RectangleShape;
using Text = sf::Text;
using Texture = sf::Texture;
using CircleShape = sf::CircleShape;

constexpr int fontSize = 24;
constexpr int tileSize = 100;

class Object {
public:
    void setVisible(bool _visible);
    virtual void setPosition(Vector2f position);
    virtual void setSize(Vector2f size);
    void setColor(Color color);
    void setThickness(float thickness);
    virtual void draw(RenderWindow& win) const;

protected:
    Object();
    virtual ~Object() {};

    RectangleShape background;
    Vector2f position{}, size{};
    bool visible{ true };
};

class Label final : public Object {
public: 
    Label();
    Label(const std::string& _caption, const Vector2f _pos, bool _visible = true);

    void setText(const std::string& _text);
    void setPosition(Vector2f position) override;
    void draw(RenderWindow& win) const override;
    void setTextThickness(float thick);
    void setFontSize(int fontSize);
    void setTextColor(Color color);
    void setOutlineColor(Color color);

private:
    Text text;
};

class Clickable : public Object {
public:
    Clickable();
    bool isPressed(Vector2i pos) const;
    void setOnPress(const std::function<void()>& callback);
    virtual void onPress();

protected:
    std::function<void()> onPressFunction;
};

class Button final : public Clickable {
public:
    Button();
    Button(const std::string& _caption, Vector2f _pos);
    Button(const std::string& _caption, Vector2f _position, Vector2f _size);

    void setCaption(const std::string& _caption);
    void setPosition(Vector2f position) override;
    void setSize(Vector2f size);
    void onPress() override;
    void onRelease();
    void draw(RenderWindow& win) const override;

private:
    void normCaption();

private:
    Text caption;
};

class Choice final : public Clickable {
public:
    using Callback = std::function<void()>;

public:
    Choice();
    Choice(Vector2f _position, const Callback& callback, bool _status, bool _visible = true);

    void setPosition(Vector2f position) override;
    void setSize(Vector2f position) override;
    void onRelease();
    void draw(RenderWindow& win) const override;
    void setStatus(bool status);
    bool getStatus();

private:
    bool status;
    RectangleShape in;
};

class Bar : public Object {
public:
    Bar();
    virtual void setValue(float toSet) = 0;
    void setFirstColor(Color color);
    void setSecondColor(Color color);
    void draw(RenderWindow& win) const override;

protected:
    RectangleShape first, second;
    Text text;
    float value;
    float posX;
};

class ProgressBar final : public Bar {
public:
    ProgressBar();
    void setPosition(Vector2f position) override;
    void setSize(Vector2f size) override;
    void setValue(float toSet);

private:
    inline void setWidth();
    inline void setTextPosition();
    inline void setString();
};

class AssessBar final : public Bar {
public:
    AssessBar();
    void setPosition(Vector2f position) override;
    void setSize(Vector2f size) override;
    void setValue(float toSet);
    void flip();

private:
    inline void setHeight();
    inline void setTextColor();
    inline void setTextPosition();
    inline void setString();
    bool isFlipped;
};

class CommentSection final : public Object
{
public:
    CommentSection();
    void setPosition(Vector2f position) override;
    //void setValues(std::vector<AssessMoveData>& vdata);
    void draw(RenderWindow& win) const override;

private:
    std::vector<Text> vText;
    std::vector<char> values;
};

class CheckersClock final : public Object {
public:
    CheckersClock();
    void update(int seconds);
    void start();
    void pause();
    void draw(RenderWindow& win) const override;
    void setPosition(Vector2f position) override;
    void release();
    void stop();

private:
    bool gameIsGoing;
    bool yourTurn;
    std::unique_ptr<std::thread> thread;
    int value;
    Text text;
    std::string getStringTime(int seconds);
    void tictac();
};

class Board final : public Clickable {
public:
    Board();
    void catchPiece(Vector2i mousePosition);
    void releasePiece(Vector2i mousePosition);
    void setPosition(Vector2f _position) override;
    void setField(Field& _field);
    std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> getMoveCoordinates();
    void flip();
    void draw(RenderWindow& win) const override;
    // void setComment(MOVE_STATUS comment, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

private:
    inline Vector2i getPieceCoordinates(Vector2i mousePosition);

    // Field field{};
    // MOVE_STATUS comment{};
    std::array<Texture, 5> moveStatusTextures;
    Vector2i moveStart{}, moveEnd{};
    bool isCaptured{false};
    bool flipped {false};
    uint8_t x1{}, y1{}, x2{}, y2{};
    uint8_t cx{ 0 }, cy{ 0 };
};

class Input final : public Clickable {
public:
    enum MODE {
        IP,
        PORT,
    };
    
    Input();
    Input(Vector2f _position, MODE mode, int limit);

    void onPress() override;
    void onKeyPress(char pressed);
    void onRelease();
    void draw(RenderWindow& win) const override;
    void setPosition(Vector2f position) override;
    void setSize(Vector2f position) override;
    void setLimit(int lim);
    std::string getText();

private:
    bool letters{ false };
    bool dot{ false };
    bool numbers{ false };
    bool isSelected{ false };
    Text text;
    int maxTextLength{ 10 };
};

class TWait final : public Object {
public:
    TWait();
    void setNext();
    void setPosition(Vector2f _position) override;
    void setRadius(int tradius);
    void draw(RenderWindow& win) const override;

private:
    std::array<CircleShape, 6> circlesArray;
    int currentState{ 0 };
    int radius{ 100 };

    void normalizePosition();
};

} // namespace Controls
