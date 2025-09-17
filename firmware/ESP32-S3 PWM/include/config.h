#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <driver/rmt.h>
#include <driver/adc.h>

// RMT Channel definitions
#define RMT_TX_CHANNEL_0  RMT_CHANNEL_0
#define RMT_TX_CHANNEL_1  RMT_CHANNEL_1

// Pin definitions
#define SENS_CUT          4
#define SENS_COAG         5
#define OPTO              6
#define REM               7
#define REM_UPPER_LIMIT   694
#define MSD1              16
#define MSD2              15
#define CTL_RLY_1         8
#define CTL_RLY_2         37
#define CTL_PWM_EN        38
#define CTL_BUZZ          36

// ADC configuration
#define ADC_CHANNEL       ADC1_CHANNEL_6
#define ADC_WIDTH         ADC_WIDTH_BIT_12
#define ADC_ATTEN         ADC_ATTEN_DB_12

// LittleFS configuration
#define FORMAT_LITTLEFS_IF_FAILED true

// Data management configuration
#define MAX_DATA_SETS 10
#define JSON_DOC_SIZE 2048
#define JSON_DATA_FILE "/data.json"

// PWM configuration
const gpio_num_t PWM_0 = (gpio_num_t)1;
const gpio_num_t PWM_1 = (gpio_num_t)2;

// Button debounce configuration
const unsigned long debounceDelay = 50; // 50ms debounce delay

#endif // CONFIG_H
