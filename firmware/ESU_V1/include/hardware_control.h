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
void rmtStart(uint8_t signalType);
void rmtStop();
bool setRelay(uint8_t mode);
void fire(uint8_t mode);
void setupRelay();
void setupButtons();
bool updateButtonState(uint8_t buttonIndex, uint8_t buttonPin);
bool readButtons(bool remFault = false);

#endif // HARDWARE_CONTROL_H
