#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

void lcd_init() {
  // setting data directions
  LCD_DDR |= _BV(LCD_CS) | _BV(LCD_RS) | _BV(3) | _BV(5);
  LCD_PORT |= _BV(LCD_CS) | _BV(LCD_RS);
  
  lcd_commandWrite(LCDCMD_FUN_REG_1);
  _delay_ms(2);
  lcd_commandWrite(LCDCMD_FUN_REG_1);
  _delay_ms(2);
  lcd_commandWrite(LCDCMD_BIAS_SET);
  lcd_commandWrite(LCDCMD_POWER_CTL);
  lcd_commandWrite(LCDCMD_FOLLOWER_CTL);
  lcd_commandWrite(LCDCMD_CONTRAST_SET);
  lcd_commandWrite(LCDCMD_ON);
  lcd_cls();
  lcd_commandWrite(LCDCMD_ENTRY_MODE_SET);
  lcd_home();
  
}

void lcd_print(char *text) {
  uint8_t i=0;  //fancy int.  avoids compiler warning when comparing i with strlen()'s uint8_t
  while (text[i] != 0){
    lcd_dataWrite(text[i]);
    i++;
  }  
}

void lcd_commandWrite(char value) {
  LCD_PORT &= ~(_BV(LCD_CS) | _BV(LCD_RS));
  lcd_SPIwrite(value);
  LCD_PORT |= _BV(LCD_CS);
}

void lcd_setCursor(int x, int y) {
  int address = y * 0x10 + x;
  address &= 0x7F;
  lcd_commandWrite(LCDCMD_SET_DDRAM_ADDRESS | address);
}

void lcd_dataWrite(char value) {
  LCD_PORT &= ~(_BV(LCD_CS));
  LCD_PORT |= _BV(LCD_RS);
  lcd_SPIwrite(value);
  LCD_PORT |= _BV(LCD_CS);
}

void lcd_redefineChar(char address, char *char_data) {
  int i=0;
  if (address > 7) return;
  lcd_commandWrite(LCDCMD_FUN_REG_0);
  lcd_commandWrite(LCDCMD_SET_CGRAM_ADDRESS | (address << 3));
  for(i=0;i<8;i++){lcd_dataWrite(char_data[i]);}
}

void lcd_cls() {
  lcd_commandWrite(LCDCMD_CLR);
  _delay_ms(2);  
}

void lcd_home() {
  lcd_commandWrite(LCDCMD_HOME);
  _delay_ms(2);
}

void lcd_SPIwrite(char value) {
  char clr;
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR1);
  clr=SPSR;
  clr=SPDR;
  
  SPDR = value;                    // Start the transmission
  while (!(SPSR & (1<<SPIF)));    // Wait for the end of the transmission
}

