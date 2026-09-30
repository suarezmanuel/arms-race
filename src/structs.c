#include "../include/structs.h"
#include "stddef.h"
#include "string.h"
#include "../include/handle_input.h"


const GameMode game_modes[GAME_MODE_COUNT] = {{FREEPLAY, "Free Play", KEY_ONE, METER, handle_input_freeplay},
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
    player->size = (Vector2){2, 2};
    player->color = RED;
    player->m = 1; // kg
    player->pos = (Vector2){SCREEN_WIDTH / (METER * 2.0f), SCREEN_HEIGHT / (METER * 2.0f)};
    player->drag_const = DRAG_CONST;
}

void append_tile(Map *map, Tile tile) {
    if (map->size == MAX_MAP_SIZE) {
        return;
    }
    for (int i = 0; i < map->size; i++) {
        if (Vector2Equals(map->tiles[i].size, tile.size) &&
            Vector2Equals(map->tiles[i].pos, tile.pos)) {
            return;
        }
    }
    map->tiles[map->size] = tile;
    map->size++;
}

void init_map(Map *map) {
    append_tile(map,
                (Tile){BROWN, (Vector2){10 * TILE_SIDE_LEN, 10 * TILE_SIDE_LEN},
                       (Vector2){TILE_SIDE_LEN, TILE_SIDE_LEN}});

    Vector2 tl = (Vector2){-MAX_MAP_TILE_WIDTH * TILE_SIDE_LEN * 0.5f,
                           -MAX_MAP_TILE_HEIGHT * TILE_SIDE_LEN * 0.5f};
    Vector2 br = Vector2Scale(tl, -1);

    append_tile(map, (Tile){BROWN, (Vector2){tl.x - TILE_SIDE_LEN, tl.y},
                            (Vector2){TILE_SIDE_LEN,
                                      MAX_MAP_TILE_HEIGHT * TILE_SIDE_LEN}});
    append_tile(map, (Tile){BROWN, (Vector2){tl.x, tl.y - TILE_SIDE_LEN},
                            (Vector2){MAX_MAP_TILE_WIDTH * TILE_SIDE_LEN,
                                      TILE_SIDE_LEN}});
    append_tile(map, (Tile){BROWN, (Vector2){tl.x, br.y},
                            (Vector2){MAX_MAP_TILE_WIDTH * TILE_SIDE_LEN,
                                      TILE_SIDE_LEN}});
    append_tile(map, (Tile){BROWN, (Vector2){br.x, tl.y},
                            (Vector2){TILE_SIDE_LEN,
                                      MAX_MAP_TILE_HEIGHT * TILE_SIDE_LEN}});
}

void init_game(GameState *game) {
    init_map(&game->map);
    init_player(&game->player);
    
    game->dt = 1.0f / 60.0f;
    game->camera.target = (Vector2){0, 0};
    game->camera.offset = (Vector2){0, 0};
    game->camera.zoom = 1.0f;
    game->camera.rotation = 0.0f;

    game->mode = game_modes[FREEPLAY];
}