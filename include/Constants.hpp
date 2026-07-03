#pragma once

// ---------------------------------------------------------------------------
// Global, tweakable game constants. Keeping them in one place makes balancing
// the physics (and guaranteeing platform reachability) much easier.
// ---------------------------------------------------------------------------
namespace Constants {

// Window
constexpr unsigned int WINDOW_WIDTH  = 480;
constexpr unsigned int WINDOW_HEIGHT = 720;
constexpr const char*  WINDOW_TITLE  = "Doodle Jump";

// Physics (pixels / second, pixels / second^2)
constexpr float GRAVITY            = 900.f;
constexpr float JUMP_VELOCITY      = -520.f; // normal bounce off a platform
constexpr float SPRING_VELOCITY    = -900.f; // extra-strong bounce off a spring
constexpr float PLAYER_SPEED       = 320.f;  // horizontal speed

// Player
constexpr float PLAYER_WIDTH       = 50.f;
constexpr float PLAYER_HEIGHT      = 50.f;

// Platforms
constexpr float PLATFORM_WIDTH     = 70.f;
constexpr float PLATFORM_HEIGHT    = 20.f;
constexpr float MOVING_PLATFORM_SPEED = 120.f;
constexpr float BREAK_FALL_SPEED   = 260.f; // speed a broken platform falls at

// Spring
constexpr float SPRING_WIDTH       = 18.f;
constexpr float SPRING_HEIGHT      = 18.f;

// Platform generation
// Reachable vertical gap is derived from the jump physics:
//   maxJumpHeight = JUMP_VELOCITY^2 / (2 * GRAVITY)
// We stay safely under that so the game never becomes impossible.
constexpr float MIN_VERTICAL_GAP   = 50.f;
constexpr float MAX_VERTICAL_GAP   = 130.f; // < theoretical max jump height (~150px)
constexpr float MIN_HORIZONTAL_GAP = 0.5f;
constexpr float SPRING_CHANCE      = 0.18f;   // probability a normal platform gets a spring
constexpr float MOVING_CHANCE      = 0.18f;   // probability a generated platform is "moving"
constexpr float BREAKABLE_CHANCE   = 0.32f;   // probability a generated platform is "breakable"

} // namespace Constants
