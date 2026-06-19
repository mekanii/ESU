/*  =========================================================================================
    hardware_control.cpp — Trigger Handling, Relay Routing, and RMT Output

    Context (ESU firmware):
    This module is the execution layer that connects physical triggers (footswitch/handpiece)
    with display parameters (`vp51`, `vp52`) to produce hardware output:
    - relay routing
    - RMT waveform generation
    - buzzer feedback

    Main responsibilities:
    - Read and debounce 4 active-low trigger inputs
    - Validate fire permission and safety interlocks
    - Start/stop relay + RMT output based on selected mode

    Input index mapping:
    - [0] SENS_CUT   (footswitch CUT)
    - [1] SENS_COAG  (footswitch COAG)
    - [2] MSD1       (handpiece CUT)
    - [3] MSD2       (handpiece COAG)

    Mode index convention:
    - 0..2: CUT family
    - 3..5: COAG family (commonly derived from `vp51[1] + 3`)
========================================================================================= */

#include "hardware_control.h"
#include "config.h"
#include "data_manager.h"
#include "display_comm.h"
#include "buzzer.h"

// Button state arrays for 4 inputs:
// [0]=SENS_CUT, [1]=SENS_COAG, [2]=MSD1, [3]=MSD2
// Active-low buttons (LOW = pressed).
bool lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};
bool currentButtonState[4] = {HIGH, HIGH, HIGH, HIGH};

// True when a button press has passed all validation checks.
// On release, this flag allows switchMainPage() to run.
static bool buttonReadyOnPress[4] = {false, false, false, false};

// Tracks currently active pressed button to enforce single-button firing.
bool buttonPressed[4] = {false, false, false, false};

// Per-button debounce timestamp.
unsigned long lastDebounceTime[4] = {0, 0, 0, 0};

// PWM total period per mode index (0..5):
// 0..2 = CUT family, 3..5 = COAG family.
uint16_t totalPeriods[6] = {200, 200, 200, 800, 1000, 200};

// Optional upper limits (used by other logic/UI constraints).
uint16_t upperLimit[6] = {100, 100, 100, 90, 60, 60};

// RMT output buffers/channels.
// channel selected dynamically by signalType.
uint16_t duration0 = 0;
uint16_t duration1 = 0;

rmt_channel_t channel = RMT_TX_CHANNEL_0;
rmt_item32_t items[20];
uint8_t item_num = 20;

// Initialize ADC settings used by REM sensing.
void setupADC() {
  // Configure REM ADC input width + attenuation.
  adc1_config_width(ADC_WIDTH);
  adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN);
}

// Read REM ADC, average samples, apply fixed-point IIR filtering, and return the filtered value.
int readREM() {
  // REM reading pipeline:
  // 1) Oversample and average raw ADC
  // 2) Apply fixed-point IIR smoothing (alpha = 1/2^REM_SHIFT_FILTER)
  // 3) Return filtered value
  static bool initialized = false;
  static long scaledVal = 0;
  static int val = 0;

  long sum = 0;
  for (int i = 0; i < REM_SAMPLES; i++) {
    sum += adc1_get_raw(ADC_CHANNEL);
  }
  int raw_avg = (int)(sum >> REM_SHIFT_SAMPLES);
  
  // First call initializes filter state to avoid startup jump.
  if (!initialized) {
    scaledVal = (long)raw_avg << REM_SHIFT_FILTER;
    initialized = true;
    return raw_avg;
  }

  // IIR filter in fixed-point domain.
  scaledVal += ( ((long)raw_avg << REM_SHIFT_FILTER) - scaledVal ) >> REM_SHIFT_FILTER;

  val = (int)(scaledVal >> REM_SHIFT_FILTER);
  Serial.println(val);
  return val;
}

bool isRemFault(int remValue, uint8_t modeIndex) {
  int offset = modeIndex == 99 ? 0 : vp52[modeIndex];
  return (remValue >= REM_UPPER_LIMIT + offset || remValue <= REM_LOWER_LIMIT + offset);
}

// Build and start RMT waveform based on selected signal type.
void rmtStart(uint8_t signalType) {
  // Build RMT waveform by signal group:
  // - 0..2: CUT family on channel 0
  // - 3..4: COAG family on channel 1
  // - 5   : special pattern on channel 0
  // vp52[signalType] = high pulse width (duration0)
  // totalPeriods[...] - vp52[...] = low pulse width (duration1)

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

  // Blocking write = start transmission and wait scheduling complete.
  rmt_write_items(channel, items, item_num, true);
}

// Stop all active RMT channels and silence buzzer.
void rmtStop() {
  // Hard stop all possible TX channels and silence buzzer.
  rmt_tx_stop(RMT_TX_CHANNEL_0);
  rmt_tx_stop(RMT_TX_CHANNEL_1);
  buzzerOff();
}

// Configure relay path for the selected mode before starting waveform output.
bool setRelay(uint8_t mode) {
  // Select output path according to mode family.
  // NOTE: CTL_RLY_2 control is currently disabled/commented.

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

  // Allow relay settling before PWM starts.
  delay(100);
  return true;
}

// Safely restart firing: stop previous output, set relay route, then start RMT waveform.
void fire(uint8_t mode) {
  // Always stop previous firing first to avoid overlap/glitch.
  rmtStop();

  // Relay path must be set before enabling PWM pattern.
  if (setRelay(mode)) {
    rmtStart(mode);
  }
}

// Configure relay/PWM control pins and execute startup enable sequence.
void setupRelay() {
  // Relay/PWM enable initialization sequence.
  pinMode(CTL_RLY_1, OUTPUT);
  pinMode(CTL_RLY_2, OUTPUT);
  pinMode(CTL_PWM_EN, OUTPUT);

  digitalWrite(CTL_RLY_1, LOW);
  digitalWrite(CTL_RLY_2, LOW);
  digitalWrite(CTL_PWM_EN, LOW);
  
  // Power-up guard delay, then enable PWM stage.
  delay(1000);
  digitalWrite(CTL_PWM_EN, HIGH);
}

// Configure trigger input pins and capture initial stable states to avoid false startup edges.
void setupButtons() {
  // Configure all trigger inputs.
  pinMode(SENS_CUT, INPUT);
  pinMode(SENS_COAG, INPUT);

  pinMode(MSD1, INPUT);
  pinMode(MSD2, INPUT);

  // Capture stable initial state to prevent false edge at startup.
  delay(10);
  lastButtonState[0] = digitalRead(SENS_CUT);
  lastButtonState[1] = digitalRead(SENS_COAG);
  lastButtonState[2] = digitalRead(MSD1);
  lastButtonState[3] = digitalRead(MSD2);
  currentButtonState[0] = lastButtonState[0];
  currentButtonState[1] = lastButtonState[1];
  currentButtonState[2] = lastButtonState[2];
  currentButtonState[3] = lastButtonState[3];
}

// Debounce one input and detect valid press/release transitions with single-button lockout.
bool updateButtonState(uint8_t buttonIndex, uint8_t buttonPin) {
  // Debounced edge detector with mutual-exclusion firing lock.
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
        // Press event: only allow if no other button is currently active.
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
        // Release event: stop output + optional UI page switch.
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

// Main trigger gate: validate display state + safety conditions, then start CUT/COAG firing.
ReadButtonsResult readButtons() {
  int remValue = readREM();

  // CUT trigger source: foot switch CUT or handpiece CUT
  if (updateButtonState(0, SENS_CUT) || updateButtonState(2, MSD1)) {
    if (!getVp51()) {
      buttonReadyOnPress[0] = false;
      buttonReadyOnPress[2] = false;
      buzzerError();
      return {RESULT_NG, remValue};
    }

    if (!getVp52()) {
      buttonReadyOnPress[0] = false;
      buttonReadyOnPress[2] = false;
      buzzerError();
      return {RESULT_NG, remValue};
    }

    if (!fireSignalState(0)) {
      buttonReadyOnPress[0] = false;
      buttonReadyOnPress[2] = false;
      buzzerError();
      return {RESULT_NG, remValue};
    }

    buttonReadyOnPress[0] = true;
    buttonReadyOnPress[2] = true;
    
    // Reject CUT fire when REM fault or bipolar mismatch condition is detected.
    if (isRemFault(remValue, vp51[0]) || ((vp51[1] + 3) == 5)) {
      // REM Fault or MONOPOLAR SW (CUT) Triggered on BIPOLAR MODE
      buzzerError();
      return {RESULT_NG, remValue};
    } else {
      // Fire update once when button is first pressed (RMT will auto-loop)
      fire(vp51[0]); // CUT mode index (0..2)
      buzzerCut();
      return {RESULT_OK, remValue};
    }
  }

  // COAG trigger source: handpiece COAG
  if (updateButtonState(3, MSD2)) {
    if (!getVp51()) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      buzzerError();
      return {RESULT_NG, remValue};
    }

    if (!getVp52()) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      buzzerError();
      return {RESULT_NG, remValue};
    }

    if (!fireSignalState(1)) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      buzzerError();
      return {RESULT_NG, remValue};
    }

    buttonReadyOnPress[1] = true;
    buttonReadyOnPress[3] = true;

    // Reject COAG fire when REM fault or bipolar mismatch condition is detected.
    if (isRemFault(remValue, vp51[1] + 3) || ((vp51[1] + 3) == 5)) {
      // REM Fault or MONOPOLAR SW (COAG) Triggered on BIPOLAR MODE
      buzzerError();
      return {RESULT_NG, remValue};
    } else {
      // Fire update once when button is first pressed (RMT will auto-loop)
      fire(vp51[1] + 3); // COAG mode index (3..5)
      buzzerCoag();
      return {RESULT_OK, remValue};
    }
  }

  // COAG trigger source: foot switch COAG
  if (updateButtonState(1, SENS_COAG)) {
    if (!getVp51()) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      buzzerError();
      return {RESULT_NG, remValue};
    }

    if (!getVp52()) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      buzzerError();
      return {RESULT_NG, remValue};
    }

    if (!fireSignalState(1)) {
      buttonReadyOnPress[1] = false;
      buttonReadyOnPress[3] = false;
      buzzerError();
      return {RESULT_NG, remValue};
    }

    buttonReadyOnPress[1] = true;
    buttonReadyOnPress[3] = true;

    // Reject COAG fire when REM fault is detected.
    if (isRemFault(remValue, vp51[1] + 3)) {
      buzzerError();
      return {RESULT_NG, remValue};
    } else {
      // Fire update once when button is first pressed (RMT will auto-loop)
      fire(vp51[1] + 3); // COAG mode index (3..5)
      buzzerCoag();
      return {RESULT_OK, remValue};
    }
  }

  return {RESULT_NO_ACTION, remValue};
}