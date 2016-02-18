#include <pebble.h>
#include <stdbool.h>
#include <math.h>
#include "digits.h"

const int DIGIT_DATA_HEIGHT = 12;
const int DIGIT_DATA_WIDTH = 8;

const int ZERO[12][8] = {
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
};

const int ONE[12][8] = {
  {1,1,1,1,1,0,0,0},
  {1,1,1,1,1,0,0,0},
  {0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0},
  {0,0,0,1,1,0,0,0},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
};

const int TWO[12][8] = {
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,0,0,0,0,0,0},
  {1,1,0,0,0,0,0,0},
  {1,1,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
};

const int THREE[12][8] = {
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
};

const int FOUR[12][8] = {
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
};

const int FIVE[12][8] = {
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,0,0,0,0,0,0},
  {1,1,0,0,0,0,0,0},
  {1,1,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
};

const int SIX[12][8] = {
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,0,0,0,0,0,0},
  {1,1,0,0,0,0,0,0},
  {1,1,0,0,0,0,0,0},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
};

const int SEVEN[12][8] = {
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
};

const int EIGHT[12][8] = {
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
};

const int NINE[12][8] = {
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {0,0,0,0,0,0,1,1},
  {1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1},
};

bool pixel_is_filled(int number, int x, int y) {
  switch(number) {
   case 0: return ZERO[y][x] == 1;
   case 1: return ONE[y][x] == 1;
   case 2: return TWO[y][x] == 1;
   case 3: return THREE[y][x] == 1;
   case 4: return FOUR[y][x] == 1;
   case 5: return FIVE[y][x] == 1;
   case 6: return SIX[y][x] == 1;
   case 7: return SEVEN[y][x] == 1;
   case 8: return EIGHT[y][x] == 1;
   case 9: return NINE[y][x] == 1;
   default: return false;
  }
}

void draw_digit(GContext *ctx, int digit, GPoint start_position, DigitSize size) {
  int data_x, data_y;

  const float data_height_modifier = (float)DIGIT_DATA_HEIGHT/(float)(size.height - (2*size.padding_y));
  const float data_width_modifier  = (float)DIGIT_DATA_WIDTH/(float)(size.width - (2*size.padding_x));

  const int y_start = start_position.y + size.padding_y;
  const int y_end = start_position.y + size.height - size.padding_y;

  const int x_start = start_position.x + size.padding_x;
  const int x_end = start_position.x + size.width - size.padding_x;

  for(int y = y_start; y < y_end; y = y + 1 ) {
    for(int x = x_start; x < x_end; x = x + 1 ) {

      data_x = data_width_modifier*(x-x_start);
      data_y = data_height_modifier*(y-y_start);

      if(pixel_is_filled(digit, data_x, data_y)) {
        graphics_draw_pixel(ctx, GPoint(x, y));
      }
    }
  }
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
