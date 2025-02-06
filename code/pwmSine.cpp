#include <driver/gpio.h>
#include <driver/ledc.h>
#include <driver/timer.h>
#include <math.h>

// Change to your desired GPIO pin
#define OUTPUT_PIN GPIO_NUM_18
#define SINE_TABLE_SIZE 20
// 10 MHz
#define PWM_FREQUENCY 10000000
// 500 kHz
#define SINE_WAVE_FREQUENCY 500000

// // Predefined sine wave lookup table (scaled to 0-255)
// const uint8_t sineTable[SINE_TABLE_SIZE] = {
//   128, 130, 132, 134, 136, 138, 140, 142, 144, 146, 148, 150, 152, 154, 156, 158,
//   160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182, 184, 186, 188, 190,
//   192, 194, 196, 198, 200, 202, 204, 206, 208, 210, 212, 214, 216, 218, 220, 222,
//   224, 226, 228, 230, 232, 234, 236, 238, 240, 242, 244, 246, 248, 250, 252, 254,
//   254, 252, 250, 248, 246, 244, 242, 240, 238, 236, 234, 232, 230, 228, 226, 224,
//   222, 220, 218, 216, 214, 212, 210, 208, 206, 204, 202, 200, 198, 196, 194, 192,
//   190, 188, 186, 184, 182, 180, 178, 176, 174, 172, 170, 168, 166, 164, 162, 160,
//   158, 156, 154, 152, 150, 148, 146, 144, 142, 140, 138, 136, 134, 132, 130, 128,
//   126, 124, 122, 120, 118, 116, 114, 112, 110, 108, 106, 104, 102, 100, 98, 96,
//   94, 92, 90, 88, 86, 84, 82, 80, 78, 76, 74, 72, 70, 68, 66, 64,
//   62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 38, 36, 34, 32,
//   30, 28, 26, 24, 22, 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0,
//   0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26, 28, 30,
//   32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62,
//   64, 66, 68, 70, 72, 74, 76, 78, 80, 82, 84, 86, 88, 90, 92, 94,
//   96, 98, 100, 102, 104, 106, 108, 110, 112, 114, 116, 118, 120, 122, 124, 126
// };

// Predefined sine wave lookup table (scaled to 0-255)
const uint8_t sineTable[SINE_TABLE_SIZE] = {
  128, 144, 160, 174, 186, 196, 204, 210, 214, 216,
  218, 210, 204, 196, 186, 174, 160, 144, 128, 112
};

volatile uint32_t index = 0;

// Set amplitude scale to 50%
float modulationIndex = 0.5;

void setupPWM() {
  // Configure the LEDC for PWM
  ledc_timer_config_t timerConfig;
  timerConfig.speed_mode = LEDC_HIGH_SPEED_MODE;
  timerConfig.timer_num = LEDC_TIMER_0;
  // 8-bit resolution
  timerConfig.duty_resolution = LEDC_TIMER_8_BIT;
  // Set PWM frequency to 10 MHz
  timerConfig.freq_hz = PWM_FREQUENCY;
  timerConfig.clk_cfg = LEDC_AUTO_CLK;
  ledc_timer_config(&timerConfig);

  ledc_channel_config_t channelConfig;
  channelConfig.speed_mode = LEDC_HIGH_SPEED_MODE;
  channelConfig.channel = LEDC_CHANNEL_0;
  channelConfig.intr_type = LEDC_INTR_DISABLE;
  channelConfig.timer_sel = LEDC_TIMER_0;
  // Start with 0 duty
  channelConfig.duty = 0;
  // Set the output pin
  channelConfig.gpio_num = OUTPUT_PIN;
  channelConfig.hpoint = 0;
  ledc_channel_config(&channelConfig);
}

void IRAM_ATTR onTimer() {
  // Calculate the scaled duty cycle using the modulation index
  uint8_t baseValue = 0; // Minimum value (0)
  uint8_t maxValue = 255; // Maximum value (255)
  
  // Scale the sine value based on the modulation index
  uint8_t scaledDuty = (uint8_t)(baseValue + (sineTable[index] - baseValue) * modulationIndex);
  
  ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, scaledDuty);
  ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

  // Loop through the sine table
  index = (index + 1) % SINE_TABLE_SIZE; // Updated to use 20 points
}

void setup() {
  // Initialize GPIO
  gpio_set_direction(OUTPUT_PIN, GPIO_MODE_OUTPUT);

  // Setup PWM
  setupPWM();

  // Configure the timer
  timer_config_t config;
  config.alarm_en = TIMER_ALARM_EN;
  config.auto_reload = true;
  config.counter_dir = TIMER_COUNT_UP;
  config.counter_en = TIMER_START;
  // Set the timer divider for 1 MHz
  config.divider = 80; // 80 MHz / 80 = 1 MHz
  config.intr_type = TIMER_INTR_LEVEL;
  config.timer_group = TIMER_GROUP_0;
  config.timer_idx = TIMER_0;

  timer_init(config.timer_group, config.timer_idx, &config);
  
  // Set the timer alarm value for 500 kHz
  timer_set_alarm_value(config.timer_group, config.timer_idx, (80e6 / SINE_WAVE_FREQUENCY) / SINE_TABLE_SIZE);
  
  // Enable the timer interrupt
  timer_enable_intr(config.timer_group, config.timer_idx);
  timer_isr_register(config.timer_group, config.timer_idx, onTimer, NULL, 0, NULL);
}

void loop() {
  // Main loop can be empty or used for other tasks
}