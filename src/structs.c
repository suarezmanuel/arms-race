#include "../include/structs.h"
#include "../include/cvars.h"
#include "../include/geometry.h"
#include "../include/handle_input.h"
#include "stddef.h"
#include "string.h"

const GameMode game_modes[GAME_MODE_COUNT] = {
    {FREEPLAY, "Free Play", KEY_ONE, METER, HandleInputFreeplay},
    {MAPEDIT, "Map Edit", KEY_TWO, METER, HandleInputFreeplay},
    {CONSOLE, "Console", KEY_GRAVE, METER, HandleInputConsole},
    {DEBUG, "Debug", KEY_THREE, METER, HandleInputFreeplay}};

const char var_type_names[VAR_TYPE_COUNT][VAR_BUF_LEN] = {"vector2", "float"};
VAR_TYPE var_type_names_mapping[VAR_TYPE_COUNT] = {VECTOR2, FLOAT};

CVar cvars[MAX_CVAR_COUNT] = {0};
size_t cvars_count = 0;

void RegisterCvar(VAR_TYPE type, const char *name, void *addr) {
    if (cvars_count == MAX_CVAR_COUNT) {
        return;
    }
    CVar *cvar = &cvars[cvars_count++];
    cvar->type = type;
    cvar->addr = addr;
    strncpy(cvar->name, TextToLower(name), VAR_BUF_LEN);
    cvar->name[VAR_BUF_LEN - 1] = '\0';
}

void SetCvar(const char *name, void *values) {
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

void InitPlayer(Player *player) {
    player->speed = SPEED;
    player->color = RED;
    player->m = 1; // kg
    player->rec = (Rectangle){SCREEN_WIDTH / (METER * 2.0f), SCREEN_HEIGHT / (METER * 2.0f), 2, 2};
    player->drag_const = DRAG_CONST;
}

void AppendTile(Map *map, Tile tile) {
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

void InitMap(Map *map) {

    Vector2 tl =
        (Vector2){-MAX_MAP_TILE_WIDTH * 0.5f, -MAX_MAP_TILE_HEIGHT * 0.5f};
    Vector2 br = Vector2Scale(tl, -1);

    AppendTile(map, (Tile){BROWN, (Rectangle){tl.x - 1, tl.y, 1,
                                               MAX_MAP_TILE_HEIGHT}});
    AppendTile(
        map, (Tile){BROWN, (Rectangle){tl.x, tl.y - 1, MAX_MAP_TILE_WIDTH, 1}});
    AppendTile(map,
                (Tile){BROWN, (Rectangle){tl.x, br.y, MAX_MAP_TILE_WIDTH, 1}});
    AppendTile(map,
                (Tile){BROWN, (Rectangle){br.x, tl.y, 1, MAX_MAP_TILE_HEIGHT}});
}

void InitGame(GameState *game) {
    InitMap(&game->map);
    InitPlayer(&game->player);

    RegisterCvars(game);

    game->mode = game_modes[FREEPLAY];
}
