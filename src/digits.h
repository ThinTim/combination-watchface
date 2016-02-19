#pragma once
#include <pebble.h>

typedef struct {
  int width;
  int height;

  int padding_x;
  int padding_y;
} DigitSize;

void draw_digit(GContext *ctx, int digit, GPoint position, DigitSize size, GColor digit_color, GColor background_color);
DigitSize digit_size_new(int width, int height, int padding_x, int padding_y);
DigitSize scale_digit(DigitSize base, int newWidth, int newHeight);
