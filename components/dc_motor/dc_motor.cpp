#include "dc_motor.hpp"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "hal/gpio_types.h"
#include "hal/ledc_types.h"

static const uint32_t ledc_freq = 9000;

dc_motor::dc_motor(ledc_timer_t timer, ledc_channel_t channel, gpio_num_t a, gpio_num_t b): gpio_b(b) {
  timer_config = (ledc_timer_config_t) {
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .duty_resolution = LEDC_TIMER_14_BIT,
    .timer_num = timer,
    .freq_hz = ledc_freq,
    .clk_cfg = LEDC_AUTO_CLK
  };
  channel_config = (ledc_channel_config_t) {
    .gpio_num = a,
    .speed_mode = LEDC_LOW_SPEED_MODE,
    .channel = channel,
    .timer_sel = timer,
  };
}

void dc_motor::init() {
  ESP_ERROR_CHECK(ledc_timer_config(&timer_config));
  ESP_ERROR_CHECK(ledc_channel_config(&channel_config));

  ESP_ERROR_CHECK(gpio_reset_pin(gpio_b));
  ESP_ERROR_CHECK(gpio_set_direction(gpio_b, GPIO_MODE_OUTPUT));
}

// takes a value from - (2^14 - 1) to 2^14 - 1
void dc_motor::set(int32_t duty_cycle) {
  gpio_set_level(gpio_b, duty_cycle >= 0);
  if (duty_cycle < 0) duty_cycle = -duty_cycle;
  ESP_ERROR_CHECK(ledc_set_duty_and_update(LEDC_LOW_SPEED_MODE, channel, duty_cycle, 0));
}
