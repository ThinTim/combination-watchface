#pragma once
#include <pebble.h>

typedef struct {
  int current_digit;
  int target_digit;

  /*
   * Tick animation progress as a fraction of 1.
   * When tick_progress is 0, (current_digit-1) is centered.
   * When tick_progress is 1, (current_digit) is centered.
   */
  float tick_progress;
  bool animating;
} CylinderState;

void draw_cylinder(CylinderState *state, GRect bounds, GContext *ctx);
void begin_tick_animation(Layer *cylinder_layer);
void initialise_cylinder_state(CylinderState *state);
