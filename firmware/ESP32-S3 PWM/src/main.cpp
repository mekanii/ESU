#include <Arduino.h>
#include <driver/rmt.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

// Include our custom headers
#include "config.h"
#include "data_manager.h"
#include "display_comm.h"
#include "hardware_control.h"
#include "buzzer.h"

void setup() {
  setupRelay();
  setupADC();
  setupBuzzer();

  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 18, 17);

  if (LITTLEFS_EN) {
    // Initialize LittleFS with proper error handling
    if (!initLittleFS()) {
      Serial.println("LittleFS initialization failed!");
      return;  // Stop execution if LittleFS fails
    }
  }

  rmt_config_t rmt_tx_0;
  rmt_tx_0.channel = RMT_TX_CHANNEL_0;
  rmt_tx_0.gpio_num = PWM_0;
  rmt_tx_0.mem_block_num = 1;
  rmt_tx_0.tx_config.carrier_en = 0;
  rmt_tx_0.tx_config.idle_output_en = true;
  rmt_tx_0.clk_div = 1; // 80 MHz ÷ 1 = 80 MHz
  // rmt_tx_0.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
  rmt_tx_0.tx_config.loop_en = 1;
  rmt_config(&rmt_tx_0);
  rmt_driver_install(RMT_TX_CHANNEL_0, 0, 0);

  rmt_config_t rmt_tx_1;
  rmt_tx_1.channel = RMT_TX_CHANNEL_1;
  rmt_tx_1.gpio_num = PWM_1;
  rmt_tx_1.mem_block_num = 1;
  rmt_tx_1.tx_config.carrier_en = 0;
  rmt_tx_1.clk_div = 4; // 80 MHz ÷ 4 = 20 MHz
  // rmt_tx_1.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
  rmt_tx_1.tx_config.idle_output_en = 1;
  rmt_tx_1.tx_config.loop_en = 1;
  rmt_config(&rmt_tx_1);
  rmt_driver_install(RMT_TX_CHANNEL_1, 0, 0);

  rmt_set_tx_intr_en(RMT_TX_CHANNEL_0, 1);
  rmt_set_tx_intr_en(RMT_TX_CHANNEL_1, 1);

  setupButtons();
  
  if (LITTLEFS_EN) {
    // Load saved data from JSON on startup
    loadFromJSON();
  
    switchMainPage();
    setMainPageVp();
  }
}

void loop() {
  static bool errorREM = false;
  readSerialData();  // Keep serial input functionality

  if(readREM() < REM_UPPER_LIMIT) {
    errorREM = false;
    readButtons();   // Add button input functionality
  } else {
    if (!errorREM) {
      buzzerError();
      errorREM = true;
    }
  }
}