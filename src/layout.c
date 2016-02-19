#include <pebble.h>
#include "layout.h"
#include "cylinder.h"

Layer *cylinder_layer_new(Layer *parent, GPoint origin, GSize size) {
  GRect layer_frame = GRect(origin.x, origin.y, size.w, size.h);

  Layer *layer = layer_create_with_data(layer_frame, sizeof(CylinderState));

  layer_add_child(parent, layer);

  return layer;
}

void set_layout(Window *window, Layout *layout) {
  Layer *root_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(root_layer);

  GPoint center = grect_center_point(&window_bounds);
  GSize layer_size = PBL_IF_ROUND_ELSE(GSize(24, 130), GSize(35,160));
  int layer_y_pos = center.y - (layer_size.h / 2);

  layout->hour_tens = cylinder_layer_new(root_layer,   GPoint(center.x - (layer_size.w * 2), layer_y_pos), layer_size);
  layout->hour_ones = cylinder_layer_new(root_layer,   GPoint(center.x - layer_size.w,       layer_y_pos), layer_size);
  layout->minute_tens = cylinder_layer_new(root_layer, GPoint(center.x,                      layer_y_pos), layer_size);
  layout->minute_ones = cylinder_layer_new(root_layer, GPoint(center.x + layer_size.w,       layer_y_pos), layer_size);
}
