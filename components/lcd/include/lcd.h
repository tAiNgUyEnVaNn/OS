// Push to LCD branches

#include "esp_log.h"
#include "driver/i2c.h"
#include "unistd.h"

// Delay time require, reference on page 13 of LCD datasheet <for 4-bit interferce>
#define DELAY_POWER_ON 50000  // wait time for stabilizing power supply >40ms
#define DELAY_SET_INIT_1 5000 // >4.1ms
#define DELAY_SET_INIT_2 200  // >100us

// Command. Reference on page 11 of LCD datasheet
#define LCD_INIT_CMD 0x30
#define FOUR_BIT_MODE 0x20
#define FUNCTION_SET 0x28 // 001D NF-- D=0 (4bit mode) N=1 (2 line mode) F=0 (5x8 per character) ==> 0b00101000
#define DISPLAY_OFF 0x08  // 0000 1DCB D=0,C=0, B=0  ---> display off
#define DISPLAY_ON 0x0C   // D=1,C,B=0---> display on
#define CLR_DISPLAY 0x01
#define ENTRY_SET 0x06 // 0000 01(I/D)S I/D = 1 (increment cursor) & S = 0 (no shift)
#define CLR_DISPLAY 0x01

// esp_err_t master_init();

void lcd_init(void); // initialize lcd

void lcd_send_cmd(char cmd); // send command to the lcd

void lcd_send_data(char data); // send data to the lcd

void lcd_send_string(char *str); // send string to the lcd

void lcd_set_cursor(int row, int col); // put cursor at the entered position row (0 or 1), col (0-15);

void lcd_clear(void);
