#include "forms.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "spdlog/spdlog.h"

//TcpSocket socket;
//TcpListener listener;

sf::Font font;
sf::Image icon;

namespace {

using MainForm = Forms::StartForm;

} // namespace

int main()
{
    constexpr auto kIconPath = "resources/Images/icon.png";
    constexpr auto kFontPath = "resources/Fonts/arialmt.ttf";

    spdlog::info("Program started");

    if (!icon.loadFromFile(kIconPath)) 
    {
        spdlog::error("Icon not found by path: {}", kIconPath);
        return -1;
    }

    spdlog::info("Icon loaded succesfully by path: {}", kIconPath);

    if (!font.openFromFile(kFontPath)) 
    {
        spdlog::error("Font not found by path: {}", kFontPath);
        return -1;
    }

    spdlog::info("Font loaded succesfully by path: {}", kFontPath);

    auto form = std::make_unique<MainForm>();
    form->poll();

    return 0;
}