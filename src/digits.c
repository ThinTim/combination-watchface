#include <pebble.h>
#include <stdbool.h>
#include <math.h>
#include "digits.h"

const int DIGIT_DATA_HEIGHT = 12;
const int DIGIT_DATA_WIDTH = 8;

static GRect scale_cutout(GRect cutout, GRect template) {
  int new_origin_x = template.origin.x + round(((float)cutout.origin.x/DIGIT_DATA_WIDTH) * template.size.w);
  int new_origin_y = template.origin.y + round(((float)cutout.origin.y/DIGIT_DATA_HEIGHT) * template.size.h);

  int new_width = round(((float)cutout.size.w/DIGIT_DATA_WIDTH) * template.size.w);
  int new_height = round(((float)cutout.size.h/DIGIT_DATA_HEIGHT) * template.size.h);

  return GRect(new_origin_x, new_origin_y, new_width, new_height);
}

static void draw_rect(GContext *ctx, GRect rect) {
  graphics_fill_rect(ctx, rect, 0, GCornerNone);
}

static void draw_digit_in_rect(GContext *ctx, int digit, GRect rect, GColor digit_color, GColor background_color) {
  graphics_context_set_fill_color(ctx, digit_color);
  draw_rect(ctx, rect);

  graphics_context_set_fill_color(ctx, background_color);
  switch(digit) {
    case 0:
      draw_rect(ctx, scale_cutout(GRect(2,2,4,8), rect));
      break;
    case 1:
      draw_rect(ctx, scale_cutout(GRect(0,2,3,8), rect));
      draw_rect(ctx, scale_cutout(GRect(5,0,3,10), rect));
      break;
    case 2:
      draw_rect(ctx, scale_cutout(GRect(0,2,6,3), rect));
      draw_rect(ctx, scale_cutout(GRect(2,7,6,3), rect));
      break;
    case 3:
      draw_rect(ctx, scale_cutout(GRect(0,2,6,3), rect));
      draw_rect(ctx, scale_cutout(GRect(0,7,6,3), rect));
      break;
    case 4:
      draw_rect(ctx, scale_cutout(GRect(2,0,4,5), rect));
      draw_rect(ctx, scale_cutout(GRect(0,7,6,5), rect));
      break;
    case 5:
      draw_rect(ctx, scale_cutout(GRect(2,2,6,3), rect));
      draw_rect(ctx, scale_cutout(GRect(0,7,6,3), rect));
      break;
    case 6:
      draw_rect(ctx, scale_cutout(GRect(2,2,6,3), rect));
      draw_rect(ctx, scale_cutout(GRect(2,7,4,3), rect));
      break;
    case 7:
      draw_rect(ctx, scale_cutout(GRect(0,2,6,10), rect));
      break;
    case 8:
      draw_rect(ctx, scale_cutout(GRect(2,2,4,3), rect));
      draw_rect(ctx, scale_cutout(GRect(2,7,4,3), rect));
      break;
    case 9:
      draw_rect(ctx, scale_cutout(GRect(2,2,4,3), rect));
      draw_rect(ctx, scale_cutout(GRect(0,7,6,3), rect));
      break;
  }
}

void draw_digit(GContext *ctx, int digit, GPoint position, DigitSize size, GColor digit_color, GColor background_color) {
  GRect digit_padded_draw_area = GRect(position.x, position.y, size.width, size.height);

  GRect digit_draw_area = grect_inset(digit_padded_draw_area, GEdgeInsets(size.padding_y, size.padding_x));

  if(digit_draw_area.size.w <= 0 || digit_draw_area.size.h <= 0) {
    return;
  }

  graphics_context_set_fill_color(ctx, background_color);
  draw_rect(ctx, digit_padded_draw_area);

  draw_digit_in_rect(ctx, digit, digit_draw_area, digit_color, background_color);
}

//Create a DigitSize with the given values
DigitSize digit_size_new(int width, int height, int padding_x, int padding_y) {
  DigitSize ds;
  ds.width = width;
  ds.height = height;
  ds.padding_x = padding_x;
  ds.padding_y = padding_y;
  return ds;
}

//Recalculate padding of a DigitSize to match the desired new width & height
DigitSize scale_digit(DigitSize base, int newWidth, int newHeight) {
  DigitSize newSize;
  float modifier;

  //width
  if(base.width == newWidth || base.width == 0) {
    newSize.width = base.width;
    newSize.padding_x = base.padding_x;
  } else {
    newSize.width = newWidth;
    modifier = ((float)newWidth/(float)base.width);
    newSize.padding_x = round(base.padding_x * modifier);
  }

  //height
  if(base.height == newHeight || base.height == 0) {
    newSize.height = base.height;
    newSize.padding_y = base.padding_y;
  } else {
    newSize.height = newHeight;
    modifier = ((float)newHeight/(float)base.height);
    newSize.padding_y = round(base.padding_y * modifier);
  }
  return newSize;
}
