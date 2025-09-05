#include "hello.hpp"
#include "units.hpp"
#include <vector>
#include "esp_dsp.h"
#include "lqr.hpp"

using namespace units::literals;

extern "C" void app_main(void) {
  std::vector<int> v;
  auto distance = 10.0_m;
  auto time = 2.0_s;
  auto speed = distance / time;
  printf("Speed: %f\n", speed.value());
  linearizing_mpc mpc(1, 2, [](auto state, auto a, auto b) {}, 3);
}
