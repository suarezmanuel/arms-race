#include "../include/drawing.h"
#include "../include/helpers.h"
#include "stdio.h"

void draw_tile(Tile *tile) {
    DrawRectangleV((Vector2){tile->pos.x, tile->pos.y},
                   (Vector2){tile->size.x, tile->size.y}, tile->color);
}

void draw_map(Camera2D *camera, Map *map) {
    for (int i = 0; i < map->size; i++) {
        draw_tile(&map->tiles[i]);
    }
}

void draw_grid(Camera2D *camera) {
    Vector2 world_tl = Vector2Max(
        GetScreenToWorld2D((Vector2){-1 * METER, -1 * METER}, *camera),
        ((Vector2){-MAX_MAP_TILE_WIDTH * 0.5f, -MAX_MAP_TILE_HEIGHT * 0.5f}));
    Vector2 world_br = Vector2Min(
        GetScreenToWorld2D(
            (Vector2){SCREEN_WIDTH + METER, SCREEN_HEIGHT + METER}, *camera),
        ((Vector2){MAX_MAP_TILE_WIDTH * 0.5f, MAX_MAP_TILE_HEIGHT * 0.5f}));

    int vlines_count = world_br.x - world_tl.x;
    int hlines_count = world_br.y - world_tl.y;
    // ceil to make sure that start is inside the camera view and we dont draw
    // unseen lines
    float vlines_start = ceil(world_tl.x);
    float hlines_start = ceil(world_tl.y);

    for (float i = vlines_start; i <= vlines_start + vlines_count; i += 1) {
        DrawLine(i, world_tl.y, i, world_br.y, GRAY);
    }

    for (float i = hlines_start; i <= hlines_start + hlines_count; i += 1) {
        DrawLine(world_tl.x, i, world_br.x, i, GRAY);
    }
}

void draw_player(Player *player) {
    draw_tile(&(Tile){
        player->color,
        (Vector2){snap_to_pixel(player->pos.x), snap_to_pixel(player->pos.y)},
        player->size});
}

static void draw_vector(Vector2 start, Vector2 vec, Color color) {
    
    float vector_length = snap_to_pixel(Vector2Length(vec));
    float head_height = fmin(vector_length / 2, 0.4f);
    float head_width = head_height;
    float line_width = head_width * 0.33f;

    Vector2 end = Vector2Add(start, vec);
    Vector2 direction = Vector2Normalize(vec);
    Vector2 perpendicular = (Vector2){-direction.y, direction.x};
    Vector2 base = Vector2Subtract(end, Vector2Scale(direction, head_height));
    Vector2 half_width = Vector2Scale(perpendicular, head_width * 0.5f);
    DrawLineEx(start, base, line_width, color);
    DrawTriangle(end, Vector2Subtract(base, half_width),
                 Vector2Add(base, half_width), color);
}

static void draw_hover(Vector2 bottom, Player *player, float font_size) {

    int count = 3;
    Vector2 pos =
        snap_vector_to_pixel((Vector2){bottom.x, bottom.y - font_size * count});
    Font font = GetFontDefault();

#define DRAW_LINE(text, color)                                                 \
    do {                                                                       \
        DrawTextEx(font, (text), pos, font_size, 0.05f, (color));              \
        pos.y += font_size;                                                    \
    } while (0)

    DRAW_LINE(TextFormat("pos: %f %f", player->pos.x, player->pos.y), GRAY);
    DRAW_LINE(TextFormat("vel_mag: %f", Vector2Length(player->vel)), BLACK);
    DRAW_LINE(TextFormat("acc_mag: %f", Vector2Length(player->acc)), ORANGE);
#undef DRAW_LINE
}

void draw_debug_info_player(Player *player) {
    Vector2 start = Vector2Add(player->pos, Vector2Scale(player->size, 0.5));
    draw_vector(start, player->vel, BLACK);
    draw_vector(start, player->acc, ORANGE);

    draw_hover(player->pos, player, 0.5);
}