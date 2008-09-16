/*
** LCD display routines for DOG-M displays (SPI mode)
*/

#define LCD_PORT PORTB
#define LCD_CS 2
#define LCD_RS 0
#define LCD_DDR DDRB

#define LCDCMD_CLR 1
#define LCDCMD_HOME 2
#define LCDCMD_ON 0x0C
#define LCDCMD_FUN_REG_1 49
#define LCDCMD_FUN_REG_0 48
#define LCDCMD_BIAS_SET 28
#define LCDCMD_POWER_CTL 90
#define LCDCMD_FOLLOWER_CTL 105
#define LCDCMD_CONTRAST_SET 116
#define LCDCMD_ENTRY_MODE_SET 6
#define LCDCMD_SET_DDRAM_ADDRESS 0x80
#define LCDCMD_SET_CGRAM_ADDRESS 0x40

void lcd_init();
void lcd_print(char *text);
void lcd_commandWrite(char value);
void lcd_dataWrite(char value);
void lcd_SPIwrite(char value);
void lcd_redefineChar(char address, char *char_data);
void lcd_setCursor(int x, int y);
void lcd_cls();
void lcd_home();
