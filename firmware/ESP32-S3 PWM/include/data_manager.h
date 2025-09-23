#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "config.h"

// Data arrays
extern uint16_t allVP81[MAX_DATA_SETS][2];
extern uint16_t allVP82[MAX_DATA_SETS][3];
extern uint16_t allVP83[MAX_DATA_SETS][3];

// Current working arrays
extern uint16_t vp81[2];
extern uint16_t vp82[3];
extern uint16_t vp83[3];

// Function declarations
bool initLittleFS();
bool saveToJSON();
bool loadFromJSON();
bool saveByPointer(uint16_t datasetIndex);
bool loadByPointer(uint16_t datasetIndex);

#endif // DATA_MANAGER_H
