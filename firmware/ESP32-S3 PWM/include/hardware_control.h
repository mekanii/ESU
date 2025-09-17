#ifndef HARDWARE_CONTROL_H
#define HARDWARE_CONTROL_H

#include <Arduino.h>
#include <driver/rmt.h>
#include <driver/adc.h>
#include "config.h"

// Button state variables
extern bool lastButtonState[4];
extern bool currentButtonState[4];
extern bool buttonPressed[4];
extern unsigned long lastDebounceTime[4];

// Mode configuration arrays
extern uint16_t dutyCycles[6];
extern uint16_t totalPeriods[6];
extern uint16_t upperLimit[6];

// RMT configuration
extern rmt_channel_t channel;
extern rmt_item32_t items[20];
extern uint8_t item_num;
extern uint16_t duration0;
extern uint16_t duration1;

// Function declarations
void setupADC();
int readREM();
bool setRelay(int mode);
void fire(int mode);
void rmtStop();
void setupRelay();
void setupButtons();
bool updateButtonState(int buttonIndex, int buttonPin);
void readButtons();

#endif // HARDWARE_CONTROL_H
