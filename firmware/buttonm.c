#include <avr/io.h>
#include "buttonm.h"
#include <avr/interrupt.h>

volatile char encoder_backup[NUM_ENCODER_ROWS * NUM_ENCODER_COLS];



void butm_init() {
  int i;
  BUTM_DDR = _BV(BUTM_CLK) | _BV(BUTM_LATCH) | _BV(BUTM_CNT_CLK) | _BV(BUTM_CNT_DATA) | _BV(BUTM_CNT_RST);
  BUTM_PORT |= _BV(BUTM_LATCH);
  BUTM_PORT &= ~(_BV(BUTM_CNT_CLK)| _BV(BUTM_CLK) | _BV(BUTM_CNT_RST));

  DDRC |= _BV(0);
  
  TCCR0A = 0;
  TCCR0B |= (1<<FOC0A)|(1<<CS00) | (1<<CS02);
  OCR0A = 4;
  TIMSK0 |= (1<<OCIE0A);
  
  for(i=0;i<NUM_ENCODER_ROWS * NUM_ENCODER_COLS;i++)
    encoder_backup[i] = 0x01;
}

ISR(TIMER0_COMPA_vect) {
  PORTC |= _BV(0);
  //BUTM_PORT |= _BV(0);
  butm_read();
  PORTC &= ~_BV(0);
}

void butm_read() {
  int i,j;
  int encoders[NUM_ENCODER_ROWS];
  int changed_encoders;

  BUTM_PORT &= ~_BV(BUTM_CNT_RST);
  BUTM_PORT |= _BV(BUTM_CNT_RST);
  BUTM_PORT |= _BV(BUTM_CNT_DATA);
  BUTM_PORT |= _BV(BUTM_CNT_CLK);
  BUTM_PORT &= ~_BV(BUTM_CNT_CLK);
  BUTM_PORT &= ~_BV(BUTM_CNT_DATA);
  
  for(j=0;j<NUM_BUTTON_ROWS;j++) {
    
    BUTM_PORT &= ~_BV(BUTM_LATCH);
    BUTM_PORT |= _BV(BUTM_LATCH);
    
    for(i=0;i<NUM_BUTTON_COLS;i++) {
      
      if (PIND & _BV(BUTM_DATA)) { 
        button_buffer[j] |= _BV(i);
      } else {
        button_buffer[j] &= ~_BV(i);
      }
      
      BUTM_PORT |= _BV(BUTM_CLK);
      BUTM_PORT &= ~_BV(BUTM_CLK);
    }
    if (j < NUM_ENCODER_ROWS) {
      // read the other two shift registers for encoder coding
      for(i=0;i<NUM_ENCODER_COLS * 2;i++) {
        if (PIND & _BV(BUTM_DATA)) { 
          encoders[j] |= (1 << i);
        } else {
          encoders[j] &= ~(1 << i);
        }
      
        BUTM_PORT |= _BV(BUTM_CLK);
        BUTM_PORT &= ~_BV(BUTM_CLK);
      }
      
    }
      
    BUTM_PORT |= _BV(BUTM_CNT_CLK);
    BUTM_PORT &= ~_BV(BUTM_CNT_CLK);      
  }
  
  // encoder processing
  for(i=0;i<NUM_ENCODER_ROWS;i++) {
    for(j=0;j<NUM_ENCODER_COLS;j++)  {
      char s = 0;
      if (encoders[i] & _BV(j*2))
        s=1;
      if (encoders[i] & _BV(j*2+1))
        s^=3;
      
      s -= encoder_backup[i*NUM_ENCODER_COLS+j];
      if (s & 1) {
        encoder_backup[i*NUM_ENCODER_COLS+j] += s;
        encoder_buffer[i*NUM_ENCODER_COLS+j] += (s & 2) - 1;
      }
    }
  }

  
  

}

  
