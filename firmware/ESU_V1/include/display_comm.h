#ifndef DISPLAY_COMM_H
#define DISPLAY_COMM_H

#include <Arduino.h>
#include "config.h"

// Data frame for display communication
extern unsigned char dataFrameTx[16];
extern uint8_t rxBuffer[32];
extern uint16_t sp80_90_A0_x;
extern uint16_t spB0_C0_D0_x;

// Function declarations
bool switchMainPage();
bool fireSignalState(bool mode);
bool getVp51();
bool getVp52();
bool setMainPageVp();
void readSerialData();

#endif // DISPLAY_COMM_H