#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>
#include "config.h"

// Setup/teardown
void setupBuzzer();

// Tones
void buzzerOff();
void buzzerCut();    // low tone
void buzzerCoag();   // low tone, different
void buzzerError();  // two consecutive high tones

#endif // BUZZER_H