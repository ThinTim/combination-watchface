#include <pebble.h>
#include "layout.h"
#include "digits.h"

void set_layout(Window *window, Layout *layout) {
  layout->root_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(layout->root_layer);
  GPoint center = grect_center_point(&window_bounds);

  int layer_height, layer_width;
  #if defined(PBL_ROUND)
  layer_width = 24;
  layer_height = 130;
  #else
  layer_width = 35;
  layer_height = 160;
  #endif

  GRect hour_tens_frame = GRect(center.x - (layer_width * 2), center.y - (layer_height / 2), layer_width, layer_height);
  GRect hour_ones_frame = GRect(center.x - layer_width,       center.y - (layer_height / 2), layer_width, layer_height);
  GRect minute_tens_frame = GRect(center.x,                   center.y - (layer_height / 2), layer_width, layer_height);
  GRect minute_ones_frame = GRect(center.x + layer_width,     center.y - (layer_height / 2), layer_width, layer_height);

  layout->hour_tens = layer_create_with_data(hour_tens_frame, sizeof(LayerData));
  layout->hour_ones = layer_create_with_data(hour_ones_frame, sizeof(LayerData));
  layout->minute_tens = layer_create_with_data(minute_tens_frame, sizeof(LayerData));
  layout->minute_ones = layer_create_with_data(minute_ones_frame, sizeof(LayerData));

  layer_add_child(layout->root_layer, layout->hour_tens);
  layer_add_child(layout->root_layer, layout->hour_ones);
  layer_add_child(layout->root_layer, layout->minute_tens);
  layer_add_child(layout->root_layer, layout->minute_ones);
}
