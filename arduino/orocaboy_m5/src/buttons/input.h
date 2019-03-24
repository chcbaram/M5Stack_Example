#pragma once

#include <stdint.h>


#define M5STACK_GAMEPAD_IO_LEFT    GPIO_NUM_39
#define M5STACK_GAMEPAD_IO_RIGHT   GPIO_NUM_37
#define M5STACK_GAMEPAD_IO_A       GPIO_NUM_38


enum
{
	M5STACK_INPUT_LEFT,
  M5STACK_INPUT_RIGHT,
  M5STACK_INPUT_A,
  M5STACK_INPUT_B,
	M5STACK_INPUT_MAX
};

typedef struct
{
    uint8_t values[M5STACK_INPUT_MAX];
} m5stack_gamepad_state;


void input_init();
void input_read(m5stack_gamepad_state* out_state);
m5stack_gamepad_state input_read_raw();
