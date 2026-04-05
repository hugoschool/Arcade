#include "games/centipede/Centipede.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/DisplayTextContent.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/IEvent.hpp"
#include "cacarcade/Tile.hpp"
#include "cacarcade/Utils.hpp"
#include "events/AEvent.hpp"
#include "games/AGameModule.hpp"
#include "games/ScoreHandler.hpp"
#include <chrono>
#include <cstddef>
#include <exception>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <random>
#include <stdexcept>
#include <utility>

arcade::CentipedeGame::CentipedeGame() : AGameModule("centipede"),
    _tileInfo(), _playerPos({10, 15}), _oldPlayerPos(_playerPos),
    _projectiles(), _updateTime(std::chrono::milliseconds(100)), _time(),
    _vecCentipedes(), _updateTimeCentipede(std::chrono::milliseconds(250)),
    _timeCentipede(), centipedeCount(5), _width(21), _height(16), _isPaused(false)
{
    _container.dimension = std::make_pair(_width, _height);

    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
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
                .entity = EntityTiles::None,
                .mushroom = MushroomDamage::Destroyed,
                .isEmpty = true,
            };
            _tileInfo.insert({{x, y}, info});
        }
    }
    placeMushroom();
    createCentipede();
}

arcade::CentipedeGame::~CentipedeGame()
{
}

void arcade::CentipedeGame::createCentipede()
{
    std::random_device device;
    std::mt19937 rng(device());

    std::uniform_int_distribution<std::mt19937::result_type> distribution(5, 9);

    size_t centipedeAmount = distribution(rng);
    int x = 0;
    int y = 0;

    for (size_t i = 0; i < centipedeAmount; i++) {
        _vecCentipedes.push_back({1, 0, {x, y}});
        x++;
    }
    _timeCentipede = std::chrono::steady_clock::now();
}

void arcade::CentipedeGame::placeMushroom()
{
    std::random_device device;
    std::mt19937 rng(device());

    std::uniform_int_distribution<std::mt19937::result_type> width(0, _container.dimension.first - 1);
    std::uniform_int_distribution<std::mt19937::result_type> height(1, _container.dimension.second - 3);
    std::uniform_int_distribution<std::mt19937::result_type> Box(10, 30);

    std::size_t BoxAmount = Box(rng);

    for (std::size_t i = 0; i < BoxAmount; i++) {
        const std::size_t randomWidth = width(rng);
        const std::size_t randomHeight = height(rng);

        try {
            TileInfo &info = _tileInfo.at({randomWidth, randomHeight});
            if (info.isEmpty) {
                info.isEmpty = false;
                info.mushroom = MushroomDamage::VeryHigh;
            } else {
                BoxAmount++;
                continue;
            }
        } catch (const std::out_of_range &) {
            std::cerr << "Unexpected error: impossible to get Box of tile " << randomWidth << ", " << randomHeight << std::endl;
        }
    }
}

void arcade::CentipedeGame::setEntityContent(cacarcade::Tile &tile, std::pair<const cacarcade::tileCoordinates, TileInfo> info)
{
    switch (info.second.entity) {
        case EntityTiles::Player:
            tile.text = 'P';
            tile.textColor = cacarcade::Color::Blue;
            break;
        case EntityTiles::Projectile:
            tile.text = '|';
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

void arcade::CentipedeGame::updateTiles()
{
    for (auto &info : _tileInfo) {
        cacarcade::Tile &tile = _container.tiles.at(info.first);
        if (info.second.isEmpty) {
            tile.textColor = cacarcade::Color::Green;
            tile.text = '\0';
            continue;
        } else {
            if (info.second.entity != EntityTiles::None) {
                setEntityContent(tile, info);
            } else {
                tile.textColor = cacarcade::Color::Red;
                tile.text = '0' + static_cast<char>(info.second.mushroom) + 1;
            }
        }
    }
}

void arcade::CentipedeGame::projectileCollisons(std::pair<size_t, size_t> &position)
{
    TileInfo &info = _tileInfo.at(position);

    if (info.mushroom != MushroomDamage::Destroyed) {
        int dmg = static_cast<int>(info.mushroom) - 1;
        if (dmg < 0) {
            info.mushroom = MushroomDamage::Destroyed;
            info.isEmpty = true;
            info.entity = EntityTiles::None;
            _scoreHandler.addScore(1);
        } else {
            info.mushroom = static_cast<MushroomDamage>(dmg);
        }
    } else {
        for (auto i = _vecCentipedes.begin(); i != _vecCentipedes.end(); i++) {
            Centipede &centipede = *i;
            if (centipede.position == position) {
                _vecCentipedes.erase(i);
                _scoreHandler.addScore(5);
                break;
            }
        }
        info.mushroom = MushroomDamage::VeryHigh;
        info.entity = EntityTiles::None;
        info.isEmpty = false;
    }
}

void arcade::CentipedeGame::updateCentipede()
{
    if (std::chrono::steady_clock::now() - _timeCentipede < _updateTimeCentipede)
        return;
    for (auto &centipede : _vecCentipedes) {
        TileInfo &info = _tileInfo.at(centipede.position);
        info.entity = EntityTiles::None;
        if (info.mushroom == MushroomDamage::Destroyed)
            info.isEmpty = true;
        if ((centipede.position.first < _width - 1 && centipede.direction > 0) || (centipede.position.first > 0 && centipede.direction < 0)) {
            TileInfo &NextTile = _tileInfo.at({centipede.position.first + centipede.direction,
            centipede.position.second});
            if (!NextTile.isEmpty && NextTile.mushroom != MushroomDamage::Destroyed) {
                centipede.direction *= -1;
                if (centipede.position.second < _height - 1)
                        centipede.position.second += 1;
            } else {
                centipede.position.first += centipede.direction;
            }
        } else {
            if (centipede.position.second < _height - 1) {
                centipede.direction *= -1;
                centipede.position.second += 1;
            } else {
                for (auto i = _vecCentipedes.begin(); i != _vecCentipedes.end(); i++) {
                    Centipede centi = *i;
                    if (centi.position == centipede.position) {
                        _vecCentipedes.erase(i);
                        _timeCentipede = std::chrono::steady_clock::now();
                        _scoreHandler.addScore(-5);
                        return;
                    }
                }
            }
        }
    }
    _timeCentipede = std::chrono::steady_clock::now();
}

void arcade::CentipedeGame::placeCentipede()
{
    updateCentipede();
    for (auto &centipede : _vecCentipedes) {
        try {
            TileInfo &info = _tileInfo.at(centipede.position);
            info.entity = EntityTiles::Centipede;
            info.isEmpty = false;
        } catch (std::exception &e) {
        }
    }
    if (_vecCentipedes.empty()) {
        createCentipede();
        centipedeCount -= 1;
    }
    if (_vecCentipedes.empty() && centipedeCount == 0) {
        _scoreHandler.saveScore("Type shit");
        _scoreHandler.resetScore();
        _isPaused = true;
    }
}

//TODO maybe rework _projectiles per tile
void arcade::CentipedeGame::updateProjectile()
{
    if ((std::chrono::steady_clock::now() - _time) >= _updateTime) {
        for (auto i = _projectiles.begin(); i != _projectiles.end(); i++) {
            std::pair<size_t, size_t> &Projectile = *i;
            TileInfo &oldProj = _tileInfo.at(Projectile);
            oldProj.entity = EntityTiles::None;
            oldProj.isEmpty = true;
            oldProj.mushroom = MushroomDamage::Destroyed;
            if (Projectile.second > 0) {
                Projectile.second -= 1;
                TileInfo &NextTile = _tileInfo.at(Projectile);
                if (!NextTile.isEmpty) {
                    projectileCollisons(Projectile);
                    _projectiles.erase(i);
                    break;
                }
            } else {
                _projectiles.erase(i);
                break;
            }
        }
        _time = std::chrono::steady_clock::now();
    }
}

void arcade::CentipedeGame::updatePlayer()
{
    TileInfo &info = _tileInfo.at(_oldPlayerPos);

    info.isEmpty = true;
    info.entity = EntityTiles::None;
    info.mushroom = MushroomDamage::Destroyed;
    TileInfo &info2 = _tileInfo.at(_playerPos);
    info2.mushroom = MushroomDamage::Destroyed;
    info2.entity = EntityTiles::Player;
    info2.isEmpty = false;
    _oldPlayerPos = _playerPos;

    //update _projectiles Position to the tile info
    updateProjectile();
    for (auto &proj : _projectiles) {
        TileInfo &projectileInfo = _tileInfo.at(proj);
        projectileInfo.entity = EntityTiles::Projectile;
        projectileInfo.isEmpty = false;
        projectileInfo.mushroom = MushroomDamage::Destroyed;
    }
}

void arcade::CentipedeGame::addProjectile()
{
    this->_projectiles.push_back({_playerPos.first, _playerPos.second - 1});
    if (_projectiles.size() == 1)
        _time = std::chrono::steady_clock::now();
    if (!_tileInfo.at({_playerPos.first, _playerPos.second - 1}).isEmpty) {
        cacarcade::tileCoordinates position = {_playerPos.first, _playerPos.second - 1};
        projectileCollisons(position);
        _projectiles.pop_back();
    }
}

void arcade::CentipedeGame::reset()
{
    _scoreHandler.saveScore("Type shit");
    AGameModule::reset();
    _container.tiles.clear();
    _tileInfo.clear();
    _vecCentipedes.clear();
    centipedeCount = 20;
    for (size_t y = 0; y < _height; y++) {
        for (size_t x = 0; x < _width; x++) {
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
                .entity = EntityTiles::None,
                .mushroom = MushroomDamage::Destroyed,
                .isEmpty = true,
            };
            _tileInfo.insert({{x, y}, info});
        }
    }
    placeMushroom();
    createCentipede();
}

bool arcade::CentipedeGame::canPlayerMove(int x, int y)
{
    TileInfo &info  = _tileInfo.at({_playerPos.first + x, _playerPos.second + y});

    if (!info.isEmpty) {
        return false;
    }
    return true;
}

void arcade::CentipedeGame::handleEvent(std::unique_ptr<cacarcade::IEvent> &event)
{
    switch (event->getType()) {
        case cacarcade::EventType::KeyPressed: {
            switch (event->getKey()) {
                case cacarcade::EventKey::Q:
                    if (_playerPos.first > 0 && canPlayerMove(-1, 0))
                        _playerPos.first -= 1;
                    break;
                case cacarcade::EventKey::D:
                    if (_playerPos.first < 20 && canPlayerMove(1, 0))
                        _playerPos.first += 1;
                    break;
                case cacarcade::EventKey::Z:
                    if (_playerPos.second > 13 && canPlayerMove(0, -1))
                        _playerPos.second -= 1;
                    break;
                case cacarcade::EventKey::S:
                    if (_playerPos.second < 15 && canPlayerMove(0, -1))
                        _playerPos.second += 1;
                    break;
                case cacarcade::EventKey::Space:
                    _isPaused = !_isPaused;
                    break;
                default:
                    break;
            }
            break;
        }
        case cacarcade::EventType::TileClicked: {
            addProjectile();
            break;
        }
        default:
            break;
    }
}

void arcade::CentipedeGame::checkPlayerCollision()
{
    TileInfo &info = _tileInfo.at(_playerPos);

    if (info.entity == EntityTiles::Centipede) {
        reset();
    }
}

cacarcade::DisplayTextContent arcade::CentipedeGame::addTextContent()
{
    cacarcade::DisplayTextContent text;
    text.color = cacarcade::Color::White;
    text.coordinates = {1500, 0};
    text.size = 15;
    text.text = "TEST";
    return text;
}

void arcade::CentipedeGame::update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event)
{
    if (event.has_value()) {
        handleEvent(event.value());
    } else {
        std::unique_ptr<cacarcade::IEvent> temp = std::make_unique<arcade::AEvent>(cacarcade::EventType::DisplayText);
        temp->setTextContent(addTextContent());
        event = std::move(temp);
    }

    if (!_isPaused) {
        if (centipedeCount > 0) {
            placeCentipede();
            checkPlayerCollision();
            updatePlayer();
        }
        updateTiles();
    }
}
