// #include <Arduino.h>
// // 400 kHz * 2 * 256  = 204.8 MHz
// const int PWM_CHANNEL = 0;
// const int PWM_FREQ = 400000;
// const int PWM_RESOLUTION = 6;
// const int PWM_OUT = 1;

// const int MAX_DUTY_CYCLE = (int)(pow(2, PWM_RESOLUTION) - 1); 

// const int DELAY_MS = 4;

// int dutyCycle = 32;

// void setup() {
//   Serial.begin(115200);
//   ledcSetup(PWM_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
//   ledcAttachPin(PWM_OUT, PWM_CHANNEL);
// }

// int readSerialData() {
//   if (Serial.available() > 0) {
//     int value = Serial.parseInt();
//     Serial.println(value);
//     return value;
//   }
//   return dutyCycle;
// }

// void loop() {
//   dutyCycle = readSerialData();
//   if (dutyCycle < 0) dutyCycle = 0;
//   if (dutyCycle > MAX_DUTY_CYCLE) dutyCycle = MAX_DUTY_CYCLE;
//   ledcWrite(PWM_CHANNEL, dutyCycle);
//   // delay(DELAY_MS);
// }

// =================================================================================
// =================================================================================

#include <Arduino.h>
#include <driver/rmt.h>

#define RMT_TX_CHANNEL_0 RMT_CHANNEL_0
#define RMT_TX_CHANNEL_1 RMT_CHANNEL_1

const gpio_num_t PWM_0 = (gpio_num_t)1;
const gpio_num_t PWM_1 = (gpio_num_t)2;

int totalPeriodCut = 200;
int duration0 = 0;
int duration1 = 0;

rmt_channel_t channel = RMT_TX_CHANNEL_0;
rmt_item32_t items[20];
int item_num = 20;

void update(int mode, int dutyCycle) {
  // Calculate durations based on duty cycle
  duration0 = dutyCycle;
  // Set up the PWM signal
  switch (mode) {
    case 0:
      duration1 = totalPeriodCut - duration0;
      items[0].duration0 = duration0;
      items[0].level0 = 1;
      items[0].duration1 = duration1;
      items[0].level1 = 0;

      item_num = 1;

      channel = RMT_TX_CHANNEL_0;
      break;
    case 1:
      duration1 = totalPeriodCut - duration0;
      for (int i = 0; i < 20; i++) {
        items[i].duration0 = duration0;
        items[i].level0 = i < 18 ? 1 : 0;
        items[i].duration1 = duration1;
        items[i].level1 = 0;
      }
      item_num = 20;

      channel = RMT_TX_CHANNEL_0;
      break;
    case 2:
      duration1 = totalPeriodCut - duration0;
      for (int i = 0; i < 20; i++) {
        items[i].duration0 = duration0;
        items[i].level0 = i < 17 ? 1 : 0;
        items[i].duration1 = duration1;
        items[i].level1 = 0;
      }
      item_num = 20;

      channel = RMT_TX_CHANNEL_0;
      break;
  }

  
}

void setup() {
  Serial.begin(115200);

  rmt_config_t rmt_tx_0;
  rmt_tx_0.channel = RMT_TX_CHANNEL_0;
  rmt_tx_0.gpio_num = PWM_0;
  rmt_tx_0.mem_block_num = 1;
  rmt_tx_0.tx_config.carrier_en = 0;
  rmt_tx_0.tx_config.idle_output_en = false;
  rmt_tx_0.clk_div = 1; // 80 MHz ÷ 1 = 80 MHz
  // rmt_tx.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
  // rmt_tx.tx_config.loop_en = 1;
  rmt_config(&rmt_tx_0);
  rmt_driver_install(RMT_TX_CHANNEL_0, 0, 0);

  rmt_config_t rmt_tx_1;
  rmt_tx_1.channel = RMT_TX_CHANNEL_1;
  rmt_tx_1.gpio_num = PWM_1;
  rmt_tx_1.mem_block_num = 1;
  rmt_tx_1.tx_config.carrier_en = 0;
  rmt_tx_1.tx_config.idle_output_en = false;
  rmt_tx_1.clk_div = 1; // 80 MHz ÷ 1 = 80 MHz
  rmt_config(&rmt_tx_1);
  rmt_driver_install(RMT_TX_CHANNEL_1, 0, 0);
  
  // update();
}

void readSerialData() {
  if (Serial.available() > 0) {
    String serialData = Serial.readStringUntil('\n');
    
    int spaceIndex = serialData.indexOf(' ');
    if (spaceIndex > 0) {
      int mode = serialData.substring(0, spaceIndex).toInt();
      int dutyCycle = serialData.substring(spaceIndex + 1).toInt();

      Serial.println(serialData, dutyCycle);
      
      update(mode);
    } else {
      Serial.println("Invalid input format. Please use 'mode dutyCycle'.");
    }
  }
}

void loop() {
  readSerialData();
}