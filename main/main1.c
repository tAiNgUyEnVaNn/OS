#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <sdkconfig.h>
#include "driver/i2c.h"
#include "lcd.h"
#include "mpu6050.h"

// Delay time per task
#define D_MPU 10
#define D_LCD 100

mpuValue rV;

SemaphoreHandle_t xMutex = NULL;
TickType_t timeOut = 1000;

void mpuTask()
{
    printf("Starting mpu6050");

    while (true)
    {
        if (xSemaphoreTake(xMutex, timeOut) == pdPASS)
        {
            esp_err_t ret = mpuReadSensors();
            if (ret == ESP_OK)
            {
                // rV.accX = rawAccelX();
                // rV.accY = rawAccelY();
                // rV.accZ = rawAccelZ();
                // rV.gyrX = rawGyrX();
                // rV.gyrY = rawGyrY();
                // rV.gyrZ = rawGyrZ();
                // float delay = D_MPU*CONFIG_FREERTOS_HZ
                updateMpuVal(&rV, pdTICKS_TO_MS(D_MPU));
                // printf("From MPU6050: %d\n", rV.gyrX);
            }
            else
            {
                printf("Read sensor fail: %s\n", esp_err_to_name(ret));
            }

            xSemaphoreGive(xMutex);
        }
        else
        {
            // do nothing
        }
        vTaskDelay(D_MPU);
    }
}

void testTask()
{
    // print for testing mpu6050
    printf("Ready for test");

    while (true)
    {
        if (xSemaphoreTake(xMutex, timeOut) == pdPASS)
        {
            printf("acc %.1f %.1f %.1f\n", rV.accX, rV.accY, rV.accZ);
            // printf("gyr %d %d %d\n", rV.gyrX, rV.gyrY, rV.gyrZ);
            xSemaphoreGive(xMutex);
        }
        else
        {
            // do nothing
        }
        vTaskDelay(D_LCD);
    }
}

void lcdTask()
{
    // initial setup for lcd
    lcd_i2c_begin();
    lcd_init();
    lcd_clear();
    char line1[17], line2[17];

    while (true)
    {
        if (xSemaphoreTake(xMutex, timeOut) == pdPASS)
        {
            // sprintf(line1, "acc %.1f %.1f %.1f", rV.accX, rV.accY, rV.accZ);
            sprintf(line2, "gyr %d %d %d", rV.gyrX, rV.gyrY, rV.gyrZ);

            xSemaphoreGive(xMutex);
        }
        else
        {
            // do nothing
        }
        lcd_set_cursor(0, 0);
        lcd_send_string(line1);
        lcd_set_cursor(1, 0);
        lcd_send_string(line2);
        vTaskDelay(1000);
    }
}

void app_main(void)
{
    // setup
    mpu_setup(MPU6050_ACCEL_RANGE_2G, MPU6050_GYRO_RANGE_250DPS, true);
    mpuSetFilterBandwidth(MPU6050_BAND_21_HZ);
    xMutex = xSemaphoreCreateMutex();
    xTaskCreate(mpuTask, "MPU6050", 1024 * 8, NULL, 2, NULL);
    vTaskDelay(1000);
    // xTaskCreate(lcdTask, "LCD", 1024 * 8, NULL, 1, NULL);
    xTaskCreate(testTask, "TEST", 1024 * 8, NULL, 1, NULL);
    vTaskDelay(100);

    // // Khởi tạo I2C và LCD
    // lcd_i2c_begin();
    // lcd_init();

    // // Xóa màn hình
    // lcd_clear();

    // // Dòng 1: Hiển thị ACC và các giá trị X, Y, Z
    // lcd_set_cursor(0, 0); // Đặt con trỏ ở dòng 1, cột 0
    // lcd_send_string("ACC   X  Y  Z");

    // lcd_set_cursor(1, 0); // Dòng 2: Hiển thị GYR và các giá trị X, Y, Z
    // lcd_send_string("GYR   X  Y  Z");

    // // Cập nhật giá trị từ cảm biến
    // char buffer[16];

    // // Dòng 1: Giá trị ACC
    // lcd_set_cursor(0, 5); // Vị trí bắt đầu hiển thị giá trị X, Y, Z
    // sprintf(buffer, "%.1f %.1f %.1f", accel_x, accel_y, accel_z);
    // lcd_send_string(buffer);

    // // Dòng 2: Giá trị GYR
    // lcd_set_cursor(1, 5); // Vị trí bắt đầu hiển thị giá trị X, Y, Z
    // sprintf(buffer, "%.1f %.1f %.1f", gyro_x, gyro_y, gyro_z);
    // lcd_send_string(buffer);
}
