### 1. Sine PWM (SPWM): lookup table, timer interrupt, and LEDC timer
#### 1.1. Graph
The image below presents two graphs that illustrate the relationship between a sine wave and its PWM (Pulse Width Modulation) representation:
- Left Graph
<br>This graph displays a sine wave with a frequency of 500 kHz, sampled using a sawtooth waveform at a sampling frequency of 10 MHz. The sawtooth waveform serves as a reference for capturing the sine wave's shape, allowing for a detailed representation of its amplitude variations over time.

- Right Graph
<br>This graph shows the PWM output generated from the same 500 kHz sine wave, also sampled at 10 MHz. The PWM output is derived from the sampling of the sine wave using the sawtooth waveform, effectively converting the continuous sine wave into a discrete signal that can be used for various applications, such as motor control or audio signal generation.

The graphs include five distinct rows, each representing a different amplitude level of the sine wave:
- Amplitude 100%
<br>This row illustrates the full-scale representation of the sine wave, where the peak values reach the maximum output level.

- Amplitude 50%
<br>This row shows the sine wave scaled down to half of its maximum amplitude, demonstrating how the waveform's height is reduced while maintaining its shape.

- Amplitude 25%
<br>In this row, the sine wave is further scaled down to a quarter of its maximum amplitude, highlighting the diminishing height of the waveform.

- Amplitude 10%
<br>This row depicts the sine wave at a significantly reduced amplitude, showcasing the waveform's ability to represent very low signal levels while still retaining its sinusoidal characteristics.

- Amplitude 2%
<br>The final row illustrates the sine wave at just 2% of its maximum amplitude, emphasizing the precision of the PWM output even at minimal signal levels.

The comparison between the left and right graphs provides valuable insights into how the sine wave is transformed into a PWM signal. The varying amplitude levels across the five rows demonstrate the flexibility and accuracy of PWM in representing different signal strengths, making it a crucial technique in in various electronic applications.

<p align="center">
  <img src="pic/Graph/SPWM%20graph.png" alt="spwm-graph">
</p>
This table is based on PWM values obtained from graph drawings. These values were visually represented and may have slight variations due to the nature of graphical representation.
<p align="center">
  <img src="pic/Table/SPWM%20table%20-%20based%20on%20graph.png" alt="spwm-table">
</p>

#### 1.2. Calculation
This table is derived from calculations based on the PWM values. It reflects the results of mathematical computations that take into account various factors such as scaling and zero crossing.
```
Scaled Value[i] = ((PWM Value[i] - Zero Crossing) x Scaling Factor ) + Zero Crossing
Scaled Value[i] = ((PWM Value[i] - 127.5) x Scaling Factor ) + 127.5
```
where PWM Value[i] is based on the 100% PWM Value from the graph
<p align="center">
  <img src="pic/Table/SPWM%20table%20-%20scaling%20calculation.png" alt="spwm-calc">
</p>
In the current table, certain rows are highlighted in color to indicate that the values in these rows are either the same as or closely approaching the corresponding values from the previous table. This visual cue helps to quickly identify areas where the calculated PWM values align well with those derived from the graphical representation.

The proximity of values between the two tables suggests that the calculations performed in the current table effectively replicate or approximate the PWM values that were originally drawn from the graph. This is important for validating the accuracy of the calculations and ensuring that the PWM signals generated will closely match the intended design.

The alignment of values in the colored rows reinforces the reliability of the current table's calculations, indicating that the mathematical model used is consistent with the graphical data. This consistency is crucial for applications where precise PWM control is necessary.

#### 1.3. Code
This code is intended for an embedded system (like an ESP32) that generates a PWM signal based on a sine wave. The PWM signal can be adjusted in amplitude using a scale factor that can be set via serial input. The code also allows stopping the PWM signal.
##### Key Components
###### Libraries and Definitions
- `<driver/gpio.h>`:
<br>Provides functions to control General Purpose Input/Output (GPIO) pins. It allows you to set pin modes (input/output) and read/write values to/from the pins.
- `<driver/ledc.h>`:
<br>Used for configuring the LED Controller (LEDC) for PWM (Pulse Width Modulation) output. It allows you to set up timers and channels for PWM signals.
- `<driver/timer.h>`:
<br>Provides functions to configure and manage hardware timers. Timers are essential for generating periodic interrupts, which are used to update the PWM signal.
- `<math.h>`:
<br>Includes mathematical functions. Although not explicitly used in this snippet, it is often included for mathematical operations.
```cpp
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <driver/timer.h>
#include <math.h>
```

- `OUTPUT_PIN`
  <br>Defines the GPIO pin number (GPIO 18) that will be used to output the PWM signal. You can change this to any other pin as needed.
- `SINE_TABLE_SIZE`
  <br>Defines the number of points in the sine wave lookup table. In this case, it is set to 20, meaning there are 20 pre-calculated values in the sine wave array.
- `SINE_WAVE_FREQUENCY`
  <br>This sets the frequency of the sine wave to 500 kHz. This is the rate at which the sine wave will be sampled and output.
```cpp
#define OUTPUT_PIN GPIO_NUM_18
#define SINE_TABLE_SIZE 20
#define SINE_WAVE_FREQUENCY 500000
```

- LEDC Timer Configuration Constants:
  - `LEDC_TIMER_CFG_SPEED_MODE`
    <br>Sets the speed mode for the LEDC timer to low speed.
  - `LEDC_TIMER_CFG_TIMER_NUM`
    <br>Specifies which timer to use (timer 0).
  - `LEDC_TIMER_CFG_DUTY_RESOLUTION`
    <br>Sets the resolution of the PWM signal to 8 bits (0-255).
  - `LEDC_TIMER_CFG_FREQ_HZ`
    <br>Sets the PWM frequency to 10 MHz.
  - `LEDC_TIMER_CFG_CLK_CFG`
    <br>Configures the clock source for the timer.
```cpp
#define LEDC_TIMER_CFG_SPEED_MODE LEDC_LOW_SPEED_MODE
#define LEDC_TIMER_CFG_TIMER_NUM LEDC_TIMER_0
#define LEDC_TIMER_CFG_DUTY_RESOLUTION LEDC_TIMER_8_BIT
#define LEDC_TIMER_CFG_FREQ_HZ 10000000
#define LEDC_TIMER_CFG_CLK_CFG LEDC_AUTO_CLK
```

- LEDC Channel Configuration Constants:
  - `LEDC_CHANNEL_CFG_CHANNEL`
    <br>Specifies which channel to use (channel 0).
  - `LEDC_CHANNEL_CFG_INTR_TYPE`
    <br>Disables interrupts for this channel.
```cpp
#define LEDC_CHANNEL_CFG_CHANNEL LEDC_CHANNEL_0
#define LEDC_CHANNEL_CFG_INTR_TYPE LEDC_INTR_DISABLE
```

- Timer Configuration Constants:
  - `TIMER_CFG_ALARM_EN`
    <br>Enables the timer alarm.
  - `TIMER_CFG_AUTO_RELOAD`
    <br>Allows the timer to automatically reload after reaching the alarm value.
  - `TIMER_CFG_COUNTER_DIR`
    <br>Sets the timer to count up.
  - `TIMER_CFG_COUNTER_EN`
    <br>Starts the timer.
  - `TIMER_CFG_INTR_TYPE`
    <br>Sets the interrupt type to level-triggered.
```cpp
#define TIMER_CFG_ALARM_EN TIMER_ALARM_EN
#define TIMER_CFG_AUTO_RELOAD TIMER_AUTORELOAD_EN
#define TIMER_CFG_COUNTER_DIR TIMER_COUNT_UP
#define TIMER_CFG_COUNTER_EN TIMER_START
#define TIMER_CFG_INTR_TYPE TIMER_INTR_LEVEL
```

- Timer Group and Index Constants:
  - `TIMER_GROUP`
    <br>Specifies the timer group to use (group 0).
  - `TIMER_IDX`
    <br>Specifies the timer index to use (timer 0).
```cpp
#define TIMER_GROUP TIMER_GROUP_0
#define TIMER_IDX TIMER_0
```

###### Sine Wave Lookup Table
This array contains 20 pre-calculated values of a sine wave, scaled to fit within the range of 0-255. Each value corresponds to a point in the sine wave, allowing for smooth PWM output. The values are derived from the sine function and scaled to match the 8-bit resolution of the PWM signal (0-255).

```cpp
const float sineTable[SINE_TABLE_SIZE] = {
  151.22, 194.57, 228.23, 248.37, 255.00, 249.14, 232.56, 208.34, 178.25, 144.84, 110.16, 76.76, 46.67, 22.44, 5.86, 0.00, 6.63, 26.78, 60.44, 103.79
};
```

###### Global Variables
- `volatile uint32_t index`
  <br>This variable keeps track of the current index in the sine wave lookup table. It is marked as `volatile` because it is modified within an interrupt service routine (ISR), ensuring the compiler does not optimize it away.
- `float scaleFactor`
  <br>This variable determines the amplitude of the PWM signal. It is initialized to 0, meaning the PWM output will be off until changed by user input.
```cpp
volatile uint32_t index = 0;
float scaleFactor = 0.0;
```

###### PWM Setup Function
This function configures the LEDC (LED Controller) for PWM output. It sets the timer, resolution, frequency, and the output pin.
- `ledc_timer_config_t timerConfig`
  <br>This structure holds the configuration for the PWM timer. The various fields are set using the defined constants for speed mode, timer number, duty resolution, frequency, and clock configuration.
- `ledc_channel_config_t channelConfig`
  <br>This structure holds the configuration for the PWM channel. The fields are set using the defined constants for speed mode, channel, interrupt type, timer selection, initial duty cycle, and GPIO pin.
The function calls `ledc_timer_config()` and `ledc_channel_config()` to apply the configurations.
```cpp
void setupPWM() {
  ledc_timer_config_t timerCfg;
  timerCfg.speed_mode = LEDC_TIMER_CFG_SPEED_MODE;
  timerCfg.timer_num = LEDC_TIMER_CFG_TIMER_NUM;
  timerCfg.duty_resolution = LEDC_TIMER_CFG_DUTY_RESOLUTION;
  timerCfg.freq_hz = LEDC_TIMER_CFG_FREQ_HZ;
  timerCfg.clk_cfg = LEDC_TIMER_CFG_CLK_CFG;
  ledc_timer_config(&timerCfg);

  ledc_channel_config_t channelCfg;
  channelCfg.speed_mode = LEDC_TIMER_CFG_SPEED_MODE;
  channelCfg.channel = LEDC_CHANNEL_CFG_CHANNEL;
  channelCfg.intr_type = LEDC_CHANNEL_CFG_INTR_TYPE;
  channelCfg.timer_sel = LEDC_TIMER_CFG_TIMER_NUM;
  channelCfg.duty = 0; // Start with 0 duty
  channelCfg.gpio_num = OUTPUT_PIN; // Set the output pin
  channelCfg.hpoint = 0;
  ledc_channel_config(&channelCfg);
}
```

###### Timer Interrupt Handler
This function is called every time the timer interrupt occurs. It updates the PWM signal based on the current sine wave value.
- `uint8_t scaledDuty`
  <br>This variable calculates the duty cycle for the PWM signal.
  <br>The formula adjusts the sine wave value to fit within the 0-255 range, scaled by the scaleFactor. The sine wave values are centered around 127.5 to ensure they fit within the PWM range.
- `ledc_set_duty()`
  <br>This function sets the duty cycle for the specified PWM channel.
- `ledc_update_duty()`
  <br>This function updates the PWM output with the new duty cycle.
- `index = (index + 1) % SINE_TABLE_SIZE`
  <br>This line increments the index to move to the next value in the sine wave table. The modulo operation ensures that the index wraps around to 0 when it reaches the end of the table, creating a continuous loop.
```cpp
void IRAM_ATTR onTimer(void* arg) {
  uint8_t scaledDuty = ((sineTable[index] - 127.5) * scaleFactor / 100) + 127.5;
  
  ledc_set_duty(LEDC_TIMER_CFG_SPEED_MODE, LEDC_CHANNEL_CFG_CHANNEL, scaledDuty);
  ledc_update_duty(LEDC_TIMER_CFG_SPEED_MODE, LEDC_CHANNEL_CFG_CHANNEL);

  index = (index + 1) % SINE_TABLE_SIZE;
}
```

###### Setup Function
Initializes serial communication for input, sets up GPIO, configures PWM, and initializes the timer.
```cpp
void setup() {
  Serial.begin(115200);
  
  gpio_set_direction(OUTPUT_PIN, GPIO_MODE_OUTPUT);

  setupPWM();

  timer_config_t config;
  config.alarm_en = TIMER_CFG_ALARM_EN;
  config.auto_reload = TIMER_CFG_AUTO_RELOAD;
  config.counter_dir = TIMER_CFG_COUNTER_DIR;
  config.counter_en = TIMER_CFG_COUNTER_EN;
  config.divider = 80;
  config.intr_type = TIMER_CFG_INTR_TYPE;

  timer_init(TIMER_GROUP, TIMER_IDX, &config);
  
  timer_set_alarm_value(TIMER_GROUP, TIMER_IDX, (80e6 / SINE_WAVE_FREQUENCY) / SINE_TABLE_SIZE);
  
  timer_enable_intr(TIMER_GROUP, TIMER_IDX);
  timer_isr_register(TIMER_GROUP, TIMER_IDX, onTimer, NULL, 0, NULL);
}
```

###### Main Loop
Continuously checks for serial input. If 's' is received, it stops the PWM by setting `scaleFactor` to 0. If a number between 1 and 100 is received, it sets the `scaleFactor` to that value divided by 100, allowing for a range of 0.01 to 1.00.
```cpp
void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    if (input == "s") {
      scaleFactor = 0.0;
    } else {
      int value = input.toInt();
      if (value >= 1 && value <= 100) {
        scaleFactor = value / 100.0;
      }
    }
  }
}
```

##### How It Works
- When the program starts, it initializes the PWM and timer.
- The timer generates interrupts at a specified frequency, calling the `onTimer()` function to update the PWM signal based on the sine wave lookup table.
- The user can send commands via serial input to adjust the amplitude of the PWM signal or stop it entirely.

##### Full Code
```cpp
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
```

#### 1.4. Measurement

### 2. Sine PWM (SPWM): MCPWM
#### 2.1 Overview
When creating software related to the hardware in the SPWM generation process, we need to consider several factors, such as project specifications, and the appropriate use of I/O ports, timers, interrupts, and operators in the MCPWM module.

<p align="center">
  <img src="pic/MCPWM%20module.jpg" alt="mcpwm-module">
</p>