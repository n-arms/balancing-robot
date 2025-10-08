#ifndef ICM_42688_H
#define ICM_42688_H

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "hal/spi_types.h"
#include "soc/gpio_num.h"
#include <array>

constexpr std::size_t max_transfer_size = 1024;

class icm_42688 {
public:
  icm_42688(
    gpio_num_t intr,
    gpio_num_t cs,
    gpio_num_t sck,
    gpio_num_t mosi,
    gpio_num_t miso
  );
  void init();
private:
  void read(uint8_t reg, uint32_t length, uint8_t bytes[]);
  void write(uint8_t reg, uint32_t length, const uint8_t bytes[]);
  void transaction(uint32_t length);
  
  spi_bus_config_t bus_config;
  spi_host_device_t spi_host;
  spi_device_interface_config_t device_config;
  spi_device_handle_t spi;

  std::array<uint8_t, max_transfer_size> rx_buffer;
  std::array<uint8_t, max_transfer_size> tx_buffer;
};


#endif
