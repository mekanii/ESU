#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
#include "config.h"

// Data arrays
extern uint16_t allVP51[MAX_DATA_SETS][2];
extern uint16_t allVP52[MAX_DATA_SETS][3];

// Current working arrays
/*
    vp51[0] CUT mode index
     - 0: PURE CUT
     - 1: BLEND CUT 1
     - 2: BLEND CUT 2
    
     vp51[1] COAG mode index
     - 0: SPRAY
     - 1: FORCED
     - 2: BIPOLAR STD
*/
extern uint16_t vp51[2];

/*
    vp52[0] = 0..200
    vp52[1] = 0..200
    vp52[2] = 0..200
    vp52[3] = 0..800
    vp52[4] = 0..1000
    vp52[5] = 0..200
*/
extern uint16_t vp52[6];

// Function declarations
bool initLittleFS();
bool saveToJSON();
bool loadFromJSON();
bool saveByPointer(uint16_t datasetIndex);
bool loadByPointer(uint16_t datasetIndex);

#endif // DATA_MANAGER_H
