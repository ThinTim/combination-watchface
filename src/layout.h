#pragma once
#include <pebble.h>

typedef struct {
  int current_digit;
} LayerData;

typedef struct {
  Layer *root_layer;
  Layer *hour_tens;
  Layer *hour_ones;
  Layer *minute_tens;
  Layer *minute_ones;
} Layout;

void set_layout(Window *window, Layout *layout);
