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
const int BUTTON_MODE_1 = 16;     // Button for mode 1 (Cut mode pattern 1)
const int BUTTON_MODE_3 = 15;     // Button for mode 3 (Spray mode)

// Button state variables (only 2 buttons now: mode1, mode3)
bool lastButtonState[2] = {HIGH, HIGH};
bool currentButtonState[2] = {HIGH, HIGH};
unsigned long lastDebounceTime[2] = {0, 0};
const unsigned long debounceDelay = 50; // 50ms debounce delay

// Current mode and duty cycle (start with mode 1)
int currentMode = 1;
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
  rmt_tx_stop(RMT_TX_CHANNEL_0);
  rmt_tx_stop(RMT_TX_CHANNEL_1);
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
    default:
      // Default to mode 1 if invalid mode is provided
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
  }

  rmt_write_items(channel, items, item_num, true);
}

void setupButtons() {
  // Configure button pins as input with internal pull-up resistors
  pinMode(BUTTON_MODE_1, INPUT_PULLUP);
  pinMode(BUTTON_MODE_3, INPUT_PULLUP);
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
  // Check mode buttons (only mode 1 and mode 3)
  if (readButtonWithDebounce(0, BUTTON_MODE_1)) {
    currentMode = 1;
    Serial.println("Mode 1 selected (Cut pattern 1)");
    update(currentMode, currentDutyCycle);
  }

  if (readButtonWithDebounce(1, BUTTON_MODE_3)) {
    currentMode = 3;
    Serial.println("Mode 3 selected (Spray)");
    update(currentMode, currentDutyCycle);
  }
}

void readSerialData() {
  if (Serial.available() > 0) {
    String serialData = Serial.readStringUntil('\n');
    
    int spaceIndex = serialData.indexOf(' ');
    if (spaceIndex > 0) {
      int mode = serialData.substring(0, spaceIndex).toInt();
      int dutyCycle = serialData.substring(spaceIndex + 1).toInt();

      Serial.println(serialData);

      if (mode == 5) {
        rmt_tx_stop(RMT_TX_CHANNEL_0);
        rmt_tx_stop(RMT_TX_CHANNEL_1);
      } else {
        update(mode, dutyCycle);
      }
      
    } else {
      Serial.println("Invalid input format. Please use 'mode dutyCycle'.");
    }
  }
}

void setup() {
  Serial.begin(115200);

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
  rmt_tx_1.clk_div = 8; // 80 MHz ÷ 8 = 10 MHz
  // rmt_tx_1.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
  rmt_tx_1.tx_config.idle_output_en = 1;
  rmt_tx_1.tx_config.loop_en = 1;
  rmt_config(&rmt_tx_1);
  rmt_driver_install(RMT_TX_CHANNEL_1, 0, 0);

  rmt_set_tx_intr_en(RMT_TX_CHANNEL_0, 1);
  rmt_set_tx_intr_en(RMT_TX_CHANNEL_1, 1);

  // Setup buttons
  setupButtons();

  Serial.println("ESP32-S3 PWM Controller Ready");
  Serial.println("Button Controls:");
  Serial.println("- Pin 16: Mode 1 (Cut pattern 1)");
  Serial.println("- Pin 15: Mode 3 (Spray)");
  Serial.println("Serial Commands - All modes available:");
  Serial.println("- Mode 0: Cut basic");
  Serial.println("- Mode 1: Cut pattern 1");
  Serial.println("- Mode 2: Cut pattern 2");
  Serial.println("- Mode 3: Spray");
  Serial.println("- Mode 4: Forced");
  Serial.println("Serial format: 'mode dutyCycle' (e.g., '0 100', '1 150', '2 120', '3 200', '4 250')");

  // update();
}

void loop() {
  readSerialData();  // Keep serial input functionality
  // readButtons();     // Add button input functionality
}