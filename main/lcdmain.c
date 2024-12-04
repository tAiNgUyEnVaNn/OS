#include <stdio.h>
#include "driver/i2c.h"
#include "lcd.h"
#include "mpu6050.h"


#define I2C_MASTER_SCL_IO GPIO_NUM_22 /*!< GPIO cho xung clock I2C SCL */
#define I2C_MASTER_SDA_IO GPIO_NUM_21 /*!< GPIO cho dữ liệu I2C SDA  */
#define I2C_MASTER_NUM 0              /*!< Cổng I2C chính của I2C */
#define I2C_MASTER_FREQ_HZ 100000     /*!< Tần số xung nhịp của I2C */
#define I2C_MASTER_TX_BUF_DISABLE 0
#define I2C_MASTER_RX_BUF_DISABLE 0

//Sau sẽ thay các giá trị đọc được từ cảm biến
float accel_x = 1.23, accel_y = 4.56, accel_z = 7.89;
float gyro_x = 0.12, gyro_y = 3.45, gyro_z = 6.78;

/**
 * @brief Hàm khởi tạo I2C
 */
static esp_err_t i2c_master_init(void)
{
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &conf));
    return i2c_driver_install(I2C_MASTER_NUM, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

void app_main(void)
{
    // Khởi tạo I2C và LCD
    ESP_ERROR_CHECK(i2c_master_init());
    lcd_init();

    // Xóa màn hình
    lcd_clear();

    // Dòng 1: Hiển thị ACC và các giá trị X, Y, Z
    lcd_set_cursor(0, 0); // Đặt con trỏ ở dòng 1, cột 0
    lcd_send_string("ACC   X  Y  Z");

    lcd_set_cursor(1, 0); // Dòng 2: Hiển thị GYR và các giá trị X, Y, Z
    lcd_send_string("GYR   X  Y  Z");

    // Cập nhật giá trị từ cảm biến
    char buffer[16];

    // Dòng 1: Giá trị ACC
    lcd_set_cursor(0, 5); // Vị trí bắt đầu hiển thị giá trị X, Y, Z
    sprintf(buffer, "%.1f %.1f %.1f", accel_x, accel_y, accel_z);
    lcd_send_string(buffer);

    // Dòng 2: Giá trị GYR
    lcd_set_cursor(1, 5); // Vị trí bắt đầu hiển thị giá trị X, Y, Z
    sprintf(buffer, "%.1f %.1f %.1f", gyro_x, gyro_y, gyro_z);
    lcd_send_string(buffer);
}
