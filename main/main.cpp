#include "hello.hpp"
#include "mat.h"
#include "units.hpp"
#include <vector>
#include "esp_dsp.h"
#include "lqr.hpp"
#include "driver/gpio.h"
#include "esp_log.h"

#define GPIO_OUTPUT_PIN  GPIO_NUM_15


using namespace units::literals;

extern "C" void app_main(void) {
  std::vector<int> v;
  auto distance = 10.0_m;
  auto time = 2.0_s;
  auto speed = distance / time;
  printf("Speed: %f\n", speed.value());


  gpio_config_t io_conf = {
      .pin_bit_mask = (1ULL << GPIO_OUTPUT_PIN), // Bit mask of the pin
      .mode = GPIO_MODE_OUTPUT,                  // Set as output mode
      .pull_up_en = GPIO_PULLUP_DISABLE,         // Disable pull-up
      .pull_down_en = GPIO_PULLDOWN_DISABLE,     // Disable pull-down
      .intr_type = GPIO_INTR_DISABLE             // No interrupt
  };
  gpio_config(&io_conf);

  float elems[52];
  for (auto i = 0; i < 52; ++i) elems[i] = i & 7;
  dspm::Mat b(elems, 7, 3);
  dspm::Mat q(elems + 1, 7, 7);
  dspm::Mat r(elems + 2, 3, 3);
  dspm::Mat x(elems, 7, 1);

  for (uint32_t i = 0;; ++i) {
    elems[0] = i;
    dspm::Mat a(elems, 7, 7);
    lqr solved_lqr = solve_lqr(a, b, q, r);
    if (solved_lqr.calculate(x, x)(0, 0) > 0.) {
      gpio_set_level(GPIO_OUTPUT_PIN, 1);
    } else {
      gpio_set_level(GPIO_OUTPUT_PIN, 0);
    }
  }
}
