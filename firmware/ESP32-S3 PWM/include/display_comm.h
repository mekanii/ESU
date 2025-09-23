#ifndef DISPLAY_COMM_H
#define DISPLAY_COMM_H

#include <Arduino.h>
#include "config.h"

// Data frame for display communication
extern unsigned char dataFrameTx[16];
extern uint8_t rxBuffer[9];
extern uint16_t sp20_30_40_x;
extern uint16_t sp50_60_70_x;

// Function declarations
bool switchMainPage();
bool setMainPageVp();
void readSerialData();

#endif // DISPLAY_COMM_H