#ifndef ENCODER_H
#define ENCODER_H

#include <driver/gpio.h>
#include <driver/pulse_cnt.h>

class encoder {
public:
  encoder(gpio_num_t edge_gpio, gpio_num_t level_gpio);
  void init();
  int32_t position() const;
  int32_t speed();
private:
  int32_t last_count;
  int64_t last_count_time;
  pcnt_unit_config_t unit_config;
  pcnt_unit_handle_t unit;
  pcnt_chan_config_t channel_config;
  pcnt_channel_handle_t channel;
  pcnt_glitch_filter_config_t glitch_filter_config;
};

#endif
