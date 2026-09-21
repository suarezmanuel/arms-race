#ifndef STRUCTS_H
#define STRUCTS_H

#include "raylib.h"
#include "raymath.h"
#include "consts.h"

typedef struct Player {
    Vector2 pos;
    Vector2 vel;
    Vector2 acc;
    Vector2 SF;
    float drag_const;
    float speed;
    float m;
    Vector2 size;
    Color color;
} Player;

typedef struct Tile {
    Color color;
    Vector2 pos;
    Vector2 size;
} Tile;

typedef struct Map {
    Tile tiles[MAX_MAP_SIZE];
    int size;
} Map;

typedef enum GameMode {
    FREEPLAY,
    MAPEDIT,
} GameMode;

extern const int ToggleKeys [10];
extern const int GameModeKeyMapping [10];
extern const char GameModeStringMapping [10][50];

typedef struct GameState {
    Map map;
    Camera2D camera;
    Player player;
    float dt;
    GameMode mode;
} GameState;

void append_tile(Map *map, Tile tile);

void init_player(Player *player);

void init_map(Map *map);

void init_game(GameState *game);

#endif