#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "forms.h"

//TcpSocket socket;
//TcpListener listener;

using sf::Font, sf::Image;

Font font;
Image icon;

int main()
{
    if (!icon.loadFromFile("Image/icon.png")) {
        return 1;
    }

    if (!font.openFromFile("Fonts/arialmt.ttf")) {
        return 2;
    }

    TStartForm mainForm;
    mainForm.poll();

    return 0;
}