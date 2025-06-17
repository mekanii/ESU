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

// Button pin definitions
const int BUTTON_MODE_0 = 3;    // Button for mode 0 (Cut mode basic)
const int BUTTON_MODE_1 = 4;    // Button for mode 1 (Cut mode pattern 1)
const int BUTTON_MODE_2 = 5;    // Button for mode 2 (Cut mode pattern 2)
const int BUTTON_MODE_3 = 6;    // Button for mode 3 (Spray mode)
const int BUTTON_MODE_4 = 7;    // Button for mode 4 (Forced mode)
const int BUTTON_DUTY_UP = 8;   // Button to increase duty cycle
const int BUTTON_DUTY_DOWN = 9; // Button to decrease duty cycle

// Button state variables
bool lastButtonState[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
bool currentButtonState[7] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long lastDebounceTime[7] = {0, 0, 0, 0, 0, 0, 0};
const unsigned long debounceDelay = 50; // 50ms debounce delay

// Current mode and duty cycle
int currentMode = 0;
int currentDutyCycle = 50;

int totalPeriodCut = 200;
int totalPeriodSpray = 400;
int totalPeriodForced = 500;
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
    case 3:
      duration1 = totalPeriodSpray - duration0;
      items[0].duration0 = duration0;
      items[0].level0 = 1;
      items[0].duration1 = duration1;
      items[0].level1 = 0;

      item_num = 1;

      channel = RMT_TX_CHANNEL_1;
      break;
    case 4:
      duration1 = totalPeriodForced - duration0;
      items[0].duration0 = duration0;
      items[0].level0 = 1;
      items[0].duration1 = duration1;
      items[0].level1 = 0;

      item_num = 1;

      channel = RMT_TX_CHANNEL_1;
      break;
  }
  rmt_write_items(channel, items, item_num, true);
}

void setupButtons() {
  // Configure button pins as input with internal pull-up resistors
  pinMode(BUTTON_MODE_0, INPUT_PULLUP);
  pinMode(BUTTON_MODE_1, INPUT_PULLUP);
  pinMode(BUTTON_MODE_2, INPUT_PULLUP);
  pinMode(BUTTON_MODE_3, INPUT_PULLUP);
  pinMode(BUTTON_MODE_4, INPUT_PULLUP);
  pinMode(BUTTON_DUTY_UP, INPUT_PULLUP);
  pinMode(BUTTON_DUTY_DOWN, INPUT_PULLUP);
}

bool readButtonWithDebounce(int buttonIndex, int buttonPin) {
  bool reading = digitalRead(buttonPin);

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState[buttonIndex]) {
    // reset the debouncing timer
    lastDebounceTime[buttonIndex] = millis();
  }

  if ((millis() - lastDebounceTime[buttonIndex]) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != currentButtonState[buttonIndex]) {
      currentButtonState[buttonIndex] = reading;

      // only trigger on button press (HIGH to LOW transition)
      if (currentButtonState[buttonIndex] == LOW) {
        lastButtonState[buttonIndex] = reading;
        return true;
      }
    }
  }

  lastButtonState[buttonIndex] = reading;
  return false;
}

void readButtons() {
  // Check mode buttons
  if (readButtonWithDebounce(0, BUTTON_MODE_0)) {
    currentMode = 0;
    Serial.println("Mode 0 selected (Cut basic)");
    update(currentMode, currentDutyCycle);
  }

  if (readButtonWithDebounce(1, BUTTON_MODE_1)) {
    currentMode = 1;
    Serial.println("Mode 1 selected (Cut pattern 1)");
    update(currentMode, currentDutyCycle);
  }

  if (readButtonWithDebounce(2, BUTTON_MODE_2)) {
    currentMode = 2;
    Serial.println("Mode 2 selected (Cut pattern 2)");
    update(currentMode, currentDutyCycle);
  }

  if (readButtonWithDebounce(3, BUTTON_MODE_3)) {
    currentMode = 3;
    Serial.println("Mode 3 selected (Spray)");
    update(currentMode, currentDutyCycle);
  }

  if (readButtonWithDebounce(4, BUTTON_MODE_4)) {
    currentMode = 4;
    Serial.println("Mode 4 selected (Forced)");
    update(currentMode, currentDutyCycle);
  }

  // Check duty cycle adjustment buttons
  if (readButtonWithDebounce(5, BUTTON_DUTY_UP)) {
    currentDutyCycle += 10;
    if (currentDutyCycle > 500) currentDutyCycle = 500; // Limit max duty cycle
    Serial.print("Duty cycle increased to: ");
    Serial.println(currentDutyCycle);
    update(currentMode, currentDutyCycle);
  }

  if (readButtonWithDebounce(6, BUTTON_DUTY_DOWN)) {
    currentDutyCycle -= 10;
    if (currentDutyCycle < 0) currentDutyCycle = 0; // Limit min duty cycle
    Serial.print("Duty cycle decreased to: ");
    Serial.println(currentDutyCycle);
    update(currentMode, currentDutyCycle);
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
  // rmt_tx_0.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
  rmt_tx_0.tx_config.loop_en = 0;
  rmt_config(&rmt_tx_0);
  rmt_driver_install(RMT_TX_CHANNEL_0, 0, 0);

  rmt_config_t rmt_tx_1;
  rmt_tx_1.channel = RMT_TX_CHANNEL_1;
  rmt_tx_1.gpio_num = PWM_1;
  rmt_tx_1.mem_block_num = 1;
  rmt_tx_1.tx_config.carrier_en = 0;
  rmt_tx_1.tx_config.idle_output_en = false;
  rmt_tx_1.clk_div = 8; // 80 MHz ÷ 8 = 10 MHz
  // rmt_tx_1.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
  rmt_tx_1.tx_config.loop_en = 0;
  rmt_config(&rmt_tx_1);
  rmt_driver_install(RMT_TX_CHANNEL_1, 0, 0);

  // Setup buttons
  setupButtons();

  Serial.println("ESP32-S3 PWM Controller Ready");
  Serial.println("Button Controls:");
  Serial.println("- Pin 3: Mode 0 (Cut basic)");
  Serial.println("- Pin 4: Mode 1 (Cut pattern 1)");
  Serial.println("- Pin 5: Mode 2 (Cut pattern 2)");
  Serial.println("- Pin 6: Mode 3 (Spray)");
  Serial.println("- Pin 7: Mode 4 (Forced)");
  Serial.println("- Pin 8: Duty cycle UP");
  Serial.println("- Pin 9: Duty cycle DOWN");
  Serial.println("Serial format: 'mode dutyCycle' (e.g., '0 100')");

  // update();
}

void readSerialData() {
  if (Serial.available() > 0) {
    String serialData = Serial.readStringUntil('\n');
    
    int spaceIndex = serialData.indexOf(' ');
    if (spaceIndex > 0) {
      int mode = serialData.substring(0, spaceIndex).toInt();
      int dutyCycle = serialData.substring(spaceIndex + 1).toInt();

      Serial.println(serialData);
      
      update(mode, dutyCycle);
    } else {
      Serial.println("Invalid input format. Please use 'mode dutyCycle'.");
    }
  }
}

void loop() {
  readSerialData();  // Keep serial input functionality
  readButtons();     // Add button input functionality
}