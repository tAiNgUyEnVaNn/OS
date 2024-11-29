#include <stdio.h>
#include <esp_log.h>
#include <driver/i2c.h>
#include "../lib/inc/lcd.h"

void setup()
{
  ESP_ERROR_CHECK(i2c_master_init());
}

void loop()
{
  // ESP_ERROR_CHECK(i2c_master_init());
  // ESP_LOGI(TAG, "I2C initialized successfully");

  lcd_init();
  lcd_clear();

  lcd_set_cursor(0, 0);
  lcd_send_string("Hello world!");

  lcd_set_cursor(1, 0);
  lcd_send_string("from ESP32");

  // sprintf(buffer, "val=%.2f", num);
  // lcd_set_cursor(0, 0);
  // lcd_send_string(buffer);
}
