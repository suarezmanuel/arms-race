#include "../include/structs.h"
#include "../include/handle_input.h"
#include "../include/geometry.h"
#include "../include/cvars.h"
#include "../include/geometry.h"
#include "stddef.h"
#include "string.h"

const GameMode game_modes[GAME_MODE_COUNT] = {
    {FREEPLAY, "Free Play", KEY_ONE, METER, handle_input_freeplay},
    {MAPEDIT, "Map Edit", KEY_TWO, METER, handle_input_freeplay},
    {CONSOLE, "Console", KEY_GRAVE, METER, handle_input_console},
    {DEBUG, "Debug", KEY_THREE, METER, handle_input_freeplay}};

const char var_type_names[VAR_TYPE_COUNT][VAR_BUF_LEN] = {"vector2", "float"};
VAR_TYPE var_type_names_mapping[VAR_TYPE_COUNT] = {VECTOR2, FLOAT};

CVar cvars[MAX_CVAR_COUNT] = {0};
size_t cvars_count = 0;

void register_cvar(VAR_TYPE type, const char *name, void *addr) {
    if (cvars_count == MAX_CVAR_COUNT) {
        return;
    }
    CVar *cvar = &cvars[cvars_count++];
    cvar->type = type;
    cvar->addr = addr;
    strncpy_s(cvar->name, VAR_BUF_LEN, TextToLower(name), VAR_BUF_LEN);
    cvar->name[VAR_BUF_LEN-1]= '\0';
}

void set_cvar(const char *name, void *values) {
    CVar selected;
    for (size_t i = 0; i < cvars_count; i++) {
        if (!strcmp(cvars[i].name, name)) {
            selected = cvars[i];
            break;
        }
    }

    switch (selected.type) {
    case VECTOR2: {
        size_t offsetx = offsetof(Vector2, x) / sizeof(float);
        size_t offsety = offsetof(Vector2, y) / sizeof(float);
        memcpy((float *)selected.addr + offsetx, values, sizeof(float));
        memcpy((float *)selected.addr + offsety, (float *)values + 1,
               sizeof(float));
        break;
    }
    case FLOAT: {
        memcpy(selected.addr, values, sizeof(float));
        break;
    }
    case VAR_TYPE_COUNT: {
        break;
    }
    }
}

void init_player(Player *player) {
    player->speed = SPEED;
    player->color = RED;
    player->m = 1; // kg
    player->rec = (Rectangle){SCREEN_WIDTH / (METER * 2.0f), SCREEN_HEIGHT / (METER * 2.0f), 2, 2};
    player->drag_const = DRAG_CONST;
}

void append_tile(Map *map, Tile tile) {
    if (map->size == MAX_MAP_SIZE) {
        return;
    }
    for (int i = 0; i < map->size; i++) {
        if (REC_EQUAL(map->tiles[i].rec, tile.rec)) {
            return;
        }
    }
    map->tiles[map->size] = tile;
    map->size++;
}

void init_map(Map *map) {

    Vector2 tl = (Vector2){-MAX_MAP_TILE_WIDTH * 0.5f, -MAX_MAP_TILE_HEIGHT * 0.5f};
    Vector2 br = Vector2Scale(tl, -1);

    append_tile(map, (Tile){BROWN, (Rectangle){tl.x - 1, tl.y, 1, MAX_MAP_TILE_HEIGHT}});
    append_tile(map, (Tile){BROWN, (Rectangle){tl.x, tl.y - 1, MAX_MAP_TILE_WIDTH, 1}});
    append_tile(map, (Tile){BROWN, (Rectangle){tl.x, br.y, MAX_MAP_TILE_WIDTH, 1}});
    append_tile(map, (Tile){BROWN, (Rectangle){br.x, tl.y, 1, MAX_MAP_TILE_HEIGHT}});
}

void init_game(GameState *game) {
    init_map(&game->map);
    init_player(&game->player);

    register_cvars(game);
    
    game->mode = game_modes[FREEPLAY];
}