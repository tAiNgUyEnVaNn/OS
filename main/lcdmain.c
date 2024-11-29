
#include <stdio.h>
#include "esp_log.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <sdkconfig.h>
#include <esp_log.h>
#include <driver/i2c.h>
#include "../lib/inc/lcd.h"

static const char *TAG = "i2c-simple-example";

void app_main(void)
{
    ESP_ERROR_CHECK(master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    lcd_init();
    lcd_clear();

    lcd_set_cursor(0, 0);
    lcd_send_string("Hello world!");

    lcd_set_cursor(1, 0);
    lcd_send_string("from ESP32");

    // sprintf(buffer, "val=%.2f", num);
    // lcd_put_cur(0, 0);
    // lcd_send_string(buffer);
}
