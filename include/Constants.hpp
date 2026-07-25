#pragma once

namespace Constants {

// Window
constexpr unsigned int WINDOW_WIDTH  = 480;
constexpr unsigned int WINDOW_HEIGHT = 720;
constexpr const char*  WINDOW_TITLE  = "Doodle Jump";

// Physics
constexpr float GRAVITY            = 900.f;
constexpr float JUMP_VELOCITY      = -520.f; 
constexpr float SPRING_VELOCITY    = -900.f; 
constexpr float MONSTER_BOUNCE_VEL = -750.f; // Strong bounce off monster
constexpr float PLAYER_SPEED       = 320.f;  

// Player
constexpr float PLAYER_WIDTH       = 50.f;
constexpr float PLAYER_HEIGHT      = 50.f;
constexpr float NOSE_WIDTH  = 20.f;
constexpr float NOSE_HEIGHT = 20.f;

// Platforms
constexpr float PLATFORM_WIDTH     = 70.f;
constexpr float PLATFORM_HEIGHT    = 20.f;
constexpr float BREAK_FALL_SPEED   = 260.f; 

// Base Generation Probabilities & Limits
constexpr float SPRING_CHANCE         = 0.15f;
constexpr float BREAKABLE_CHANCE      = 0.20f;
constexpr float MIN_VERTICAL_GAP      = 30.f;
constexpr float MAX_VERTICAL_GAP      = 120.f; 

// Moving Platforms Speed & Chance
constexpr float MOVING_CHANCE         = 0.25f;
constexpr float MOVING_PLATFORM_SPEED_EASY   = 80.f;
constexpr float MOVING_PLATFORM_SPEED_MEDIUM = 130.f;
constexpr float MOVING_PLATFORM_SPEED_HARD   = 180.f;

// Monster Spawn Chances & HP by Difficulty
constexpr float MONSTER_SPAWN_CHANCE_EASY   = 0.10f;
constexpr float MONSTER_SPAWN_CHANCE_MEDIUM = 0.25f;
constexpr float MONSTER_SPAWN_CHANCE_HARD   = 0.40f;

constexpr int MONSTER_HP_EASY   = 1;
constexpr int MONSTER_HP_MEDIUM = 2;
constexpr int MONSTER_HP_HARD   = 3;

// Hole Spawn Chance (Hard Only)
constexpr float HOLE_SPAWN_CHANCE_HARD = 0.15f;
constexpr float BIG_HOLE_SPWAN_CHANCE_HARD = 0.08f;

// Entities Stats
constexpr float BULLET_SPEED       = 700.f;
constexpr float BULLET_WIDTH       = 12.f;
constexpr float BULLET_HEIGHT      = 12.f;

constexpr float MONSTER_WIDTH      = 40.f;
constexpr float MONSTER_HEIGHT     = 40.f;
constexpr float MONSTER_SPEED      = 70.f;

constexpr float HOLE_SMALL_SIZE    = 70.f;
constexpr float HOLE_LARGE_SIZE    = 140.f;

constexpr float PLAYER_SHRINK_SPEED = 60.f; // Pixels shrink per sec

constexpr float SPRING_WIDTH       = 18.f;
constexpr float SPRING_HEIGHT      = 18.f;
} // namespace Constants

enum class Difficulty { Easy, Medium, Hard };