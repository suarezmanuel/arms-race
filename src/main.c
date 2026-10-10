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
#include "../include/debug.h"

int main() {

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(700, 700, "raylib basic window");
    SCREEN_WIDTH = GetScreenWidth();
    SCREEN_HEIGHT = GetScreenHeight();

    SetExitKey(KEY_DELETE);
    SetTargetFPS(60);

    RenderTexture2D buffer = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);

    GameState game = {0};
    InitGame(&game);
    Camera2D camera = (Camera2D){(Vector2){0, 0}, (Vector2){0, 0}, 0.0f, 1.0f};
    Camera2D cameraTexture;

    while (!WindowShouldClose()) {

        HandleInputs(&camera, &game);
        GenerateForces(&game.player);
        ApplyForces(&game.player, &game.map);
        Vector2 loss = FocusCamera(&camera, game.mode.type, &game.player);

        BeginDrawing();
        BeginTextureMode(buffer);

        BeginMode2D(camera);
        DrawWorld(&camera, &game);
        EndMode2D();

        DrawUi(&game);
        EndTextureMode();

        cameraTexture.target = (Vector2){0, 0};
        cameraTexture.offset = Vector2Add(camera.offset, loss);
        cameraTexture.rotation = 0.0f;
        cameraTexture.zoom = 1.0f;

        printf("%f %f\n", loss.x, loss.y);

        BeginMode2D(cameraTexture);
        ClearBackground(RAYWHITE);

        Vector2 a = GetScreenToWorld2D((Vector2){0, 0}, cameraTexture);

        DrawTexturePro(buffer.texture,
                       (Rectangle){0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT},
                       (Rectangle){a.x, a.y, SCREEN_WIDTH / cameraTexture.zoom,
                                   SCREEN_HEIGHT / cameraTexture.zoom},
                       (Vector2){0, 0}, 0, WHITE);

        DebugDraw(&cameraTexture);

        EndMode2D();
        EndDrawing();

        game.player.SF = (Vector2){0, 0};
    }
    CloseWindow();
    return 0;
}
