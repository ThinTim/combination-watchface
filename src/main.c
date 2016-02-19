#include <pebble.h>
#include <math.h>
#include "digits.h"
#include "layout.h"
#include "cylinder.h"

static Window *s_main_window;
static Layout layout;

static void cylinder_layer_update_proc(Layer *cylinder_layer, GContext *ctx) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "cylinder_layer_update_proc(%p, %p)", (void*)cylinder_layer, (void*)ctx);

  CylinderState *state = layer_get_data(cylinder_layer);
  GRect bounds = layer_get_bounds(cylinder_layer);
  draw_cylinder(state, bounds, ctx);
}

static void update_layer_digit(Layer *layer, int new_digit) {
  CylinderState *state = layer_get_data(layer);

  int current_digit = state->current_digit;

  if(current_digit != new_digit) {
    state->current_digit = new_digit;
    schedule_tick_animation(layer);
  }
}

static void update_time(struct tm *tick_time) {
  int s_hour = tick_time->tm_hour;
  int s_minute = tick_time->tm_min;

  int formatter_hour = clock_is_24h_style() ? s_hour : s_hour % 12;

  update_layer_digit(layout.hour_tens, formatter_hour / 10);
  update_layer_digit(layout.hour_ones, formatter_hour % 10);
  update_layer_digit(layout.minute_tens, s_minute / 10);
  update_layer_digit(layout.minute_ones, s_minute % 10);

  APP_LOG(APP_LOG_LEVEL_DEBUG, "MEMORY USAGE = %d bytes", (int)heap_bytes_used());
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

static void main_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "main_window_load(%p)", (void*)window);

  set_layout(window, &layout);

  layer_set_update_proc(layout.hour_tens, cylinder_layer_update_proc);
  layer_set_update_proc(layout.hour_ones, cylinder_layer_update_proc);
  layer_set_update_proc(layout.minute_tens, cylinder_layer_update_proc);
  layer_set_update_proc(layout.minute_ones, cylinder_layer_update_proc);
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
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  window_stack_push(s_main_window, true);

  time_t start = time(NULL);
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  update_time(localtime(&start));
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
