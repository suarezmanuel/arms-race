#include "math.h"
#include "stddef.h"

#define RAYGUI_IMPLEMENTATION
#include "../include/raylib/raygui.h"
#include "../include/raylib/raylib.h"

#include "../include/camera.h"
#include "../include/drawing.h"

#include "../include/handle_input.h"
#include "../include/physics.h"
#include "../include/structs.h"


int main() {
    
    SetConfigFlags(FLAG_FULLSCREEN_MODE | FLAG_VSYNC_HINT);
    InitWindow(0, 0, "raylib basic window");
    SCREEN_WIDTH = GetScreenWidth();
    SCREEN_HEIGHT = GetScreenHeight();
    
    SetExitKey(KEY_DELETE);
    SetTargetFPS(60);

    RenderTexture2D buffer = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    GameState game = {0};
    init_game(&game);
    Camera2D camera = (Camera2D){(Vector2){0, 0}, (Vector2){0, 0}, 0.0f, 1.0f};

    while (!WindowShouldClose()) {

        handle_inputs(&camera, &game);
        generate_forces(&game.player);
        apply_forces(&game.player, &game.map);
        focus_camera(&camera, game.mode.type, &game.player);

        BeginDrawing();
        BeginTextureMode(buffer);
        BeginMode2D(camera);

        draw_world(&camera, &game);

        EndMode2D();

        draw_ui(&game);

        EndTextureMode();
        DrawTexturePro(
            buffer.texture,
            (Rectangle){0, SCREEN_HEIGHT, SCREEN_WIDTH, -SCREEN_HEIGHT},
            (Rectangle){0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Vector2){0, 0}, 0,
            WHITE);
        EndDrawing();

        game.player.SF = (Vector2){0, 0};
    }
    CloseWindow();
    return 0;
}
