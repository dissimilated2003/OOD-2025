#pragma once

#include "Image.h"

/*
для рисования используется алгоритм Брезенхэма

https://ru.wikipedia.org/wiki/Алгоритм_Брезенхэма
*/

void DrawLine(Image& image, Point from, Point to, uint32_t color);
void DrawCircle(Image& image, Point center, int radius, uint32_t color);
void FillCircle(Image& image, Point center, int radius, uint32_t color);