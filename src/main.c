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
#include "../include/handle_input.h"

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
