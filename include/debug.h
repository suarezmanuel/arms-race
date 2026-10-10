#ifndef DEBUG_H
#define DEBUG_H

#include "raylib/raylib.h"

void RegisterDebugCircle(Vector2 center, float radius, Color color, float duration, bool to_world);

void RegisterDebugLine(Vector2 start, Vector2 end, Color color, float duration, bool is_vector, bool to_world);

void RegisterDebugRect(Rectangle rec, Color color, float duration, bool to_world);

void RegisterDebugText(const char* text, Vector2 pos, float font_size, Color color, float duration, bool to_world);

void DebugDraw(Camera2D *camera);

#endif