#include "graphicals/SFML/SFML.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventMouseButton.hpp"
#include "cacarcade/Tile.hpp"
#include "cacarcade/TileContainer.hpp"
#include "common/Exception.hpp"
#include "events/KeyPressedEvent.hpp"
#include "events/QuitEvent.hpp"
#include "events/TileClickedEvent.hpp"
#include "graphicals/ADisplayModule.hpp"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <exception>
#include <iostream>
#include <memory>
#include <optional>

arcade::SFMLDisplay::SFMLDisplay() : arcade::ADisplayModule(),
    _window(), _font(), _outlineThickness(1)
{
}

arcade::SFMLDisplay::~SFMLDisplay()
{}

void arcade::SFMLDisplay::open()
{
    try {
        sf::VideoMode videoMode(_screenWidth, _screenHeight);

        _window.create(videoMode, "Arcade");
        _window.setFramerateLimit(60);
        _font = sf::Font();
        if (!_font.loadFromFile("/usr/share/fonts/gnu-free/FreeSans.otf"))
            throw arcade::Exception("Font wasn't loaded correctly.");
    } catch (std::exception &e) {
        throw arcade::Exception("Something went wrong with the creation of the window.");
    }
}

void arcade::SFMLDisplay::close()
{
    _window.close();
}

void arcade::SFMLDisplay::clear()
{
    _window.clear(sf::Color::White);
}

cacarcade::EventKey arcade::SFMLDisplay::getKey(const sf::Keyboard::Key key)
{
    if (key >= sf::Keyboard::Key::A && key <= sf::Keyboard::Key::Z) {
        return static_cast<cacarcade::EventKey>(
            static_cast<int>(cacarcade::EventKey::A) + static_cast<int>(key) - static_cast<int>(sf::Keyboard::Key::A)
        );
    }

    switch (key) {
        case sf::Keyboard::Key::Backspace:
            return cacarcade::EventKey::Backspace;
        case sf::Keyboard::Key::Up:
            return cacarcade::EventKey::Up;
        case sf::Keyboard::Key::Down:
            return cacarcade::EventKey::Down;
        case sf::Keyboard::Key::Left:
            return cacarcade::EventKey::Left;
        case sf::Keyboard::Key::Right:
            return cacarcade::EventKey::Right;
        case sf::Keyboard::Key::Space:
            return cacarcade::EventKey::Space;
        default:
            return cacarcade::EventKey::None;
    }
}

cacarcade::EventMouseButton arcade::SFMLDisplay::getMouseButton(sf::Mouse::Button button)
{
    if (button == sf::Mouse::Button::Left)
        return cacarcade::EventMouseButton::Left;
    if (button == sf::Mouse::Button::Right)
        return cacarcade::EventMouseButton::Right;
    return cacarcade::EventMouseButton::Left;
}

std::optional<std::unique_ptr<cacarcade::IEvent>> arcade::SFMLDisplay::pollEvent()
{
    sf::Event event;

    while (_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            return std::make_unique<arcade::QuitEvent>();
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Event::MouseButtonEvent mouseEvent = event.mouseButton;

            return std::make_unique<arcade::TileClickedEvent>(
                findClosestTile(mouseEvent.x, mouseEvent.y),
                getMouseButton(mouseEvent.button)
            );
        }

        if (event.type == sf::Event::KeyPressed) {
            const sf::Event::KeyEvent keyPressed = event.key;

            return std::make_unique<arcade::KeyPressedEvent>(getKey(keyPressed.code));
        }
    }
    return std::nullopt;
}

std::optional<std::weak_ptr<sf::Texture>> arcade::SFMLDisplay::createTexture(std::string &textureName)
{
    try {
        return std::weak_ptr<sf::Texture>(_textureMap.at(textureName));
    } catch (const std::out_of_range &) {
        std::shared_ptr<sf::Texture> texture = std::make_shared<sf::Texture>();

        if (!texture->loadFromFile(textureName)) {
            std::cerr << "An error occured with the creation of the " << textureName << " texture." << std::endl;
            return std::nullopt;
        }

        texture->setSmooth(true);
        _textureMap.insert({textureName, texture});

        return std::weak_ptr<sf::Texture>(texture);
    }
}

sf::Color arcade::SFMLDisplay::getColor(cacarcade::ColorCode color)
{
    return sf::Color(color.r, color.g, color.b, color.a);
}

void arcade::SFMLDisplay::displayTileText(cacarcade::Tile &tile, sf::RectangleShape &tileRect)
{
    sf::Color backgroundColor = getColor(tile.backgroundColor);
    sf::Color textColor = getColor(tile.textColor);

    tileRect.setFillColor(backgroundColor);
    tileRect.setOutlineColor(textColor);
    tileRect.setOutlineThickness(_outlineThickness);

    _window.draw(tileRect);

    if (tile.text != '\0') {
        sf::Text text(tile.text, _font);

        sf::Vector2f pos = tileRect.getPosition();
        pos.x += tileRect.getSize().x / 4;
        pos.y -= tileRect.getSize().y / 20;
        text.setPosition(pos);

        text.setFillColor(textColor);
        text.setCharacterSize(_tileSize - (tileRect.getSize().x / 5));

        _window.draw(text);
    }
}

void arcade::SFMLDisplay::displayTileTexture(cacarcade::Tile &tile, sf::RectangleShape &tileRect)
{
    std::optional<std::weak_ptr<sf::Texture>> ptr = createTexture(tile.textureName);
    if (!ptr.has_value()) {
        displayTileText(tile, tileRect);
        return;
    }

    sf::Texture texture = *(ptr.value()).lock();

    tileRect.setTexture(&texture);
    _window.draw(tileRect);
}

void arcade::SFMLDisplay::displayTiles(cacarcade::TileContainer container)
{
    setTileDimensions(container.dimension);

    for (auto &[_, tile] : container.tiles) {
        int x = tile.x * _tileSize;
        int y = tile.y * _tileSize;

        sf::RectangleShape rec(sf::Vector2f(_tileSize - (_outlineThickness * 2), _tileSize - (_outlineThickness * 2)));
        rec.setPosition(sf::Vector2f(x, y));

        if (!tile.textureName.empty()) {
            displayTileTexture(tile, rec);
        } else {
            displayTileText(tile, rec);
        }
    }
    _window.display();
}
