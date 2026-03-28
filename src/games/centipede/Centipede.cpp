#include "games/centipede/Centipede.hpp"
#include "cacarcade/Color.hpp"
#include "cacarcade/EventKey.hpp"
#include "cacarcade/EventType.hpp"
#include "cacarcade/Tile.hpp"
#include "cacarcade/Utils.hpp"
#include "games/AGameModule.hpp"
#include "games/ScoreHandler.hpp"
#include <chrono>
#include <cstddef>
#include <exception>
#include <iostream>
#include <ostream>
#include <random>
#include <stdexcept>
#include <utility>

arcade::CentipedeGame::CentipedeGame() : AGameModule("centipede"),
    _tileInfo(), PlayerPos({10, 15}), OldPlayerPos(PlayerPos),
    Projectiles(), updateTime(std::chrono::milliseconds(100)), _time(),
    vecCentipedes(), updateTimeCentipede(std::chrono::milliseconds(250)),
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
                .Entity = EntityTiles::None,
                .Mushroom = MushroomDamage::Destroyed,
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
        vecCentipedes.push_back({1, 0, {x, y}});
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

void arcade::CentipedeGame::setEntityPosition(cacarcade::Tile &tile, std::pair<const cacarcade::tileCoordinates, TileInfo> info)
{
    switch (info.second.Entity) {
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
            if (info.second.Entity != EntityTiles::None) {
                setEntityPosition(tile, info);
            } else {
                tile.textColor = cacarcade::Color::Red;
                tile.text = '0' + static_cast<char>(info.second.Mushroom) + 1;
            }
        }
    }
}

void arcade::CentipedeGame::projectileCollisons(std::pair<size_t, size_t> position)
{
    TileInfo &info = _tileInfo.at(position);

    if (info.Mushroom != MushroomDamage::Destroyed) {
        int dmg = static_cast<int>(info.Mushroom) - 1;
        if (dmg < 0) {
            info.Mushroom = MushroomDamage::Destroyed;
            info.isEmpty = true;
            info.Entity = EntityTiles::None;
            _scoreHandler.addScore(1);
        } else {
            info.Mushroom = static_cast<MushroomDamage>(dmg);
        }
    } else {
        for (auto i = vecCentipedes.begin(); i != vecCentipedes.end(); i++) {
            Centipede &centipede = *i;
            if (centipede.position == position) {
                vecCentipedes.erase(i);
                _scoreHandler.addScore(5);
                break;
            }
        }
        info.Mushroom = MushroomDamage::VeryHigh;
        info.Entity = EntityTiles::None;
        info.isEmpty = false;
    }
}

void arcade::CentipedeGame::updateCentipede()
{
    if (std::chrono::steady_clock::now() - _timeCentipede < updateTimeCentipede)
        return;
    for (auto &centipede : vecCentipedes) {
        TileInfo &info = _tileInfo.at(centipede.position);
        info.Entity = EntityTiles::None;
        if (info.Mushroom == MushroomDamage::Destroyed)
            info.isEmpty = true;
        if ((centipede.position.first < _width - 1 && centipede.direction > 0) || (centipede.position.first > 0 && centipede.direction < 0)) {
            TileInfo &NextTile = _tileInfo.at({centipede.position.first + centipede.direction,
            centipede.position.second});
            if (!NextTile.isEmpty && NextTile.Mushroom != MushroomDamage::Destroyed) {
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
                // chage score to an int instead of a size_t
                for (auto i = vecCentipedes.begin(); i != vecCentipedes.end(); i++) {
                    Centipede centi = *i;
                    if (centi.position == centipede.position) {
                        vecCentipedes.erase(i);
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
    for (auto &centipede : vecCentipedes) {
        try {
            TileInfo &info = _tileInfo.at(centipede.position);
            info.Entity = EntityTiles::Centipede;
            info.isEmpty = false;
        } catch (std::exception &e) {
            std::cout << "pipi\n";
        }
    }
    if (vecCentipedes.empty()) {
        createCentipede();
        centipedeCount -= 1;
    }
    if (vecCentipedes.empty() && centipedeCount == 0) {
        _scoreHandler.saveScore("Type shit");
        _scoreHandler.resetScore();
        _isPaused = true;
    }
}

//TODO maybe rework projectiles per tile
void arcade::CentipedeGame::updateProjectile()
{
    if ((std::chrono::steady_clock::now() - _time) >= updateTime) {
        for (auto i = Projectiles.begin(); i != Projectiles.end(); i++) {
            std::pair<size_t, size_t> &Projectile = *i;
            TileInfo &oldProj = _tileInfo.at(Projectile);
            oldProj.Entity = EntityTiles::None;
            oldProj.isEmpty = true;
            oldProj.Mushroom = MushroomDamage::Destroyed;
            if (Projectile.second > 0) {
                Projectile.second -= 1;
                TileInfo &NextTile = _tileInfo.at(Projectile);
                if (!NextTile.isEmpty) {
                    projectileCollisons(Projectile);
                    Projectiles.erase(i);
                    break;
                }
            } else {
                Projectiles.erase(i);
                break;
            }
        }
        _time = std::chrono::steady_clock::now();
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

    //update Projectiles Position to the tile info
    updateProjectile();
    for (auto &proj : Projectiles) {
        TileInfo &projectileInfo = _tileInfo.at(proj);
        projectileInfo.Entity = EntityTiles::Projectile;
        projectileInfo.isEmpty = false;
        projectileInfo.Mushroom = MushroomDamage::Destroyed;
    }
}

void arcade::CentipedeGame::addProjectile()
{
    this->Projectiles.push_back({PlayerPos.first, PlayerPos.second - 1});
    if (Projectiles.size() == 1)
        _time = std::chrono::steady_clock::now();
    if (!_tileInfo.at({PlayerPos.first, PlayerPos.second - 1}).isEmpty) {
        projectileCollisons({PlayerPos.first, PlayerPos.second - 1});
        Projectiles.pop_back();
    }
}

void arcade::CentipedeGame::reset()
{
    _scoreHandler.saveScore("Type shit");
    AGameModule::reset();
    _container.tiles.clear();
    _tileInfo.clear();
    vecCentipedes.clear();
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
                .Entity = EntityTiles::None,
                .Mushroom = MushroomDamage::Destroyed,
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
    TileInfo &info  = _tileInfo.at({PlayerPos.first + x, PlayerPos.second + y});

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
                    if (PlayerPos.first > 0 && canPlayerMove(-1, 0))
                        PlayerPos.first -= 1;
                    break;
                case cacarcade::EventKey::D:
                    if (PlayerPos.first < 20 && canPlayerMove(1, 0))
                        PlayerPos.first += 1;
                    break;
                case cacarcade::EventKey::Z:
                    if (PlayerPos.second > 13 && canPlayerMove(0, -1))
                        PlayerPos.second -= 1;
                    break;
                case cacarcade::EventKey::S:
                    if (PlayerPos.second < 15 && canPlayerMove(0, -1))
                        PlayerPos.second += 1;
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
    TileInfo &info = _tileInfo.at(PlayerPos);

    if (info.Entity == EntityTiles::Centipede) {
        reset();
    }
}

void arcade::CentipedeGame::update(std::optional<std::unique_ptr<cacarcade::IEvent>> &event)
{
    if (event.has_value())
        handleEvent(event.value());
    if (!_isPaused) {
        if (centipedeCount > 0) {
            placeCentipede();
            checkPlayerCollision();
            updatePlayer();
        }
        updateTiles();
    }
    std::cout << _scoreHandler.getScore() << std::endl;
}

