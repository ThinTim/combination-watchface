#include <pebble.h>
#include <math.h>
#include "digits.h"
#include "layout.h"
#include "cylinder.h"

static Window *s_main_window;
static Layout layout;

static void cylinder_layer_update_proc(Layer *cylinder_layer, GContext *ctx) {
  CylinderState *state = layer_get_data(cylinder_layer);
  GRect bounds = layer_get_bounds(cylinder_layer);
  draw_cylinder(state, bounds, ctx);
}

static void update_layer_digit(Layer *layer, int new_digit, bool first_update) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "update_layer_digit(%p, %d, %d)", (void*)layer, new_digit, (int)first_update);

  CylinderState *state = layer_get_data(layer);

  if(first_update) {
    //On first load, every cylinder clicks into place at once
    state->current_digit = new_digit;
  }

  if(first_update || (state->target_digit != new_digit)) {
    state->target_digit = new_digit;
    begin_tick_animation(layer);
  }
}

static void update_time(struct tm *tick_time, bool first_update) {
  int hour = tick_time->tm_hour;
  int minute = tick_time->tm_min;

  APP_LOG(APP_LOG_LEVEL_DEBUG, "update_time(%d:%d)", (int)hour, (int)minute);
  APP_LOG(APP_LOG_LEVEL_DEBUG, "MEMORY USAGE = %d bytes", (int)heap_bytes_used());

  int formatted_hour = clock_is_24h_style() ? hour : hour % 12;

  update_layer_digit(layout.hour_tens, formatted_hour / 10, first_update);
  update_layer_digit(layout.hour_ones, formatted_hour % 10, first_update);
  update_layer_digit(layout.minute_tens, minute / 10, first_update);
  update_layer_digit(layout.minute_ones, minute % 10, first_update);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time, false);
}

static void intialise_cylinder_layer(Layer *layer) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "intialise_cylinder_layer(%p)", (void*)layer);

  CylinderState* state = layer_get_data(layer);
  initialise_cylinder_state(state);
  layer_set_update_proc(layer, cylinder_layer_update_proc);
}

static void main_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main_window_load(%p)", (void*)window);

  set_layout(window, &layout);

  intialise_cylinder_layer(layout.hour_tens);
  intialise_cylinder_layer(layout.hour_ones);
  intialise_cylinder_layer(layout.minute_tens);
  intialise_cylinder_layer(layout.minute_ones);

  time_t start = time(NULL);
  update_time(localtime(&start), true);
}

static void main_window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main_window_unload(%p)", (void*)window);
  Layer *root_layer = window_get_root_layer(window);

  layer_destroy(layout.hour_tens);
  layer_destroy(layout.hour_ones);
  layer_destroy(layout.minute_tens);
  layer_destroy(layout.minute_ones);
  layer_destroy(root_layer);
}

static void init(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "init()");

  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);

  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit(void) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "deinit()");

  animation_unschedule_all();
  tick_timer_service_unsubscribe();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
