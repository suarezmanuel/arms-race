#ifndef STRUCTS_H
#define STRUCTS_H

#include <stddef.h> 
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "consts.h"

typedef struct Player {
    Rectangle rec;
    Vector2 vel;
    Vector2 acc;
    Vector2 SF;
    float drag_const;
    float speed;
    float m;
    Color color;
} Player;

typedef struct Tile {
    Color color;
    Rectangle rec;
} Tile;

typedef struct Map {
    Tile tiles[MAX_MAP_SIZE];
    int size;
} Map;

typedef enum GameModeType {
    FREEPLAY,
    MAPEDIT,
    CONSOLE,
    DEBUG,
} GameModeType;

typedef GameModeType HandleInputFunction(Map* map, Camera2D* camera, GameModeType initialMode);

typedef struct GameMode {
    GameModeType type;
    char name[MAX_NAME_LEN];
    int toggle_key;
    float zoom;
    HandleInputFunction* handle_input_function;
} GameMode;

typedef enum VAR_TYPE {
    VECTOR2,
    FLOAT,
    VAR_TYPE_COUNT
} VAR_TYPE;

extern const char var_type_names[VAR_TYPE_COUNT][VAR_BUF_LEN];
extern VAR_TYPE var_type_names_mapping[VAR_TYPE_COUNT];

typedef struct CVar {
    VAR_TYPE type;
    char name[VAR_BUF_LEN];
    void* addr;
} CVar;

#define GAME_MODE_COUNT 4
extern const GameMode game_modes[GAME_MODE_COUNT];
#define MAX_CVAR_COUNT 1024
extern size_t cvars_count;
extern CVar cvars [MAX_CVAR_COUNT];

typedef struct GameState {
    Map map;
    Player player;
    GameMode mode;
} GameState;

void append_tile(Map *map, Tile tile);

void init_player(Player *player);

void init_map(Map *map);

void init_game(GameState *game);

void register_cvar(VAR_TYPE type, const char* name, void* addr);

void set_cvar(const char* name, void* values);

#endif