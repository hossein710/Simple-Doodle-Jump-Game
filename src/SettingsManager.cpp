#include "SettingsManager.hpp"
#include <fstream>
#include <algorithm>

SettingsManager::SettingsManager() {
    load();
}

void SettingsManager::cycleDifficulty() {
    if (difficulty == Difficulty::Easy) difficulty = Difficulty::Medium;
    else if (difficulty == Difficulty::Medium) difficulty = Difficulty::Hard;
    else difficulty = Difficulty::Easy;
}

int SettingsManager::getHighScore(Difficulty d) const {
    switch (d) {
        case Difficulty::Easy:   return highScoreEasy;
        case Difficulty::Medium: return highScoreMedium;
        case Difficulty::Hard:   return highScoreHard;
    }
    return 0;
}

void SettingsManager::updateHighScore(int score) {
    switch (difficulty) {
        case Difficulty::Easy:   highScoreEasy = std::max(highScoreEasy, score); break;
        case Difficulty::Medium: highScoreMedium = std::max(highScoreMedium, score); break;
        case Difficulty::Hard:   highScoreHard = std::max(highScoreHard, score); break;
    }
}

void SettingsManager::load() {
    // 1. High scores
    std::ifstream hs(highScoreFile);
    if (hs.is_open()) {
        hs >> highScoreEasy >> highScoreMedium >> highScoreHard;
    }

    // 2. Settings
    std::ifstream st(settingsFile);
    if (st.is_open()) {
        int diffInt = 0;
        st >> volume >> diffInt;
        difficulty = static_cast<Difficulty>(diffInt);
    }
}

void SettingsManager::save() {
    std::ofstream hs(highScoreFile);
    if (hs.is_open()) {
        hs << highScoreEasy << " " << highScoreMedium << " " << highScoreHard;
    }

    std::ofstream st(settingsFile);
    if (st.is_open()) {
        st << volume << " " << static_cast<int>(difficulty);
    }
}