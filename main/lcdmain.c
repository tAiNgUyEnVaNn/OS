/* i2c - Simple example

   Simple I2C example that shows how to initialize I2C
   as well as reading and writing from and to registers for a sensor connected over I2C.

   The sensor used in this example is a MPU9250 inertial measurement unit.

   For other examples please check:
   https://github.com/espressif/esp-idf/tree/master/examples

   See README.md file to get detailed usage of this example.

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "lcd.h"
#include "mpu6050.h"

static const char *TAG = "i2c-simple-example";

#define I2C_MASTER_SCL_IO GPIO_NUM_22 /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO GPIO_NUM_21 /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM 0              /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ 100000     /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE 0   /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0   /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS 1000

char buffer[10];
float num = 12.34;

/**
 * @brief i2c master initialization
 */
static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

void app_main(void)
{
    ESP_ERROR_CHECK(i2c_master_init());
    // ESP_LOGI(TAG, "I2C initialized successfully");

    lcd_init();
    lcd_clear();

    //    lcd_put_cur(0, 0);
    //    lcd_send_string("Hello world!");
    //
    //    lcd_set_cursor(1, 0);
    //    lcd_send_string("from ESP32");

    sprintf(buffer, "val=%.2f", num);
    lcd_set_cursor(0, 0);
    lcd_send_string(buffer);
}
