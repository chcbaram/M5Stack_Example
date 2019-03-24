#include "input.h"

#include "driver/gpio.h"
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"


static volatile bool input_task_is_running = false;
static volatile m5stack_gamepad_state gamepad_state;
static m5stack_gamepad_state previous_gamepad_state;
static uint8_t debounce[M5STACK_INPUT_MAX];
static volatile bool input_gamepad_initialized = false;
static SemaphoreHandle_t xSemaphore;




m5stack_gamepad_state input_read_raw()
{
  m5stack_gamepad_state state = {0};

  state.values[M5STACK_INPUT_LEFT]  = !(gpio_get_level(M5STACK_GAMEPAD_IO_LEFT));
  state.values[M5STACK_INPUT_RIGHT] = !(gpio_get_level(M5STACK_GAMEPAD_IO_RIGHT));
  state.values[M5STACK_INPUT_A]     = !(gpio_get_level(M5STACK_GAMEPAD_IO_A));

  return state;
}

void input_read(m5stack_gamepad_state* out_state)
{
  if (!input_gamepad_initialized) abort();

  xSemaphoreTake(xSemaphore, portMAX_DELAY);

  *out_state = gamepad_state;

  xSemaphoreGive(xSemaphore);
}

static void input_task(void *arg)
{
  input_task_is_running = true;

  // Initialize state
  for(int i = 0; i < M5STACK_INPUT_MAX; ++i)
  {
    debounce[i] = 0xff;
  }

  while(input_task_is_running)
  {
    // Shift current values
    for(int i = 0; i < M5STACK_INPUT_MAX; ++i)
    {
      debounce[i] <<= 1;
    }

    // Read hardware
    m5stack_gamepad_state state = input_read_raw();

    // Debounce
    xSemaphoreTake(xSemaphore, portMAX_DELAY);

    for(int i = 0; i < M5STACK_INPUT_MAX; ++i)
    {
      debounce[i] |= state.values[i] ? 1 : 0;
      uint8_t val = debounce[i] & 0x03; //0x0f;
      switch (val) {
        case 0x00:
          gamepad_state.values[i] = 0;
          break;

        case 0x03: //0x0f:
          gamepad_state.values[i] = 1;
          break;

        default:
          // ignore
          break;
      }
    }

    previous_gamepad_state = gamepad_state;

    xSemaphoreGive(xSemaphore);

    // delay
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }

  input_gamepad_initialized = false;

  vSemaphoreDelete(xSemaphore);

  // Remove the task from scheduler
  vTaskDelete(NULL);

  // Never return
  while (1) { vTaskDelay(1);}
}

void input_init()
{
    xSemaphore = xSemaphoreCreateMutex();

    if(xSemaphore == NULL)
    {
        printf("xSemaphoreCreateMutex failed.\n");
        abort();
    }


	gpio_set_direction(M5STACK_GAMEPAD_IO_LEFT, GPIO_MODE_INPUT);
	gpio_set_direction(M5STACK_GAMEPAD_IO_RIGHT, GPIO_MODE_INPUT);
	gpio_set_direction(M5STACK_GAMEPAD_IO_A, GPIO_MODE_INPUT);



  input_gamepad_initialized = true;

  // Start background polling
  xTaskCreatePinnedToCore(&input_task, "input_task", 1024 * 2, NULL, 5, NULL, 1);
}
