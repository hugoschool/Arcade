#include "games/centipede/Centipede.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/Tile.hpp"
#include "games/AGameModule.hpp"
#include <cstddef>
#include <iostream>
#include <random>
#include <stdexcept>
#include <utility>

arcade::CentipedeGame::CentipedeGame() : AGameModule("centipede"),
    _tileInfo(), PlayerPos({10, 15}), OldPlayerPos(PlayerPos),
    projectiles()
{
    size_t width = 21;
    size_t height = 16;

    _container.dimension = std::make_pair(width, height);

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            cacarcade::Tile tile = {
                .x = x,
                .y = y,
                .textureName = "",
                .backgroundColor = cacarcade::Color::Black,
                .text = '\0',
                .textColor = cacarcade::Color::Green,
            };

            _container.tiles.insert({{x, y}, tile});

            TileInfo info = {
                .Entity = EntityTiles::None,
                .Mushroom = MushroomDamage::Destroyed,
                .isEmpty = true,
            };
            _tileInfo.insert({{x, y}, info});
        }
    }
    placeMushroom();
}

arcade::CentipedeGame::~CentipedeGame()
{
}

void arcade::CentipedeGame::placeMushroom()
{
    std::random_device device;
    std::mt19937 rng(device());

    std::uniform_int_distribution<std::mt19937::result_type> width(0, _container.dimension.first - 1);
    std::uniform_int_distribution<std::mt19937::result_type> height(0, _container.dimension.second - 3);
    std::uniform_int_distribution<std::mt19937::result_type> Box(5, 20);

    std::size_t BoxAmount = Box(rng);

    for (std::size_t i = 0; i < BoxAmount; i++) {
        const std::size_t randomWidth = width(rng);
        const std::size_t randomHeight = height(rng);

        try {
            TileInfo &info = _tileInfo.at({randomWidth, randomHeight});
            if (info.isEmpty) {
                info.isEmpty = false;
                info.Mushroom = MushroomDamage::VeryHigh;
            } else {
                BoxAmount++;
                continue;
            }
        } catch (const std::out_of_range &) {
            std::cerr << "Unexpected error: impossible to get Box of tile " << randomWidth << ", " << randomHeight << std::endl;
        }
    }
}

void arcade::CentipedeGame::updateTiles()
{
    for (auto &info : _tileInfo) {
        cacarcade::Tile &tile = _container.tiles.at(info.first);
        if (info.second.isEmpty) {
            tile.textColor = cacarcade::Color::Green;
            tile.text = '\0';
            continue;
        } else {
            if (info.second.Mushroom != MushroomDamage::Destroyed) {
                tile.textColor = cacarcade::Color::Red;
                tile.text = 'M';
            } else {
                switch (info.second.Entity) {
                    case EntityTiles::Player:
                        tile.text = 'P';
                        tile.textColor = cacarcade::Color::Blue;
                        break;
                    case EntityTiles::Projectile:
                        tile.text = 'I';
                        tile.textColor = cacarcade::Color::Yellow;
                        break;
                    case EntityTiles::Centipede:
                        tile.text = 'C';
                        tile.textColor = cacarcade::Color::White;
                        break;
                    case EntityTiles::None:
                        tile.text = '\0';
                        tile.textColor = cacarcade::Color::Green;
                        break;
                }
            }
        }
    }
}

void arcade::CentipedeGame::updatePlayer()
{
    TileInfo &info = _tileInfo.at(OldPlayerPos);

    info.isEmpty = true;
    info.Entity = EntityTiles::None;
    info.Mushroom = MushroomDamage::Destroyed;
    TileInfo &info2 = _tileInfo.at(PlayerPos);
    info2.Mushroom = MushroomDamage::Destroyed;
    info2.Entity = EntityTiles::Player;
    info2.isEmpty = false;
    OldPlayerPos = PlayerPos;
}

// void arcade::CentipedeGame::

void arcade::CentipedeGame::reset()
{
    AGameModule::reset();
}

void arcade::CentipedeGame::handleEvent(std::unique_ptr<cacarcade::IEvent> &event)
{
    switch (event->getType()) {
        case cacarcade::EventType::KeyPressed: {
            switch (event->getKey()) {
                case cacarcade::EventKey::Q:
                    if (PlayerPos.first > 0)
                        PlayerPos.first -= 1;
                    break;
                case cacarcade::EventKey::D:
                    if (PlayerPos.first < 20)
                        PlayerPos.first += 1;
                    break;
                default:
                    break;
            }
            break;
        }
        case cacarcade::EventType::TileClicked: {
            // addProjectile();
            break;
        }
        default:
            break;
    }
}

void arcade::CentipedeGame::update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event)
{
    if (event.has_value())
        handleEvent(event.value());
    updatePlayer();
    updateTiles();
}

