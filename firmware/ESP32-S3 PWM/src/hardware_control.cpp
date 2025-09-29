#include "hardware_control.h"
#include "config.h"
#include "data_manager.h"
#include "display_comm.h"

// Button state variables (only 2 buttons now: mode1, mode3)
bool lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
bool currentButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
static bool buttonReadyOnPress[4] = {false, false, false, false};
bool buttonPressed[4] = {false, false, false, false};  // Track if button is currently pressed
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};


// Default duty cycles for each mode (pure, cut1, cut2, spray, forced, standard)
// uint16_t dutyCycles[6] = {50, 50, 50, 50, 50, 50};

// Total periods for each mode (pure, cut1, cut2, spray, forced, standard)
uint16_t totalPeriods[6] = {200, 200, 200, 800, 1000, 200};

uint16_t upperLimit[6] = {100, 100, 100, 90, 60, 60};

// Current mode and duty cycle (start with mode 1)
uint16_t duration0 = 0;
uint16_t duration1 = 0;

rmt_channel_t channel = RMT_TX_CHANNEL_0;
rmt_item32_t items[20];
uint8_t item_num = 20;

void setupADC() {
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);
}

int readREM() {
  int adc_value = adc1_get_raw(ADC_CHANNEL);
  // Serial.print("REM: ");
  // Serial.println(adc_value);
  return adc_value;
}

bool setRelay(int mode) {
  switch (mode) {
    case 0:
      digitalWrite(CTL_RLY_1, LOW);
      digitalWrite(CTL_RLY_2, LOW);
      break;
    case 1:
      digitalWrite(CTL_RLY_1, LOW);
      digitalWrite(CTL_RLY_2, LOW);
      break;
    case 2:
      digitalWrite(CTL_RLY_1, LOW);
      digitalWrite(CTL_RLY_2, LOW);
      break;
    case 3:
      digitalWrite(CTL_RLY_1, LOW);
      digitalWrite(CTL_RLY_2, HIGH);
      break;
    case 4:
      digitalWrite(CTL_RLY_1, LOW);
      digitalWrite(CTL_RLY_2, HIGH);
      break;
    case 5:
      digitalWrite(CTL_RLY_1, HIGH);
      digitalWrite(CTL_RLY_2, HIGH);
      break;
    default:
      digitalWrite(CTL_RLY_1, LOW);
      digitalWrite(CTL_RLY_2, LOW);
      break;
  }

  delay(100);
  return true;
}

void fire(int mode) {
  // Stop any ongoing transmission before starting a new one
  rmt_tx_stop(RMT_TX_CHANNEL_0);
  rmt_tx_stop(RMT_TX_CHANNEL_1);

  // Calculate durations based on duty cycle
  // duration0 = dutyCycles[mode];

  // Wait for setRelay to complete and return true
  if (setRelay(mode)) {
    // Set up the PWM signal
    switch (mode) {
      case 0:
        duration0 = vp52[0];
        duration1 = totalPeriods[0] - duration0;
        items[0].duration0 = duration0;
        items[0].level0 = 1;
        items[0].duration1 = duration1;
        items[0].level1 = 0;

        item_num = 1;

        channel = RMT_TX_CHANNEL_0;
        break;
      case 1:
        duration0 = vp52[1];
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
        duration0 = vp52[3];
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
      duration0 = vp52[3];
        duration1 = totalPeriods[3] - duration0;
        items[0].duration0 = duration0;
        items[0].level0 = 1;
        items[0].duration1 = duration1;
        items[0].level1 = 0;

        item_num = 1;

        channel = RMT_TX_CHANNEL_1;
        break;
      case 4:
        duration0 = vp52[4];
        duration1 = totalPeriods[4] - duration0;
        items[0].duration0 = duration0;
        items[0].level0 = 1;
        items[0].duration1 = duration1;
        items[0].level1 = 0;

        item_num = 1;

        channel = RMT_TX_CHANNEL_1;
        break;
      case 5:
        duration0 = vp52[5];
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
      default:
        // Default to mode 1 if invalid mode is provided
        duration0 = vp52[0];
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
}

void rmtStop() {
  rmt_tx_stop(RMT_TX_CHANNEL_0);
  rmt_tx_stop(RMT_TX_CHANNEL_1);
}

void setupRelay() {
  pinMode(CTL_RLY_1, OUTPUT);
  pinMode(CTL_RLY_2, OUTPUT);
  pinMode(CTL_PWM_EN, OUTPUT);

  digitalWrite(CTL_RLY_1, LOW);
  digitalWrite(CTL_RLY_2, LOW);
  digitalWrite(CTL_PWM_EN, LOW);
  
  delay(1000);

  digitalWrite(CTL_PWM_EN, HIGH);
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
          rmtStop();
          if (buttonReadyOnPress[buttonIndex]) {
            switchMainPage();
          }
          buttonReadyOnPress[buttonIndex] = false;
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
    if (!getVp51()) {
      buttonReadyOnPress[0] = false;
      buttonReadyOnPress[2] = false;
      return;
    }

    if (!getVp52()) {
      buttonReadyOnPress[0] = false;
      buttonReadyOnPress[2] = false;
      return;
    }

    if (!fireSignalState(0)) {
      buttonReadyOnPress[0] = false;
      buttonReadyOnPress[2] = false;
      return;
    }

    buttonReadyOnPress[0] = true;
    buttonReadyOnPress[2] = true;
    
    // Fire update once when button is first pressed (RMT will auto-loop)
    fire(vp51[0]);
    // return true;
  }

  // Check if mode 3 button (SENS_COAG) was just pressed
  if (updateButtonState(1, SENS_COAG) || updateButtonState(3, MSD2)) {
    if (!getVp51()) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      return;
    }

    if (!getVp52()) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      return;
    }

    if (!fireSignalState(1)) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      return;
    }

    buttonReadyOnPress[1] = true;
    buttonReadyOnPress[3] = true;

    // Fire update once when button is first pressed (RMT will auto-loop)
    fire(vp51[1]);
    // return true;
  }
}