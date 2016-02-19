#pragma once
#include <pebble.h>

typedef struct {
  Layer *hour_tens;
  Layer *hour_ones;
  Layer *minute_tens;
  Layer *minute_ones;
} Layout;

void set_layout(Window *window, Layout *layout);
