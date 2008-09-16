
#define NUM_BUTTON_ROWS 8
#define NUM_BUTTON_COLS 8
#define NUM_ENCODER_ROWS 2
#define NUM_ENCODER_COLS 8

#define BUTM_PORT PORTD
#define BUTM_DDR DDRD

#define BUTM_CLK 2
#define BUTM_LATCH 3
#define BUTM_DATA 4
#define BUTM_CNT_DATA 5
#define BUTM_CNT_CLK 6
#define BUTM_CNT_RST 7


void butm_init();

void butm_read();
