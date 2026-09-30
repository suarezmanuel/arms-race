#include <math.h>
#define RAYGUI_IMPLEMENTATION

#include "../include/raylib/raygui.h"
#include "../include/raylib/raylib.h"
#include "stddef.h"
#include "stdio.h"

#include "../include/camera.h"
#include "../include/consts.h"
#include "../include/drawing.h"
#include "../include/physics.h"
#include "../include/structs.h"
#include "../include/terminal.h"

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

void handle_inputs(GameState *game) {
    Vector2 mouse_world = GetScreenToWorld2D(GetMousePosition(), game->camera);
    game->camera.zoom = game->mode.zoom;

    switch (game->mode.type) {
    case DEBUG:
    case FREEPLAY: {
        Vector2 mouse_world =
            GetScreenToWorld2D(GetMousePosition(), game->camera);
        Vector2 mouse_tile =
            (Vector2){floor(mouse_world.x / TILE_SIDE_LEN) * TILE_SIDE_LEN,
                      floor(mouse_world.y / TILE_SIDE_LEN) * TILE_SIDE_LEN};
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            append_tile(&game->map,
                        (Tile){BROWN, mouse_tile,
                               (Vector2){TILE_SIDE_LEN, TILE_SIDE_LEN}});
        }
        break;
    }
    case MAPEDIT: {
        Vector2 mouse_world =
            GetScreenToWorld2D(GetMousePosition(), game->camera);
        Vector2 mouse_tile =
            (Vector2){floor(mouse_world.x / TILE_SIDE_LEN) * TILE_SIDE_LEN,
                      floor(mouse_world.y / TILE_SIDE_LEN) * TILE_SIDE_LEN};
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            append_tile(&game->map,
                        (Tile){BROWN, mouse_tile,
                               (Vector2){TILE_SIDE_LEN, TILE_SIDE_LEN}});
        }
        break;
    }
    case CONSOLE: {
        // append all keys to text buffer
        terminal_handle_inputs();

        if (IsKeyDown(KEY_ESCAPE)) {
            game->mode = game_modes[FREEPLAY];
        }

        return;
    }
    }

    for (int i = 0; i < GAME_MODE_COUNT; i++) {
        if (IsKeyDown(game_modes[i].toggle_key)) {
            game->mode = game_modes[i];
            break;
        }
    }
}

void register_cvars(GameState *game) {
    register_cvar(VECTOR2, "player.pos", &game->player.pos);
    register_cvar(VECTOR2, "player.vel", &game->player.vel);
    register_cvar(VECTOR2, "player.acc", &game->player.acc);
    register_cvar(VECTOR2, "player.SF", &game->player.SF);
    register_cvar(VECTOR2, "player.size", &game->player.size);
    register_cvar(FLOAT, "player.drag_const", &game->player.drag_const);
    register_cvar(FLOAT, "player.speed", &game->player.speed);
    register_cvar(FLOAT, "player.m", &game->player.m);
}

void draw_background(GameState *game) {
    ClearBackground(BOUNDS_COLOR);
    DrawRectangle(-TILE_SIDE_LEN * MAX_MAP_TILE_WIDTH * 0.5f,
                  -TILE_SIDE_LEN * MAX_MAP_TILE_HEIGHT * 0.5f,
                  TILE_SIDE_LEN * MAX_MAP_TILE_WIDTH,
                  TILE_SIDE_LEN * MAX_MAP_TILE_HEIGHT, RAYWHITE);

    draw_grid(&game->camera);
}

void draw_world_objects(GameState *game) {

    char buffer[100];
    snprintf(buffer, sizeof(buffer), "Game Mode: %s", game->mode.name);
    DrawTextEx(GetFontDefault(), buffer, (Vector2){0, 0}, 0.5, 0.05, BLACK);

    draw_map(&game->camera, &game->map);
    draw_player(&game->player);
}

void draw_debug_tools(GameState *game) {
    draw_debug_info_player(&game->player);
}

void draw_ui(GameState *game) {
    if (game->mode.type == CONSOLE) {
        terminal_draw(game);
    }
}

int main() {
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_VSYNC_HINT);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib basic window");
    SetExitKey(KEY_DELETE);
    SetTargetFPS(60);
    GameState game = {0};
    init_game(&game);

    register_cvars(&game);

    while (!WindowShouldClose()) {
        BeginDrawing();

        handle_inputs(&game);

        generate_forces(&game.player, game.dt);
        apply_forces(&game.player, game.dt);
        solve_collisions(&game.player, &game.map, game.dt);

        focus_camera(&game.camera, &game);

        BeginMode2D(game.camera);

        draw_background(&game);
        draw_world_objects(&game);

        if (game.mode.type == DEBUG) {
            draw_debug_tools(&game);
        }

        EndMode2D();

        draw_ui(&game);

        EndDrawing();
        game.player.SF = (Vector2){0, 0};
    }
    CloseWindow();
    return 0;
}
