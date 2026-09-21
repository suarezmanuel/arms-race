#define RAYGUI_IMPLEMENTATION
#include "raygui.h"
#include "raylib.h"

#include "../include/consts.h"
#include "../include/drawing.h"
#include "../include/physics.h"
#include "../include/structs.h"

void focus_on_map_tiling(Camera2D *camera) {
    // i want the blocks on -50, 50 to be visible
    camera->target = (Vector2){0, 0};
    camera->zoom = fmin(SCREEN_WIDTH / (TILE_SIDE_LEN * MAX_MAP_TILE_WIDTH),
                        SCREEN_HEIGHT / (TILE_SIDE_LEN * MAX_MAP_TILE_HEIGHT));
    camera->offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};

}

void draw_grid(Camera2D *camera) {
    Vector2 world_tl = Vector2Max(GetScreenToWorld2D((Vector2){0, 0}, *camera), ((Vector2){-MAX_MAP_TILE_WIDTH * TILE_SIDE_LEN * 0.5f, -MAX_MAP_TILE_HEIGHT * TILE_SIDE_LEN * 0.5f}));
    Vector2 world_br = Vector2Min(GetScreenToWorld2D((Vector2){SCREEN_WIDTH, SCREEN_HEIGHT}, *camera), ((Vector2){MAX_MAP_TILE_WIDTH * TILE_SIDE_LEN * 0.5f, MAX_MAP_TILE_HEIGHT * TILE_SIDE_LEN * 0.5f}));;

    int vlines_count = trunc((world_br.x - world_tl.x) / TILE_SIDE_LEN);
    int hlines_count = trunc((world_br.y - world_tl.y) / TILE_SIDE_LEN);
    // ceil to make sure that start is inside the camera view and we dont draw
    // unseen lines
    float vlines_start = (ceil(world_tl.x / TILE_SIDE_LEN)) * TILE_SIDE_LEN;
    float hlines_start = (ceil(world_tl.y / TILE_SIDE_LEN)) * TILE_SIDE_LEN;

    for (float i = vlines_start;
         i <= vlines_start + vlines_count * TILE_SIDE_LEN; i += TILE_SIDE_LEN) {
        DrawLine(i, world_tl.y, i, world_br.y, GRAY);
    }

    for (float i = hlines_start; i <= hlines_start + hlines_count * TILE_SIDE_LEN; i += TILE_SIDE_LEN) {
        DrawLine(world_tl.x, i, world_br.x, i, GRAY);
    }
}

void focus_on_player(Camera2D *camera, Vector2 target) {
    camera->target = (Vector2){(int)target.x, (int)target.y};
    camera->offset = (Vector2){SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f};
    camera->zoom = 1.0f;
}

void focus_camera(Camera2D *camera, GameState *game) {
    switch (game->mode) {
    case FREEPLAY: {
        focus_on_player(
            camera, (Vector2){game->player.pos.x + game->player.size.x / 2.0f,
                              game->player.pos.y + game->player.size.y / 2.0f});
        break;
        // game->camera.target = (Vector2){0, 0};
        // game->camera.offset = (Vector2){0, 0};
        // game->camera.zoom = 5.0f;
        // game->camera.rotation = 0.0f;
        break;
    };
    case MAPEDIT: {
        focus_on_map_tiling(&game->camera);
        break;
    }
    }
}

void handle_inputs(GameState *game) {
    Vector2 mouse_world = GetScreenToWorld2D(GetMousePosition(), game->camera);

    switch (game->mode) {
    case FREEPLAY: {
        Vector2 v_move = (Vector2){IsKeyDown(KEY_D) - IsKeyDown(KEY_A),
                                   IsKeyDown(KEY_S) - IsKeyDown(KEY_W)};
        game->player.SF = Vector2Add(game->player.SF,
                                     Vector2Scale(v_move, game->player.speed));
                                           Vector2 mouse_world = GetScreenToWorld2D(GetMousePosition(), game->camera);
        Vector2 mouse_tile = (Vector2){floor(mouse_world.x / TILE_SIDE_LEN) * TILE_SIDE_LEN, floor(mouse_world.y / TILE_SIDE_LEN) * TILE_SIDE_LEN};
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            append_tile(&game->map, (Tile){BROWN, mouse_tile, (Vector2){TILE_SIDE_LEN, TILE_SIDE_LEN}});
        }
        break;
    }
    case MAPEDIT: {
        Vector2 mouse_world = GetScreenToWorld2D(GetMousePosition(), game->camera);
        Vector2 mouse_tile = (Vector2){floor(mouse_world.x / TILE_SIDE_LEN) * TILE_SIDE_LEN, floor(mouse_world.y / TILE_SIDE_LEN) * TILE_SIDE_LEN};
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            append_tile(&game->map, (Tile){BROWN, mouse_tile, (Vector2){TILE_SIDE_LEN, TILE_SIDE_LEN}});
        }
        break;
    }
    }

    for (int i = 0; i < sizeof(ToggleKeys) / sizeof(ToggleKeys[0]); i++) {
        if (IsKeyDown(ToggleKeys[i])) {
            game->mode = GameModeKeyMapping[i];
            break;
        }
    }
}

int main() {
    GameState game = {0};
    init_game(&game);
    SetConfigFlags(FLAG_WINDOW_UNDECORATED | FLAG_VSYNC_HINT); 
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "raylib basic window");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();

        
        handle_inputs(&game);

        generate_forces(&game.player, game.dt);
        apply_forces(&game.player, game.dt);
        solve_collisions(&game.player, &game.map, game.dt);

        focus_camera(&game.camera, &game);

        BeginMode2D(game.camera);
        ClearBackground(BOUNDS_COLOR);

        DrawRectangle(-TILE_SIDE_LEN * MAX_MAP_TILE_WIDTH  * 0.5f,-TILE_SIDE_LEN * MAX_MAP_TILE_HEIGHT * 0.5f, 
                      TILE_SIDE_LEN * MAX_MAP_TILE_WIDTH,
                      TILE_SIDE_LEN * MAX_MAP_TILE_HEIGHT, RAYWHITE);

        draw_grid(&game.camera);
        draw_map(&game.camera, &game.map);

        char buffer[100];
        snprintf(buffer, sizeof(buffer), "Game Mode: %s",
                 GameModeStringMapping[game.mode]);
        DrawText(buffer, 20, 20, 20, BLACK);
        draw_player(&game.player);

        game.player.SF = (Vector2){0, 0};

        EndMode2D();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
