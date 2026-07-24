#pragma once
#include "Constants.hpp"
#include <string>

class SettingsManager {
public:
    SettingsManager();

    void load();
    void save();

    float getVolume() const { return volume; }
    void setVolume(float v) { volume = v; }

    Difficulty getDifficulty() const { return difficulty; }
    void setDifficulty(Difficulty d) { difficulty = d; }
    void cycleDifficulty();

    int getHighScore(Difficulty d) const;
    int getCurrentHighScore() const { return getHighScore(difficulty); }
    void updateHighScore(int score);

private:
    float volume = 80.f;
    Difficulty difficulty = Difficulty::Easy;

    int highScoreEasy = 0;
    int highScoreMedium = 0;
    int highScoreHard = 0;

    const std::string settingsFile = "settings.txt";
    const std::string highScoreFile = "highscore.txt";
};