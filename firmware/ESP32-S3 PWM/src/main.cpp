#include <Arduino.h>
#include <driver/rmt.h>

#define RMT_TX_CHANNEL_0 RMT_CHANNEL_0
#define RMT_TX_CHANNEL_1 RMT_CHANNEL_1
#define SENS_CUT  4
#define SENS_COAG 5
#define MSD1      16
#define MSD2      15

const gpio_num_t PWM_0 = (gpio_num_t)1;
const gpio_num_t PWM_1 = (gpio_num_t)2;

// Button state variables (only 2 buttons now: mode1, mode3)
bool lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
bool currentButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
bool buttonPressed[4] = {false, false, false, false};  // Track if button is currently pressed
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};
const unsigned long debounceDelay = 50; // 50ms debounce delay

// Default duty cycles for each mode (pure, cut1, cut2, spray, forced, standard)
int dutyCycles[6] = {50, 50, 50, 50, 50, 50};

// Total periods for each mode (pure, cut1, cut2, spray, forced, standard)
int totalPeriods[6] = {200, 200, 200, 400, 500, 200};

// Current mode and duty cycle (start with mode 1)
int currentMode[2] = {1, 3};

int duration0 = 0;
int duration1 = 0;

rmt_channel_t channel = RMT_TX_CHANNEL_0;
rmt_item32_t items[20];
int item_num = 20;

void fire(int mode) {
  // Stop any ongoing transmission before starting a new one
  rmt_tx_stop(RMT_TX_CHANNEL_0);
  rmt_tx_stop(RMT_TX_CHANNEL_1);

  // Calculate durations based on duty cycle
  duration0 = dutyCycles[mode];
  // Set up the PWM signal
  switch (mode) {
    case 0:
      duration1 = totalPeriods[0] - duration0;
      items[0].duration0 = duration0;
      items[0].level0 = 1;
      items[0].duration1 = duration1;
      items[0].level1 = 0;

      item_num = 1;

      channel = RMT_TX_CHANNEL_0;
      break;
    case 1:
      duration1 = totalPeriods[1] - duration0;
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
      duration1 = totalPeriods[2] - duration0;
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
      duration1 = totalPeriods[3] - duration0;
      items[0].duration0 = duration0;
      items[0].level0 = 1;
      items[0].duration1 = duration1;
      items[0].level1 = 0;

      item_num = 1;

      channel = RMT_TX_CHANNEL_1;
      break;
    case 4:
      duration1 = totalPeriods[4] - duration0;
      items[0].duration0 = duration0;
      items[0].level0 = 1;
      items[0].duration1 = duration1;
      items[0].level1 = 0;

      item_num = 1;

      channel = RMT_TX_CHANNEL_1;
      break;
    case 5:
      duration1 = totalPeriods[0] - duration0;
      items[0].duration0 = duration0;
      items[0].level0 = 1;
      items[0].duration1 = duration1;
      items[0].level1 = 0;

      item_num = 1;

      channel = RMT_TX_CHANNEL_0;
      break;
    default:
      // Default to mode 1 if invalid mode is provided
      duration1 = totalPeriods[0] - duration0;
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

void rmtStop() {
  rmt_tx_stop(RMT_TX_CHANNEL_0);
  rmt_tx_stop(RMT_TX_CHANNEL_1);
}

void setupButtons() {
  // Configure button pins as input
  pinMode(SENS_CUT, INPUT);
  pinMode(SENS_COAG, INPUT);

  pinMode(MSD1, INPUT);
  pinMode(MSD2, INPUT);

  // Initialize button states by reading current state
  delay(10); // Small delay to let pins stabilize
  lastButtonState[0] = digitalRead(SENS_CUT);
  lastButtonState[1] = digitalRead(SENS_COAG);
  lastButtonState[2] = digitalRead(MSD1);
  lastButtonState[3] = digitalRead(MSD2);
  currentButtonState[0] = lastButtonState[0];
  currentButtonState[1] = lastButtonState[1];
  currentButtonState[2] = lastButtonState[2];
  currentButtonState[3] = lastButtonState[3];
}

bool updateButtonState(int buttonIndex, int buttonPin) {
  bool reading = digitalRead(buttonPin);
  bool buttonJustPressed = false;

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState[buttonIndex]) {
    // reset the debouncing timer
    lastDebounceTime[buttonIndex] = millis();
  }

  if ((millis() - lastDebounceTime[buttonIndex]) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce delay,
    // so take it as the actual current state:

    // if the button state has changed:
    if (reading != currentButtonState[buttonIndex]) {
      currentButtonState[buttonIndex] = reading;

      if (currentButtonState[buttonIndex] == LOW) {
        // Check if any other button is already pressed before allowing this press
        bool otherButtonPressed = false;
        for (int i = 0; i < 4; i++) {
          if (i != buttonIndex && buttonPressed[i]) {
            otherButtonPressed = true;
            break;
          }
        }

        if (!otherButtonPressed) {
          // Button pressed (HIGH to LOW transition for active LOW) and no other button is pressed
          buttonPressed[buttonIndex] = true;
          buttonJustPressed = true;
        }
        // If another button is pressed, ignore this button press
      } else {
        // Button released (LOW to HIGH transition for active LOW)
        if (buttonPressed[buttonIndex]) {  // Only stop if this button was actually active
          buttonPressed[buttonIndex] = false;
          // Stop RMT transmission when button is released
          rmt_tx_stop(RMT_TX_CHANNEL_0);
          rmt_tx_stop(RMT_TX_CHANNEL_1);
        }
      }
    }
  }

  lastButtonState[buttonIndex] = reading;
  return buttonJustPressed;
}

void readButtons() {
  // Check if mode 1 button (SENS_CUT) was just pressed
  if (updateButtonState(0, SENS_CUT) || updateButtonState(2, MSD1)) {
    // Fire update once when button is first pressed (RMT will auto-loop)
    fire(currentMode[0]);
  }

  // Check if mode 3 button (SENS_COAG) was just pressed
  if (updateButtonState(1, SENS_COAG) || updateButtonState(3, MSD2)) {
    // Fire update once when button is first pressed (RMT will auto-loop)
    fire(currentMode[1]);
  }
}

void readSerialData() {
  if (Serial.available() > 0) {
    int data0 = 0;
    int data1 = 0;
    int data2 = 0;

    String serialData = Serial.readStringUntil('\n');
    
    int firstSpaceIndex = serialData.indexOf(' ');
    int secondSpaceIndex = serialData.indexOf(' ', firstSpaceIndex + 1);

    if (firstSpaceIndex > 0 && secondSpaceIndex > firstSpaceIndex) {
      // Three data values
      int data0 = serialData.substring(0, firstSpaceIndex).toInt();
      int data1 = serialData.substring(firstSpaceIndex + 1, secondSpaceIndex).toInt();
      int data2 = serialData.substring(secondSpaceIndex + 1).toInt();

      if (data0 == 1) {
        if (data1 == 0 || data1 == 1 || data1 == 2 ||data1 == 5 ) {
          if (data2 >= 0 && data2 <= 200) {
            rmtStop();
            dutyCycles[data1] = data2;
            Serial.println("00");
          } else {
            Serial.println("02");
          }
        }
        else if (data1 == 3) {
          if (data2 >= 0 && data2 <= 400) {
            rmtStop();
            dutyCycles[data1] = data2;
            Serial.println("00");
          } else {
            Serial.println("02");
          }
        }
        else if (data1 == 4) {
          if (data2 >= 0 && data2 <= 500) {
            rmtStop();
            dutyCycles[data1] = data2;
            Serial.println("00");
          } else {
            Serial.println("02");
          }
        }
        else {
          Serial.println("02");
        }
      }
    } else if (firstSpaceIndex > 0) {
      // Two data values (fallback to current logic)
      int data0 = serialData.substring(0, firstSpaceIndex).toInt();
      int data1 = serialData.substring(firstSpaceIndex + 1).toInt();
      
      if (data0 == 0) {
        if (data1 == 0 || data1 == 1 || data1 == 2) {
          rmtStop();
          currentMode[0] = data1;
          Serial.println("00");
        } else if (data1 == 3 || data1 == 4 || data1 == 5) {
          rmtStop();
          currentMode[1] = data1;
          Serial.println("00");
        } else {
          Serial.println("02");
        }
      } else {
        Serial.println("02");
      }
    } else {
      int data0 = serialData.toInt();
      if (data0 == 2) {
        rmtStop();
        Serial.println("00");
      } else {
        Serial.println("02");
      }
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
}

void loop() {
  readSerialData();  // Keep serial input functionality
  readButtons();     // Add button input functionality
}