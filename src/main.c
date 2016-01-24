#include <pebble.h>
#include <math.h>
#include "digits.h"
#include "util.h"

const float medium_digit_height_modifier = 0.80;
const float small_digit_height_modifier = 0.31;

static Window *s_main_window;
static Layer *s_canvas_layer;

static uint8_t s_hour;
static uint8_t s_minute;

static DigitSize digit_size_large, digit_size_medium, digit_size_small;

static int col1_x, col2_x, col3_x, col4_x;
static int row1_y, row2_y, row3_y, row4_y, row5_y;

void draw_wheel(int digit, int x_position, GContext *ctx) {
  draw_digit(ctx, mod(digit + 2, 10), GPoint(x_position, row1_y), digit_size_small);
  draw_digit(ctx, mod(digit + 1, 10), GPoint(x_position, row2_y), digit_size_medium);
  draw_digit(ctx, digit,              GPoint(x_position, row3_y), digit_size_large);
  draw_digit(ctx, mod(digit - 1, 10), GPoint(x_position, row4_y), digit_size_medium);
  draw_digit(ctx, mod(digit - 2, 10), GPoint(x_position, row5_y), digit_size_small);
}

static void canvas_update_proc(Layer *this_layer, GContext *ctx) {
  int hour = clock_is_24h_style() ? s_hour : s_hour % 12;

  draw_wheel(hour / 10,     col1_x, ctx);
  draw_wheel(hour % 10,     col2_x, ctx);
  draw_wheel(s_minute / 10, col3_x, ctx);
  draw_wheel(s_minute % 10, col4_x, ctx);
}

static void update_time(struct tm *tick_time) {
  s_hour = tick_time->tm_hour;
  s_minute = tick_time->tm_min;
  layer_mark_dirty(s_canvas_layer);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time(tick_time);
}

static void set_layout(GRect window_bounds) {
  #if defined(PBL_ROUND)
  digit_size_large = digit_size_new(24, 45, 2, 3);
  #else
  digit_size_large = digit_size_new(35, 50, 3, 3);
  #endif
  digit_size_medium = scale_digit(digit_size_large, digit_size_large.width, round(medium_digit_height_modifier * digit_size_large.height));
  digit_size_small  = scale_digit(digit_size_large, digit_size_large.width, round(small_digit_height_modifier * digit_size_large.height));

  GPoint center = grect_center_point(&window_bounds);

  col1_x = center.x - (digit_size_large.width * 2);
  col2_x = center.x - digit_size_large.width;
  col3_x = center.x;
  col4_x = center.x + digit_size_large.width;

  row1_y = center.y - (digit_size_large.height / 2) - digit_size_medium.height - digit_size_small.height;
  row2_y = center.y - (digit_size_large.height / 2) - digit_size_medium.height;
  row3_y = center.y - (digit_size_large.height / 2);
  row4_y = center.y + (digit_size_large.height / 2);
  row5_y = center.y + (digit_size_large.height / 2) + digit_size_medium.height;
}

static void main_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect window_bounds = layer_get_bounds(window_layer);

  set_layout(window_bounds);

  s_canvas_layer = layer_create(GRect(0, 0, window_bounds.size.w, window_bounds.size.h));
  layer_add_child(window_layer, s_canvas_layer);
  layer_set_update_proc(s_canvas_layer, canvas_update_proc);
}

static void main_window_unload(Window *window) {
  layer_destroy(s_canvas_layer);
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
