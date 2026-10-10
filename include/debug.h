#ifndef DEBUG_H
#define DEBUG_H

#include "raylib/raylib.h"

void DrawDebugCircle(Vector2 center, float radius, Color color, float duration, bool to_world);

void DrawDebugLine(Vector2 start, Vector2 end, Color color, float duration, bool to_world);

void DrawDebugRect(Rectangle rec, Color color, float duration, bool to_world);

void DrawDebugText(const char* text, Vector2 pos, int font_size, Color color, float duration, bool to_world);

void DebugStep();

#endif