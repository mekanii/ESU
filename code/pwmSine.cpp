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

// Predefined sine wave lookup table (scaled to 0-255)
const float sineTable[SINE_TABLE_SIZE] = {
  151.22, 194.57, 228.23, 248.37, 255.00, 249.14, 232.56, 208.34, 178.25, 144.84, 110.16, 76.76, 46.67, 22.44, 5.86, 0.00, 6.63, 26.78, 60.44, 103.79
};

volatile uint32_t index = 0;

// Set amplitude scale to 100%
float scaleFactor = 0.0;

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
  // Scale the duty cycle based on the scale factor
  uint8_t scaledDuty = (( sineTable[index] - 127.5 ) * scaleFactor / 100 ) + 127.5;
  
  ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, scaledDuty);
  ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

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