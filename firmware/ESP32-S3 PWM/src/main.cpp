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

#define RMT_TX_CHANNEL RMT_CHANNEL_0

const gpio_num_t PWM_OUT = (gpio_num_t)1;

int dutyCycle = 100;
int totalPeriod = 200;
int duration0 = 0;
int duration1 = 0;

rmt_item32_t items[1];

void update() {
  // Calculate durations based on duty cycle
  duration0 = dutyCycle;
  duration1 = totalPeriod - duration0;
  // Set up the PWM signal
  items[0].duration0 = duration0;
  items[0].level0 = 1;
  items[0].duration1 = duration1;
  items[0].level1 = 0;
  rmt_write_items(RMT_TX_CHANNEL, items, 1, true);
}

void setup() {
  Serial.begin(115200);

  rmt_config_t rmt_tx;
  rmt_tx.channel = RMT_TX_CHANNEL;
  rmt_tx.gpio_num = PWM_OUT;
  rmt_tx.mem_block_num = 1;
  rmt_tx.tx_config.carrier_en = 0;
  // rmt_tx.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
  rmt_tx.tx_config.idle_output_en = false;
  rmt_tx.clk_div = 1; // 80 MHz ÷ 1 = 80 MHz
  rmt_tx.tx_config.loop_en = 1;
  rmt_config(&rmt_tx);
  rmt_driver_install(RMT_TX_CHANNEL, 0, 0);
  
  update();
}

int readSerialData() {
  if (Serial.available() > 0) {
    int value = Serial.parseInt();
    Serial.println(value);
    
    if (value != dutyCycle) {
      dutyCycle = value;
      update();
    }
    return value;
  }
  return dutyCycle;
}

void loop() {
    dutyCycle = readSerialData();
}