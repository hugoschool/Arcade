#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace arcade {
    class ScoreHandler {
        public:
            ScoreHandler() = delete;
            ScoreHandler(const std::string gameName);

            ~ScoreHandler();

            void addScore(std::size_t plusScore);
            std::size_t getScore() const;
            void resetScore();

            // Pair of player names and scores
            std::vector<std::pair<const std::string, const std::size_t>> loadScores();

            void saveScore(const std::string playerName);

            bool getSavedState() const;
            void resetSavedState();

        private:
            const std::string _fileName;
            std::ifstream _fileInput;

            const std::string _gameName;
            std::size_t _score;

            bool _isSaved;

            bool isKeyInFile(const std::string &key);

            std::string getKeyGameName() const;
    };
}
