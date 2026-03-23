#include "games/ScoreHandler.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

arcade::ScoreHandler::ScoreHandler(const std::string gameName) :
    _fileName(".scores"), _fileInput(_fileName),
    _gameName(gameName), _score(0), _isSaved(false)
{
}

arcade::ScoreHandler::~ScoreHandler()
{
    if (_fileInput.is_open())
        _fileInput.close();
}

void arcade::ScoreHandler::addScore(std::size_t plusScore)
{
    _score += plusScore;
}

std::size_t arcade::ScoreHandler::getScore() const
{
    return _score;
}

void arcade::ScoreHandler::resetScore()
{
    _score = 0;
}

bool arcade::ScoreHandler::isKeyInFile(const std::string &key)
{
    _fileInput.seekg(0, std::fstream::beg);

    std::string line;
    while (std::getline(_fileInput, line)) {
        if (line == key) {
            return true;
        }
    }
    return false;
}

std::string arcade::ScoreHandler::getKeyGameName() const
{
    std::string keyGameName = "[" + _gameName + "]";

    // Make the key lower case
    std::transform(
        keyGameName.begin(),
        keyGameName.end(),
        keyGameName.begin(),
        [](unsigned char c){ return std::tolower(c); }
    );

    return keyGameName;
}

std::vector<std::pair<const std::string, const std::size_t>> arcade::ScoreHandler::loadScores()
{
    std::vector<std::pair<const std::string, const std::size_t>> scores;

    if (!_fileInput.is_open())
        return scores;

    const std::string keyGameName = getKeyGameName();
    const bool keyInFile = isKeyInFile(keyGameName);

    if (!keyInFile)
        return scores;

    _fileInput.seekg(0, std::fstream::beg);

    std::string line;
    bool foundGame = false;

    while (std::getline(_fileInput, line)) {
        if (line == keyGameName) {
            foundGame = true;
            continue;
        } else if (line.starts_with("[") && line.ends_with("]")) {
            foundGame = false;
            continue;
        }
        if (foundGame) {
            std::string delimiter = "=";
            std::string playerName = line.substr(0, line.find(delimiter));
            std::string scoreString = line.substr(line.find(delimiter), line.length());

            std::size_t score = 0;
            std::stringstream ss(scoreString);

            ss >> score;
            if (ss.fail()) {
                std::cerr << "Impossible to get score" << std::endl;
            }

            scores.push_back({playerName, score});
        }
    }
    return scores;
}

void arcade::ScoreHandler::saveScore(const std::string playerName)
{
    if (_isSaved == true)
        return;

    const std::string scoreLine(playerName + "=" + std::to_string(_score));
    std::vector<std::string> lines;

    const std::string keyGameName = getKeyGameName();

    if (_fileInput.is_open()) {
        _fileInput.seekg(0, std::fstream::beg);

        std::string line;
        bool keyGameNameInFile = false;

        while (std::getline(_fileInput, line)) {
            lines.push_back(line);
            if (line == keyGameName) {
                keyGameNameInFile = true;
                lines.push_back(scoreLine);
            }
        }

        if (keyGameNameInFile == false) {
            lines.push_back(keyGameName);
            lines.push_back(scoreLine);
        }
    } else {
        lines.push_back(keyGameName);
        lines.push_back(scoreLine);
    }

    std::string fullStringToWrite;
    for (const std::string &line : lines) {
        fullStringToWrite += line + '\n';
    }

    std::ofstream _fileOutput(_fileName);
    _fileOutput << fullStringToWrite;
    _fileOutput.close();

    _isSaved = true;
}

bool arcade::ScoreHandler::getSavedState() const
{
    return _isSaved;
}

void arcade::ScoreHandler::resetSavedState()
{
    _isSaved = false;
}
