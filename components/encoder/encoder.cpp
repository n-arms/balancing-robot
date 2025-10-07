#include "encoder.hpp"
#include <limits.h>
#include <driver/pulse_cnt.h>
#include "esp_timer.h"
#include "hal/pcnt_types.h"
#include <limits>

static const uint16_t max_glitch_ns = 128;
static const int32_t speed_estimate_time_us = 100;

encoder::encoder(
  gpio_num_t edge_gpio, gpio_num_t level_gpio
): last_count(0), last_count_time(0), unit(nullptr), channel(nullptr) {
  unit_config = (pcnt_unit_config_t) {
    .low_limit = std::numeric_limits<int16_t>::min(),
    .high_limit = std::numeric_limits<int16_t>::max(),
  };
  channel_config = (pcnt_chan_config_t) {
    .edge_gpio_num = edge_gpio,
    .level_gpio_num = level_gpio,
  };
  glitch_filter_config = (pcnt_glitch_filter_config_t) {
    .max_glitch_ns = max_glitch_ns
  };
}

void encoder::init() {
  ESP_ERROR_CHECK(pcnt_new_unit(&unit_config, &unit));
  ESP_ERROR_CHECK(pcnt_new_channel(unit, &channel_config, &channel));

  ESP_ERROR_CHECK(pcnt_channel_set_edge_action(channel, PCNT_CHANNEL_EDGE_ACTION_INCREASE, PCNT_CHANNEL_EDGE_ACTION_HOLD));
  ESP_ERROR_CHECK(pcnt_channel_set_level_action(channel, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));

  ESP_ERROR_CHECK(pcnt_unit_set_glitch_filter(unit, &glitch_filter_config));

  ESP_ERROR_CHECK(pcnt_unit_enable(unit));
  ESP_ERROR_CHECK(pcnt_unit_start(unit));
}

int32_t encoder::position() const {
  int count;
  ESP_ERROR_CHECK(pcnt_unit_get_count(unit, &count));
  return count;
}

int32_t encoder::speed() {
  int64_t current_time = esp_timer_get_time();
  int32_t dt = current_time - last_count_time;
  int32_t count = position();
  int32_t speed = (count - last_count) / dt;

  if (dt > speed_estimate_time_us) {
    last_count_time = current_time;
    last_count = count;
  }

  return speed;
}
