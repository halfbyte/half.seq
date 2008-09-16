#include <avr/io.h>
#include "buttonm.h"

char encoder_backup[NUM_ENCODER_ROWS];

void butm_init() {
  BUTM_DDR = _BV(BUTM_CLK) | _BV(BUTM_LATCH) | _BV(BUTM_CNT_CLK) | _BV(BUTM_CNT_DATA) | _BV(BUTM_CNT_RST);
  BUTM_PORT |= _BV(BUTM_LATCH);
  BUTM_PORT &= ~(_BV(BUTM_CNT_CLK)| _BV(BUTM_CLK) | _BV(BUTM_CNT_RST));
}

void butm_read(char *keybuffer, char *encoder_buffer) {
  int i,j;
  int encoders[NUM_ENCODER_ROWS];
  int changed_encoders;

  BUTM_PORT &= ~_BV(BUTM_CNT_RST);
  // asm("nop\n nop\n nop\n nop\n");
  // asm("nop\n nop\n nop\n nop\n");
  BUTM_PORT |= _BV(BUTM_CNT_RST);
  // asm("nop\n nop\n nop\n nop\n");
  // asm("nop\n nop\n nop\n nop\n");
  BUTM_PORT |= _BV(BUTM_CNT_DATA);
  // asm("nop\n");
  // asm("nop\n nop\n nop\n nop\n");
  // asm("nop\n nop\n nop\n nop\n");
  BUTM_PORT |= _BV(BUTM_CNT_CLK);
  // asm("nop\n nop\n nop\n nop\n");
  // asm("nop\n nop\n nop\n nop\n");
  // asm("nop\n");
  BUTM_PORT &= ~_BV(BUTM_CNT_CLK);
  // asm("nop\n nop\n nop\n nop\n");
  // asm("nop\n nop\n nop\n nop\n");
  BUTM_PORT &= ~_BV(BUTM_CNT_DATA);
  // asm("nop\n nop\n nop\n nop\n");
  // asm("nop\n nop\n nop\n nop\n");

  
  for(j=0;j<NUM_BUTTON_ROWS;j++) {
    
    BUTM_PORT &= ~_BV(BUTM_LATCH);
    // asm("nop\n nop\n nop\n nop\n");
    // asm("nop\n nop\n nop\n nop\n");
    // asm("nop\n nop\n nop\n nop\n");
    BUTM_PORT |= _BV(BUTM_LATCH);
    // asm("nop\n nop\n nop\n nop\n");
    // asm("nop\n nop\n nop\n nop\n");
    // asm("nop\n nop\n nop\n nop\n");
    
    for(i=0;i<NUM_BUTTON_COLS;i++) {
      
      if (PIND & _BV(BUTM_DATA)) { 
        keybuffer[j] |= _BV(i);
      } else {
        keybuffer[j] &= ~_BV(i);
      }
      
      BUTM_PORT |= _BV(BUTM_CLK);
      // asm("nop\n nop\n nop\n nop\n");
      // asm("nop\n nop\n nop\n nop\n");
      // asm("nop\n nop\n nop\n nop\n");
      BUTM_PORT &= ~_BV(BUTM_CLK);
      // asm("nop\n nop\n nop\n nop\n");
      // asm("nop\n nop\n nop\n nop\n");
      // asm("nop\n nop\n nop\n nop\n");
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
        // asm("nop\n nop\n nop\n nop\n");
        // asm("nop\n nop\n nop\n nop\n");
        // asm("nop\n nop\n nop\n nop\n");
        BUTM_PORT &= ~_BV(BUTM_CLK);
        // asm("nop\n nop\n nop\n nop\n");
        // asm("nop\n nop\n nop\n nop\n");
        // asm("nop\n nop\n nop\n nop\n");
      }
      
    }
      
    BUTM_PORT |= _BV(BUTM_CNT_CLK);
    // asm("nop\n nop\n nop\n nop\n");
    // asm("nop\n nop\n nop\n nop\n");
    // asm("nop\n nop\n nop\n nop\n");
    // asm("nop\n nop\n nop\n nop\n");
    BUTM_PORT &= ~_BV(BUTM_CNT_CLK);
    // asm("nop\n nop\n nop\n nop\n");
    // asm("nop\n nop\n nop\n nop\n");
    // asm("nop\n nop\n nop\n nop\n");
      
  }
  
  // encoder processing
  for(i=0;i<NUM_ENCODER_ROWS;i++) {
    changed_encoders = (encoder_backup[i] ^ encoders[i]) & encoders[i];
    for(j=0;j<NUM_ENCODER_COLS;j++)  {
      if (changed_encoders & _BV(j*2)) {
        // aha!
        if (encoders[i] & _BV(j*2+1)) {
          encoder_buffer[i*NUM_ENCODER_COLS+j]++;
        } else {
          encoder_buffer[i*NUM_ENCODER_COLS+j]--;
        }
      }
    }
    encoder_backup[i] = encoders[i];
  }

  
  

}

  
