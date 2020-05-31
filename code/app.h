#ifndef APP_H
#define APP_H

global Platform *app_platform = 0;

#include "utility.cpp"
#include "memory.cpp"
#include "renderer.cpp"

#define PI 3.14159265358979323846
#define MAX_BOUNCE_ANGLE 5*PI/12

struct Player{
    Vec2 pos;
    Vec2 scale;
    Vec4 color;
};

struct Ball{
    Vec2 pos;
    Vec2 scale;
    Vec2 velocity;
    Vec4 color;
    f32 speed;
};

//Block state
enum{
    EMPTY,
    SOLID
};

struct Level{
    u32 blocks;
    Vec2 *positions;
    Vec2 *scales;
    Vec4 *colors;
    u32 *state;
};

struct LevelManager{
    Level *levels;
    Level current_level;
    u32 count;
};

/* App specific state is here, independent of platform */
struct App{
    MemoryArena permanent_arena;
    Renderer renderer;
    Font font;
    Player player;
    Ball ball;
    LevelManager level_manager;
    b32 playing;
};

global App *app = 0;
#endif //APP_H
