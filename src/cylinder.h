#pragma once
#include <pebble.h>

#define ANIMATION_DURATION  500
#define ANIMATION_DELAY     0

typedef struct {
  int current_digit;
  float tick_progress; //Tick animation progress as a fraction of 1
} CylinderState;

void draw_cylinder(CylinderState *state, GRect bounds, GContext *ctx);
void schedule_tick_animation(Layer *cylinder_layer);
