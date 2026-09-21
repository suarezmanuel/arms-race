#include "../include/drawing.h"

void draw_tile(Tile *tile) {
    DrawRectangle(tile->pos.x, tile->pos.y, tile->size.x, tile->size.y,
                  tile->color);
}

void draw_map(Camera2D *camera, Map *map) {
    for (int i = 0; i < map->size; i++) {
        draw_tile(&map->tiles[i]);
    }
}

void draw_player(Player *player) {
    draw_tile(&(Tile){player->color, (Vector2){truncf(player->pos.x), truncf(player->pos.y)}, player->size});
}