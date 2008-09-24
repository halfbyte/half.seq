/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 */

#include <avr/io.h>
#include <stdlib.h>
#include "lcd.h"
#include "ledm.h"
#include "buttonm.h"
#include <avr/interrupt.h>
 
int main (void)
{
  char chardata[8] = {0xFF,0x00,0xFF,0x00,0xFF,0x00,0xFF,0x00};
  int i,j,k;
  char buffer[50];
  volatile char ledm_buffer[8] = {1,2,4,8,16,32,64,128};
  volatile char encoders[16];

  encoder_buffer = &encoders;
  button_buffer = &ledm_buffer;
  
  lcd_init();
  
  ledm_init();
  butm_init();
  ledm_syncBuffer(ledm_buffer);
  lcd_redefineChar(1, chardata);
  lcd_setCursor(0,0);
  lcd_dataWrite(1);
  lcd_setCursor(7,2);

  lcd_print("Hallo");
  
  for(i=0;i<16;i++) encoders[i] = 0;
  
  sei();
  
  i=0;
  while(1) {
    if (i % 100 == 0) {
      //butm_read();
      lcd_setCursor(0,0);
      lcd_print(itoa(i, buffer, 10));
      lcd_setCursor(0,1);
      lcd_print("                                ");
      //butm_read();
      for (j=0;j<2;j++) {
        for(k=0;k<4;k++) {
          lcd_setCursor(k*4,j+1);
          lcd_print(itoa(encoders[j*4+k], buffer, 10));
        }
      }
      ledm_syncBuffer(ledm_buffer);
    }
    //butm_read();
  
    
    
    i++;
  }
}
