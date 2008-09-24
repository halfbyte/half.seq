
#define LEDM_PORT PORTB
#define LEDM_CS 1
#define LEDM_DDR DDRB



void ledm_init();

void ledm_cls();

void ledm_dataWrite(char chip, char address, char data);
void ledm_rowWrite(char chip, int row, char data);
void ledm_SPIwrite(char value);
void ledm_syncBuffer(char *buffer);
