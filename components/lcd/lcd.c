#include "lcd.h"

#define SLAVE_ADDRESS_LCD 0x4E >> 1 // change this according to ur setup
#define I2C_NUM I2C_NUM_0

static const char *TAG = "lcd";

// Step by step sending command or data to slave
// <S> (addr)<A6-A5-A4-A3-A2-A1-A0-R/W> <ACK> (data)[<P7-P6-P5-P4-P3-P2-P1-P0> <ACK>...]<P>

//
// rs = 0/1 ---> Instruction register select/ Data register select
// en = 0/1 ---> Save data to DDRAM (Display Data RAM)<Not permission to read/write>/ Enable to read/write

void lcd_i2c_begin()
{
    i2c_config_t lcd_i2c_conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    ESP_ERROR_CHECK(i2c_param_config(I2C_MASTER_NUM, &lcd_i2c_conf));
    esp_err_t check = i2c_driver_install(I2C_MASTER_NUM, lcd_i2c_conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
    ESP_ERROR_CHECK(check);
}

void lcd_send_cmd(char cmd)
{
    esp_err_t ret;
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (cmd & 0xf0); // get 4 high bit
    data_l = ((cmd << 4) & 0xf0);
    data_t[0] = data_u | 0x0C; // en=1, rs=0
    data_t[1] = data_u | 0x08; // en=0, rs=0
    data_t[2] = data_l | 0x0C; // en=1, rs=0
    data_t[3] = data_l | 0x08; // en=0, rs=0
    ret = i2c_master_write_to_device(I2C_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000);
    if (ret != 0)
        ESP_LOGI(TAG, "Error in sending command");
}

void lcd_send_data(char data)
{
    esp_err_t ret;
    char data_u, data_l;
    uint8_t data_t[4];
    data_u = (data & 0xf0);
    data_l = ((data << 4) & 0xf0);
    data_t[0] = data_u | 0x0D; // en=1, rs=1
    data_t[1] = data_u | 0x09; // en=0, rs=1
    data_t[2] = data_l | 0x0D; // en=1, rs=1
    data_t[3] = data_l | 0x09; // en=0, rs=1
    ret = i2c_master_write_to_device(I2C_NUM, SLAVE_ADDRESS_LCD, data_t, 4, 1000);
    if (ret != 0)
        ESP_LOGI(TAG, "Error in sending data");
}

void lcd_clear(void)
{
    lcd_send_cmd(CLR_DISPLAY);
    usleep(5000);
}

void lcd_set_cursor(int row, int col)
{
    switch (row)
    {
    case 0:
        col |= 0x80;
        break;
    case 1:
        col |= 0xC0;
        break;
    }

    lcd_send_cmd(col);
}

void lcd_init(void)
{
    // 4 bit initialisation
    usleep(DELAY_POWER_ON); // wait for >40ms
    lcd_send_cmd(LCD_INIT_CMD);
    usleep(DELAY_SET_INIT_1); // wait for >4.1ms
    lcd_send_cmd(LCD_INIT_CMD);
    usleep(DELAY_SET_INIT_2); // wait for >100us
    lcd_send_cmd(LCD_INIT_CMD);
    usleep(10000);
    lcd_send_cmd(FOUR_BIT_MODE); // 4bit mode
    usleep(10000);

    // dislay initialisation
    lcd_send_cmd(FUNCTION_SET); // Function set --> DL=0 (4 bit mode), N = 1 (2 line display) F = 0 (5x8 characters)
    usleep(1000);
    lcd_send_cmd(DISPLAY_OFF); // Display on/off control --> D=0,C=0, B=0  ---> display off
    usleep(1000);
    lcd_send_cmd(CLR_DISPLAY); // clear display
    usleep(1000);
    usleep(1000);
    lcd_send_cmd(ENTRY_SET); // Entry mode set --> I/D = 1 (increment cursor) & S = 0 (no shift)
    usleep(1000);
    lcd_send_cmd(DISPLAY_ON); // Display on/off control --> D = 1, C and B = 0. (Cursor and blink, last two bits)
    usleep(1000);
}

void lcd_send_string(char *str)
{
    while (*str)
        lcd_send_data(*str++);
}
