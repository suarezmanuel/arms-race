#ifndef DEBUG_H
#define DEBUG_H

#include "raylib/raylib.h"

void register_debug_circle(Vector2 center, float radius, Color color, float duration);

void register_debug_line(Vector2 start, Vector2 end, Color color, float duration);

void register_debug_rect(Rectangle rec, Color color, float duration);

void register_debug_text(const char* text, Vector2 pos, int font_size, Color color, float duration);

void debug_step();

#endif