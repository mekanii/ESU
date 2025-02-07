#include <Arduino.h>

#include <driver/gpio.h>
#include <driver/ledc.h>
#include <driver/timer.h>
#include <math.h>

// Change to your desired GPIO pin
#define OUTPUT_PIN GPIO_NUM_18
#define SINE_TABLE_SIZE 20
#define SINE_WAVE_FREQUENCY 500000

#define LEDC_TIMER_CFG_SPEED_MODE LEDC_LOW_SPEED_MODE
#define LEDC_TIMER_CFG_TIMER_NUM LEDC_TIMER_0
#define LEDC_TIMER_CFG_DUTY_RESOLUTION LEDC_TIMER_8_BIT
#define LEDC_TIMER_CFG_FREQ_HZ 10000000
#define LEDC_TIMER_CFG_CLK_CFG LEDC_AUTO_CLK

#define LEDC_CHANNEL_CFG_CHANNEL LEDC_CHANNEL_0
#define LEDC_CHANNEL_CFG_INTR_TYPE LEDC_INTR_DISABLE

#define TIMER_CFG_ALARM_EN TIMER_ALARM_EN
#define TIMER_CFG_AUTO_RELOAD TIMER_AUTORELOAD_EN
#define TIMER_CFG_COUNTER_DIR TIMER_COUNT_UP
#define TIMER_CFG_COUNTER_EN TIMER_START
#define TIMER_CFG_INTR_TYPE TIMER_INTR_LEVEL

#define TIMER_GROUP TIMER_GROUP_0
#define TIMER_IDX TIMER_0

// Predefined sine wave lookup table (scaled to 0-255)
const float sineTable[SINE_TABLE_SIZE] = {
  151.22, 194.57, 228.23, 248.37, 255.00, 249.14, 232.56, 208.34, 178.25, 144.84, 110.16, 76.76, 46.67, 22.44, 5.86, 0.00, 6.63, 26.78, 60.44, 103.79
};

volatile uint32_t index = 0;

// Set amplitude scale to 100%
float scaleFactor = 0.0;

void setupPWM() {
  // Configure the LEDC for PWM
  ledc_timer_config_t timerCfg;
  timerCfg.speed_mode = LEDC_TIMER_CFG_SPEED_MODE;
  timerCfg.timer_num = LEDC_TIMER_CFG_TIMER_NUM;
  // 8-bit resolution
  timerCfg.duty_resolution = LEDC_TIMER_CFG_DUTY_RESOLUTION;
  // Set PWM frequency to 10 MHz
  timerCfg.freq_hz = LEDC_TIMER_CFG_FREQ_HZ;
  timerCfg.clk_cfg = LEDC_TIMER_CFG_CLK_CFG;
  ledc_timer_config(&timerCfg);

  ledc_channel_config_t channelCfg;
  channelCfg.speed_mode = LEDC_TIMER_CFG_SPEED_MODE;
  channelCfg.channel = LEDC_CHANNEL_CFG_CHANNEL;
  channelCfg.intr_type = LEDC_CHANNEL_CFG_INTR_TYPE;
  channelCfg.timer_sel = LEDC_TIMER_CFG_TIMER_NUM;
  // Start with 0 duty
  channelCfg.duty = 0;
  // Set the output pin
  channelCfg.gpio_num = OUTPUT_PIN;
  channelCfg.hpoint = 0;
  ledc_channel_config(&channelCfg);
}

void IRAM_ATTR onTimer(void* arg) {
  // Scale the duty cycle based on the scale factor
  uint8_t scaledDuty = (( sineTable[index] - 127.5 ) * scaleFactor / 100 ) + 127.5;
  
  ledc_set_duty(LEDC_TIMER_CFG_SPEED_MODE, LEDC_CHANNEL_CFG_CHANNEL, scaledDuty);
  ledc_update_duty(LEDC_TIMER_CFG_SPEED_MODE, LEDC_CHANNEL_CFG_CHANNEL);

  // Loop through the sine table
  index = (index + 1) % SINE_TABLE_SIZE; // Updated to use 20 points
}

void setup() {
  Serial.begin(115200);
  
  // Initialize GPIO
  gpio_set_direction(OUTPUT_PIN, GPIO_MODE_OUTPUT);

  // Setup PWM
  setupPWM();

  // Configure the timer
  timer_config_t config;
  config.alarm_en = TIMER_CFG_ALARM_EN;
  config.auto_reload = TIMER_CFG_AUTO_RELOAD;
  config.counter_dir = TIMER_CFG_COUNTER_DIR;
  config.counter_en = TIMER_CFG_COUNTER_EN;
  // Set the timer divider for 1 MHz
  config.divider = 80; // 80 MHz / 80 = 1 MHz
  config.intr_type = TIMER_CFG_INTR_TYPE;

  timer_init(TIMER_GROUP, TIMER_IDX, &config);
  
  // Set the timer alarm value for 500 kHz
  timer_set_alarm_value(TIMER_GROUP, TIMER_IDX, (80e6 / SINE_WAVE_FREQUENCY) / SINE_TABLE_SIZE);
  
  // Enable the timer interrupt
  timer_enable_intr(TIMER_GROUP, TIMER_IDX);
  timer_isr_register(TIMER_GROUP, TIMER_IDX, onTimer, NULL, 0, NULL);
}

void loop() {
  // Main loop can be empty or used for other tasks
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n'); // Read until newline
    if (input == "s") {
      // Stop PWM
      scaleFactor = 0.0; // Set scaleFactor to 0 to stop PWM
    } else {
      // Convert input to scaleFactor
      int value = input.toInt(); // Convert string to integer
      if (value >= 1 && value <= 100) {
        scaleFactor = value / 100.0; // Set scaleFactor as a decimal
      }
    }
  }
}