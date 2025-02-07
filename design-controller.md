### 1. Sine PWM (SPWM)
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

- `OUTPUT_PIN`:
  <br>Defines the GPIO pin number (GPIO 18) that will be used to output the PWM signal. You can change this to any other pin as needed.
- `SINE_TABLE_SIZE`:
  <br>Defines the number of points in the sine wave lookup table. In this case, it is set to 20, meaning there are 20 pre-calculated values in the sine wave array.
- `PWM_FREQUENCY`:
  <br>Sets the frequency of the PWM signal to 10 MHz. This is the rate at which the PWM signal will toggle on and off.
- `SINE_WAVE_FREQUENCY`:
  <br>This sets the frequency of the sine wave to 500 kHz. This is the rate at which the sine wave will be sampled and output.
```cpp
#define OUTPUT_PIN GPIO_NUM_18
#define SINE_TABLE_SIZE 20
#define PWM_FREQUENCY 10000000
#define SINE_WAVE_FREQUENCY 500000
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
  <br>This structure holds the configuration for the PWM timer.
  - `speed_mode`
    <br>Sets the speed mode to high-speed, allowing for higher frequency PWM.
  - `timer_num`
    <br>Specifies which timer to use (in this case, timer 0).
  - `duty_resolution`
    <br>Sets the resolution of the PWM signal to 8 bits (0-255).
  - `freq_hz`
    <br>Sets the frequency of the PWM signal to the defined PWM_FREQUENCY.
  - `clk_cfg`
    <br>Configures the clock source for the timer.
- `ledc_channel_config_t channelConfig`
  <br>This structure holds the configuration for the PWM channel.
  - `channel`
    <br>Specifies which channel to use (channel 0).
  - `intr_type`
    <br>Disables interrupts for this channel.
  - `timer_sel`
    <br>Selects the timer to use for this channel.
  - `duty`
    <br>Initializes the duty cycle to 0 (no output).
  - `gpio_num`
    <br>Sets the GPIO pin for output.
The function calls `ledc_timer_config()` and `ledc_channel_config()` to apply the configurations.
```cpp
void setupPWM() {
  ledc_timer_config_t timerConfig;
  timerConfig.speed_mode = LEDC_HIGH_SPEED_MODE;
  timerConfig.timer_num = LEDC_TIMER_0;
  timerConfig.duty_resolution = LEDC_TIMER_8_BIT;
  timerConfig.freq_hz = PWM_FREQUENCY;
  timerConfig.clk_cfg = LEDC_AUTO_CLK;
  ledc_timer_config(&timerConfig);

  ledc_channel_config_t channelConfig;
  channelConfig.speed_mode = LEDC_HIGH_SPEED_MODE;
  channelConfig.channel = LEDC_CHANNEL_0;
  channelConfig.intr_type = LEDC_INTR_DISABLE;
  channelConfig.timer_sel = LEDC_TIMER_0;
  channelConfig.duty = 0;
  channelConfig.gpio_num = OUTPUT_PIN;
  channelConfig.hpoint = 0;
  ledc_channel_config(&channelConfig);
}
```

###### Timer Interrupt Handler
This function is called every time the timer interrupt occurs. It calculates the PWM duty cycle based on the current sine wave value and the `scaleFactor`. The duty cycle is then updated to the PWM channel.
```cpp
void IRAM_ATTR onTimer() {
  uint8_t scaledDuty = ((sineTable[index] - 127.5) * scaleFactor / 100) + 127.5;
  ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0, scaledDuty);
  ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_0);

  index = (index + 1) % SINE_TABLE_SIZE;
}
```

###### Setup Function
Initializes serial communication for input, sets up GPIO, configures PWM, and initializes the timer.
```cpp
void setup() {
  Serial.begin(115200);
  ...
}
```

###### Main Loop
Continuously checks for serial input. If 's' is received, it stops the PWM by setting `scaleFactor` to 0. If a number between 1 and 100 is received, it sets the `scaleFactor` to that value divided by 100, allowing for a range of 0.01 to 1.00.
```cpp
void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    ...
  }
}
```

##### How It Works
- When the program starts, it initializes the PWM and timer.
- The timer generates interrupts at a specified frequency, calling the `onTimer()` function to update the PWM signal based on the sine wave lookup table.
- The user can send commands via serial input to adjust the amplitude of the PWM signal or stop it entirely.

##### Full Code
```cpp
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
  uint8_t scaledDuty = (( sineTable[index] - 127.5 ) * scaleFactor / 100 ) + 127.5
  
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
```


#### 1.4. Measurement