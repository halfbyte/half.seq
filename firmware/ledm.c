#include <avr/io.h>
#include <util/delay.h>

#include "ledm.h"

void ledm_init() {
  
  LEDM_DDR |= _BV(LEDM_CS) | _BV(3) | _BV(5);
  LEDM_PORT |= _BV(LEDM_CS);

  ledm_dataWrite(0x09, 0x00);
  ledm_dataWrite(0x0A, 0x01);
  ledm_dataWrite(0x0B, 0x07);
  ledm_cls();
  ledm_dataWrite(0x0C, 0x01);
  
}

void ledm_cls() {
  int i;
  for(i=0;i<8;i++) {
    ledm_rowWrite(i, 0x00);
  }  
}

void ledm_syncBuffer(char *buffer) {
  int i;
  for(i=0;i<8;i++) {
    ledm_rowWrite(i, buffer[i]);
  }
}

void ledm_rowWrite(int row, char data) {
  if (row > 7) return;
  ledm_dataWrite(row+1, data);
}

void ledm_dataWrite(char address, char data) {
  LEDM_PORT &= ~_BV(LEDM_CS);
  ledm_SPIwrite(address);
  ledm_SPIwrite(data);
  LEDM_PORT |= _BV(LEDM_CS);
}



void ledm_SPIwrite(char value) {
  char clr;
  SPCR = _BV(SPE)|_BV(MSTR)|_BV(SPR0);
  clr=SPSR;
  clr=SPDR;
  
  SPDR = value;                    // Start the transmission
  while (!(SPSR & (1<<SPIF)));    // Wait for the end of the transmission
}
