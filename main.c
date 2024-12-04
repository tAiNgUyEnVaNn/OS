#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

// Include the library
#include <mpu6050.h>

void app_main()
{
    printf("Starting MPU6050!\n");
    // bool true parameter to install the I2C driver
    mpuBegin(MPU6050_ACCEL_RANGE_2G, MPU6050_GYRO_RANGE_250DPS, true);
    mpuSetFilterBandwidth(MPU6050_BAND_21_HZ);

    while (1)
    {
        esp_err_t ret = mpuReadSensors();
        if (ret == ESP_OK)
        {
            printf("Temperature: %.2f°C\n", mpuGetTemperature());
            printf("Acceleration X: %.2f\n", mpuGetAccelerationX());
            printf("Acceleration Y: %.2f\n", mpuGetAccelerationY());
            printf("Acceleration Z: %.2f\n", mpuGetAccelerationZ());
            printf("Gyroscope X: %.2f\n", mpuGetGyroscopeX());
            printf("Gyroscope Y: %.2f\n", mpuGetGyroscopeY());
            printf("Gyroscope Z: %.2f\n", mpuGetGyroscopeZ());
        }
        else
        {
            printf("Read sensors failed! Error: %s\n", esp_err_to_name(ret));
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

