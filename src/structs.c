#include "../include/structs.h"

const int ToggleKeys[10] = {KEY_ONE, KEY_TWO,   KEY_THREE, KEY_FOUR, KEY_FIVE,
                            KEY_SIX, KEY_SEVEN, KEY_EIGHT, KEY_NINE, KEY_ZERO};
const int GameModeKeyMapping[10] = {FREEPLAY, MAPEDIT, -1, -1, -1,
                                    -1,       -1,      -1, -1, -1};
const char GameModeStringMapping[10][50] = {
    "Free Play", "Map Editing", -1, -1, -1, -1, -1, -1, -1, -1};

void init_player(Player *player) {
    player->speed = 30000;
    player->size = (Vector2){100, 100};
    player->color = RED;
    player->m = 1;
    player->pos = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    player->drag_const = 12;
}

void append_tile(Map *map, Tile tile) {
    if (map->size == MAX_MAP_SIZE) {
        return;
    }
    for (int i=0; i < map->size; i++) {
        if (Vector2Equals(map->tiles[i].size, tile.size) && Vector2Equals(map->tiles[i].pos, tile.pos)) {
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
    game->dt = 1.0f / 200.0f;
    game->camera.target = (Vector2){0, 0};
    game->camera.offset = (Vector2){0, 0};
    game->camera.zoom = 5.0f;
    game->camera.rotation = 0.0f;
}