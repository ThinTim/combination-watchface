#include <pebble.h>
#include <math.h>
#include "cylinder.h"
#include "digits.h"

#define LARGE_DIGIT_SCALE   0.310f
#define MEDIUM_DIGIT_SCALE  0.250f
#define SMALL_DIGIT_SCALE   0.095f

static int mod(int a, int b)
{
    int r = a % b;
    return r < 0 ? r + b : r;
}

void draw_cylinder(CylinderState *state, GRect bounds, GContext *ctx) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "draw_cylinder(%p, <bounds>, %p)", (void*)state, (void*)ctx);

  const int digit = state->current_digit;
  const float progress = state->tick_progress;

  //Calculate the normal height of each digit row
  const int large_digit_height = round(LARGE_DIGIT_SCALE * bounds.size.h);
  const int medium_digit_height = round(MEDIUM_DIGIT_SCALE * bounds.size.h);
  const int small_digit_height = round(SMALL_DIGIT_SCALE * bounds.size.h);

  DigitSize base_digit_size = digit_size_new(bounds.size.w, large_digit_height, 3, 3);

  //Calculate actual heights of each digit row given tick progress
  int row1_h, row2_h, row3_h, row4_h, row5_h, row6_h;
  row1_h = round(progress * small_digit_height);
  row2_h = small_digit_height + round(progress * (medium_digit_height - small_digit_height));
  row3_h = medium_digit_height + round(progress * (large_digit_height - medium_digit_height));
  row4_h = large_digit_height - round(progress * (large_digit_height - medium_digit_height));
  row5_h = medium_digit_height - round(progress * (medium_digit_height - small_digit_height));
  row6_h = small_digit_height - round(progress * small_digit_height);

  //Calculate y positions of each row
  int row1_y, row2_y, row3_y, row4_y, row5_y, row6_y;
  row1_y = 0;
  row2_y = row1_y + row1_h;
  row3_y = row2_y + row2_h;
  row4_y = row3_y + row3_h;
  row5_y = row4_y + row4_h;
  row6_y = row5_y + row5_h;

  //Draw the rows
  graphics_context_set_stroke_color(ctx, GColorBlack);

  draw_digit(ctx, mod(digit + 2, 10), GPoint(0, row1_y), scale_digit(base_digit_size, bounds.size.w, row1_h));
  draw_digit(ctx, mod(digit + 1, 10), GPoint(0, row2_y), scale_digit(base_digit_size, bounds.size.w, row2_h));
  draw_digit(ctx, digit,              GPoint(0, row3_y), scale_digit(base_digit_size, bounds.size.w, row3_h));
  draw_digit(ctx, mod(digit - 1, 10), GPoint(0, row4_y), scale_digit(base_digit_size, bounds.size.w, row4_h));
  draw_digit(ctx, mod(digit - 2, 10), GPoint(0, row5_y), scale_digit(base_digit_size, bounds.size.w, row5_h));
  if(row6_h > 0) {
      draw_digit(ctx, mod(digit - 3, 10), GPoint(0, row6_y), scale_digit(base_digit_size, bounds.size.w, row6_h));
  }
}

//Saves us from having to malloc it every time
static AnimationImplementation animation_impl;

//Returns a value between 0.0f and 1.0f indicating animation progress
static float animation_percentage(AnimationProgress dist_normalized) {
  return (float)dist_normalized / (float)ANIMATION_NORMALIZED_MAX;
}

static void tick_animation_start(Animation *anim, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "tick_animation_start(%p, %p)", (void*)anim, context);

  Layer *cylinder_layer = (Layer*)context;
  CylinderState *state = layer_get_data(cylinder_layer);

  state->tick_progress = 0.0f;

  layer_mark_dirty(cylinder_layer);
}

static void tick_animation_stop(Animation *anim, bool finished, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "tick_animation_stop(%p, %d, %p)", (void*)anim, (int)finished, context);

  Layer *cylinder_layer = (Layer*)context;
  CylinderState *state = layer_get_data(cylinder_layer);

  state->tick_progress = 1.0f;

  layer_mark_dirty(cylinder_layer);
}

static void tick_animation_update(Animation *anim, AnimationProgress dist_normalized) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "tick_animation_update(%p, %d)", (void*)anim, (int)dist_normalized);

  Layer *cylinder_layer = animation_get_context(anim);
  CylinderState *state = layer_get_data(cylinder_layer);

  state->tick_progress = animation_percentage(dist_normalized);

  layer_mark_dirty(cylinder_layer);
}

static void tick_animation_teardown(Animation *anim) {
  animation_destroy(anim);
}

void schedule_tick_animation(Layer *cylinder_layer) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "schedule_tick_animation(%p)", (void*)cylinder_layer);

  Animation *anim = animation_create();

  animation_set_duration(anim, ANIMATION_DURATION);
  animation_set_delay(anim, ANIMATION_DELAY);
  animation_set_curve(anim, AnimationCurveEaseIn);

  AnimationHandlers handlers = (AnimationHandlers) {
    .started = tick_animation_start,
    .stopped = tick_animation_stop
  };
  animation_set_handlers(anim, handlers, cylinder_layer);

  if(animation_impl.update == NULL) {
    animation_impl = (AnimationImplementation) {
      .update = tick_animation_update,
      .teardown = tick_animation_teardown
    };
  }
  animation_set_implementation(anim, &animation_impl);

  animation_schedule(anim);
}
