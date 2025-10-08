#ifndef DC_MOTOR
#define DC_MOTOR

#include "hal/ledc_types.h"
#include "soc/gpio_num.h"
#include <driver/ledc.h>

class dc_motor {
public:
  dc_motor(ledc_timer_t timer, ledc_channel_t channel, gpio_num_t a, gpio_num_t b);
  void init();
  void set(int32_t duty_cycle);
private:
  ledc_timer_config_t timer_config;
  ledc_channel_config_t channel_config;

  ledc_timer_t timer;
  ledc_channel_t channel;

  gpio_num_t gpio_b;
};

#endif
