#include "hardware_control.h"
#include "config.h"
#include "data_manager.h"
#include "display_comm.h"
#include "buzzer.h"

// Button state variables (only 2 buttons now: mode1, mode3)
bool lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
bool currentButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
static bool buttonReadyOnPress[4] = {false, false, false, false};
bool buttonPressed[4] = {false, false, false, false};  // Track if button is currently pressed
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};

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
  static bool initialized = false;
  static long scaledVal = 0;
  static int val = 0;

  long sum = 0;
  for (int i = 0; i < REM_SAMPLES; i++) {
    sum += adc1_get_raw(ADC_CHANNEL);
  }
  int raw_avg = (int)(sum >> REM_SHIFT_SAMPLES);
  
  if (!initialized) {
    scaledVal = (long)raw_avg << REM_SHIFT_FILTER;
    initialized = true;
    return raw_avg;
  }

  scaledVal += ( ((long)raw_avg << REM_SHIFT_FILTER) - scaledVal ) >> REM_SHIFT_FILTER;

  val = (int)(scaledVal >> REM_SHIFT_FILTER);
  // Serial.println(val);
  return val;
}

void rmtStart(uint8_t signalType) {
  if (signalType == 0 || signalType == 1 || signalType == 2) {
    item_num = signalType == 0 ? 1 : 20;
    for (int i = 0; i < item_num; i++) {
      items[i].duration0 = vp52[signalType];
      items[i].level0 = i < (signalType == 0 ? 1 : (signalType == 1 ? 18 : 17)) ? 1 : 0;
      items[i].duration1 = totalPeriods[signalType] - vp52[signalType];
      items[i].level1 = 0;
    }
    channel = RMT_TX_CHANNEL_0;
  } else if (signalType == 3 || signalType == 4) {
    item_num = 1;
    items[0].duration0 = vp52[signalType];
    items[0].level0 = 1;
    items[0].duration1 = totalPeriods[signalType] - vp52[signalType];
    items[0].level1 = 0;
    channel = RMT_TX_CHANNEL_1;
  } else if (signalType == 5) {
    item_num = 20;
    for (int i = 0; i < 20; i++) {
      items[i].duration0 = vp52[signalType];
      items[i].level0 = i < 18 ? 1 : 0;
      items[i].duration1 = totalPeriods[1] - vp52[signalType];
      items[i].level1 = 0;
    }
    channel = RMT_TX_CHANNEL_0;
  }
  rmt_write_items(channel, items, item_num, true);
}

void rmtStop() {
  rmt_tx_stop(RMT_TX_CHANNEL_0);
  rmt_tx_stop(RMT_TX_CHANNEL_1);
  buzzerOff();
}

bool setRelay(uint8_t mode) {
  // Serial.println(mode);
  if (mode == 0 || mode == 1 || mode == 2) {
    digitalWrite(CTL_RLY_1, HIGH);
    // digitalWrite(CTL_RLY_2, LOW);
  } else if (mode == 3 || mode == 4) {
    digitalWrite(CTL_RLY_1, HIGH);
    // digitalWrite(CTL_RLY_2, HIGH);
  } else if (mode == 5){
    digitalWrite(CTL_RLY_1, LOW);
    // digitalWrite(CTL_RLY_2, HIGH);
  }

  delay(100);
  return true;
}

void fire(uint8_t mode) {
  // Stop any ongoing transmission before starting a new one
  rmtStop();

  // Wait for setRelay to complete and return true
  if (setRelay(mode)) {
    rmtStart(mode);
  }
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

bool updateButtonState(uint8_t buttonIndex, uint8_t buttonPin) {
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

bool readButtons(bool remFault) {
  // Check if SENS_CUT (FOOT SW) OR MSD1 (CAUTER) was just pressed
  if (updateButtonState(0, SENS_CUT) || updateButtonState(2, MSD1)) {
    if (!getVp51()) {
      buttonReadyOnPress[0] = false;
      buttonReadyOnPress[2] = false;
      buzzerError();
      return false;
    }

    if (!getVp52()) {
      buttonReadyOnPress[0] = false;
      buttonReadyOnPress[2] = false;
      buzzerError();
      return false;
    }

    if (!fireSignalState(0)) {
      buttonReadyOnPress[0] = false;
      buttonReadyOnPress[2] = false;
      buzzerError();
      return false;
    }

    buttonReadyOnPress[0] = true;
    buttonReadyOnPress[2] = true;
    
    if (remFault || ((vp51[1] + 3) == 5)) {
      // REM Fault or MONOPOLAR SW (CUT) Triggered on BIPOLAR MODE
      buzzerError();
      return false;
    } else {
      // Fire update once when button is first pressed (RMT will auto-loop)
      fire(vp51[0]);
      buzzerCut();
      return true;
    }
  }

  // Check if MSD2 (CAUTER) was just pressed
  if (updateButtonState(3, MSD2)) {
    if (!getVp51()) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      buzzerError();
      return false;
    }

    if (!getVp52()) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      buzzerError();
      return false;
    }

    if (!fireSignalState(1)) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      buzzerError();
      return false;
    }

    buttonReadyOnPress[1] = true;
    buttonReadyOnPress[3] = true;

    if (remFault || ((vp51[1] + 3) == 5)) {
      // REM Fault or MONOPOLAR SW (COAG) Triggered on BIPOLAR MODE
      buzzerError();
      return false;
    } else {
      // Fire update once when button is first pressed (RMT will auto-loop)
      fire(vp51[1] + 3);
      buzzerCoag();
      return true;
    }
  }

  // Check if SENS_COAG (FOOT SW) was just pressed
  if (updateButtonState(1, SENS_COAG)) {
    if (!getVp51()) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      buzzerError();
      return false;
    }

    if (!getVp52()) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      buzzerError();
      return false;
    }

    if (!fireSignalState(1)) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      buzzerError();
      return false;
    }

    buttonReadyOnPress[1] = true;
    buttonReadyOnPress[3] = true;

    if (remFault) {
      buzzerError();
      return false;
    } else {
      // Fire update once when button is first pressed (RMT will auto-loop)
      fire(vp51[1] + 3);
      buzzerCoag();
      return true;
    }
  }

  return true;
}