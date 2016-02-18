#include <pebble.h>
#include <math.h>
#include "digits.h"
#include "util.h"
#include "layout.h"

static Window *s_main_window;
static Layout layout;

static float large_digit_scale  = 0.310;
static float medium_digit_scale = 0.250;
static float small_digit_scale  = 0.095;

void draw_cylinder(int digit, GRect bounds, GContext *ctx) {
  int large_digit_height = round(large_digit_scale * bounds.size.h);
  int medium_digit_height = round((medium_digit_scale / large_digit_scale) * large_digit_height);
  int small_digit_height = round((small_digit_scale / large_digit_scale) * large_digit_height);

  DigitSize large_digit_size = digit_size_new(bounds.size.w, large_digit_height, 3, 3);
  DigitSize medium_digit_size = scale_digit(large_digit_size, bounds.size.w, medium_digit_height);
  DigitSize small_digit_size = scale_digit(large_digit_size, bounds.size.w, small_digit_height);

  int row1_y = 0;
  int row2_y = row1_y + small_digit_height;
  int row3_y = row2_y + medium_digit_height;
  int row4_y = row3_y + large_digit_height;
  int row5_y = row4_y + medium_digit_height;

  draw_digit(ctx, mod(digit + 2, 10), GPoint(0, row1_y), small_digit_size);
  draw_digit(ctx, mod(digit + 1, 10), GPoint(0, row2_y), medium_digit_size);
  draw_digit(ctx, digit,              GPoint(0, row3_y), large_digit_size);
  draw_digit(ctx, mod(digit - 1, 10), GPoint(0, row4_y), medium_digit_size);
  draw_digit(ctx, mod(digit - 2, 10), GPoint(0, row5_y), small_digit_size);
}

static void cylinder_layer_update_proc(Layer *cylinder_layer, GContext *ctx) {
  LayerData *data = layer_get_data(cylinder_layer);
  GRect bounds = layer_get_bounds(cylinder_layer);
  draw_cylinder(data->current_digit, bounds, ctx);
}

static void update_time(struct tm *tick_time) {
  uint8_t s_hour = tick_time->tm_hour;
  uint8_t s_minute = tick_time->tm_min;

  uint8_t formatter_hour = clock_is_24h_style() ? s_hour : s_hour % 12;

  LayerData *hour_tens = layer_get_data(layout.hour_tens);
  LayerData *hour_ones = layer_get_data(layout.hour_ones);
  LayerData *minute_tens = layer_get_data(layout.minute_tens);
  LayerData *minute_ones = layer_get_data(layout.minute_ones);

  hour_tens->current_digit = formatter_hour / 10;
  hour_ones->current_digit = formatter_hour % 10;
  minute_tens->current_digit = s_minute / 10;
  minute_ones->current_digit = s_minute % 10;

  layer_mark_dirty(layout.hour_tens);
  layer_mark_dirty(layout.hour_ones);
  layer_mark_dirty(layout.minute_tens);
  layer_mark_dirty(layout.minute_ones);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

static void main_window_load(Window *window) {
  set_layout(window, &layout);
  layer_set_update_proc(layout.hour_tens, cylinder_layer_update_proc);
  layer_set_update_proc(layout.hour_ones, cylinder_layer_update_proc);
  layer_set_update_proc(layout.minute_tens, cylinder_layer_update_proc);
  layer_set_update_proc(layout.minute_ones, cylinder_layer_update_proc);
}

static void main_window_unload(Window *window) {
  layer_destroy(layout.root_layer);
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
  tick_timer_service_unsubscribe();
  window_destroy(s_main_window);
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
