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

void draw_player(Player *player) {
    draw_tile(&(Tile){
        player->color,
        (Vector2){snap_to_pixel(player->pos.x), snap_to_pixel(player->pos.y)},
        player->size});
}

static void draw_vector(Vector2 start, Vector2 vec, Color color) {
    float line_width = 10 / METER;
    Vector2 end = Vector2Add(start, vec);

    float vector_length = Vector2Length(vec);
    if (vector_length < 0.001f) {
        DrawLineEx(start, end, line_width, color);
        return;
    }

    Vector2 direction = Vector2Scale(vec, 1.0f / vector_length);
    Vector2 perpendicular = (Vector2){-direction.y, direction.x};
    float head_length = vector_length * 0.4f;
    if (head_length > line_width * 4)
        head_length = line_width * 4;
    Vector2 base = Vector2Subtract(end, Vector2Scale(direction, head_length));
    Vector2 half_width = Vector2Scale(perpendicular, head_length * 1.5f / 4.0f);
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