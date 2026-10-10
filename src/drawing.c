#include "../include/drawing.h"
#include "../include/geometry.h"
#include "../include/helpers.h"
#include "../include/terminal.h"

void DrawRec(Tile *tile) {
    DrawRectangleV(REC_TL(tile->rec), REC_SIZE(tile->rec), tile->color);
}

void DrawMap(Camera2D *camera, Map *map) {
    for (int i = 0; i < map->size; i++) {
        DrawRec(&map->tiles[i]);
    }
}

static void DrawWorldGrid(Camera2D *camera) {
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

static void DrawVector(Vector2 start, Vector2 vec, Color color) {

    float vector_length = SnapToPixel(Vector2Length(vec));
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

void DrawHover(Vector2 bottom, Player *player, float font_size) {

    int count = 3;
    Vector2 pos =
        SnapVectorToPixel((Vector2){bottom.x, bottom.y - font_size * count});
    Font font = GetFontDefault();

#define DRAW_LINE(text, color)                                                 \
    do {                                                                       \
        DrawTextEx(font, (text), pos, font_size, 0.05f, (color));              \
        pos.y += font_size;                                                    \
    } while (0)

    DRAW_LINE(TextFormat("pos: %f %f", player->rec.x, player->rec.y), GRAY);
    DRAW_LINE(TextFormat("vel_mag: %f", Vector2Length(player->vel)), BLACK);
    DRAW_LINE(TextFormat("acc_mag: %f", Vector2Length(player->acc)), ORANGE);
#undef DRAW_LINE
}

void DrawDebugInfoPlayer(Player *player) {
    Vector2 start = REC_CENTER(player->rec);
    DrawVector(start, player->vel, BLACK);
    DrawVector(start, player->acc, ORANGE);

    DrawHover(REC_TL(player->rec), player, 0.5);
}

void DrawBackground(Camera2D *camera, GameState *game) {
    ClearBackground(BOUNDS_COLOR);
    DrawRectangle(-TILE_SIDE_LEN * MAX_MAP_TILE_WIDTH * 0.5f,
                  -TILE_SIDE_LEN * MAX_MAP_TILE_HEIGHT * 0.5f,
                  TILE_SIDE_LEN * MAX_MAP_TILE_WIDTH,
                  TILE_SIDE_LEN * MAX_MAP_TILE_HEIGHT, RAYWHITE);
    DrawWorldGrid(camera);
}

void DrawWorldObjects(Camera2D *camera, GameState *game) {
    DrawMap(camera, &game->map);
    DrawRec(&(Tile){game->player.color, game->player.rec});
}

void DrawDebugInfoMap(Map *map) {
    for (int i = 0; i < map->size; i++) {
        Tile tile = map->tiles[i];
        Rectangle rec =
            (Rectangle){tile.rec.x, tile.rec.y + PIXEL, tile.rec.width - PIXEL,
                        tile.rec.height - PIXEL};
        DrawRectangleLinesEx(rec, 0.1, YELLOW);
    }
}

void DrawDebugTools(GameState *game) {
    DrawDebugInfoPlayer(&game->player);
    DrawDebugInfoMap(&game->map);
}

void DrawUi(GameState *game) {
    if (game->mode.type == CONSOLE) {
        TerminalDraw(game);
    }
}

void DrawWorld(Camera2D *camera, GameState *game) {
    DrawBackground(camera, game);
    DrawWorldObjects(camera, game);

    if (game->mode.type == DEBUG) {
        DrawDebugTools(game);
    }
}
