#include "icm_42688.hpp"
#include "driver/spi_common.h"
#include "hal/spi_types.h"
#include <cstring>
#include <driver/spi_master.h>

icm_42688::icm_42688(
    gpio_num_t intr,
    gpio_num_t cs,
    gpio_num_t sclk,
    gpio_num_t mosi,
    gpio_num_t miso
) {
  bus_config = (spi_bus_config_t) {
    .mosi_io_num = mosi,
    .miso_io_num = miso,
    .sclk_io_num = sclk,
    .data2_io_num = -1,
    .data3_io_num = -1,
    .max_transfer_sz = 4096
  };
  spi_host = SPI2_HOST;
  device_config = (spi_device_interface_config_t) {
    .mode = 3,
    .clock_speed_hz = 1000 * 1000,
    .spics_io_num = cs,
    .queue_size = 1
  };
}

constexpr std::size_t read_flag = 0b1000000;
constexpr std::size_t write_flag = 0b0000000;

void icm_42688::init() {
  ESP_ERROR_CHECK(spi_bus_initialize(spi_host, &bus_config, SPI_DMA_CH_AUTO));
  ESP_ERROR_CHECK(spi_bus_add_device(SPI2_HOST, &device_config, &spi));
}

void icm_42688::read(uint8_t reg, uint32_t length, uint8_t bytes[]) {
  tx_buffer[0] = read_flag | reg;
  transaction(length + 1);
  memcpy(bytes, rx_buffer.data() + 1, length);
}

void icm_42688::write(uint8_t reg, uint32_t length, const uint8_t bytes[]) {
  rx_buffer[0] = write_flag | reg;
  memcpy(tx_buffer.data() + 1, bytes, length);
  transaction(length + 1);
}

void icm_42688::transaction(uint32_t length) {
  assert(length <= max_transfer_size);
  spi_transaction_t t = {
    .length = 8 * length,
    .tx_buffer = tx_buffer.data(),
    .rx_buffer = rx_buffer.data()
  };

  ESP_ERROR_CHECK(spi_device_transmit(spi, &t));
}
