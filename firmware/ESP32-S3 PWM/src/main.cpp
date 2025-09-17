#include <Arduino.h>
#include <driver/rmt.h>
#include <driver/adc.h>
#include <esp_adc_cal.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#define RMT_TX_CHANNEL_0  RMT_CHANNEL_0
#define RMT_TX_CHANNEL_1  RMT_CHANNEL_1
#define SENS_CUT          4
#define SENS_COAG         5
#define OPTO              6
#define REM               7
#define REM_UPPER_LIMIT   694
#define MSD1              16
#define MSD2              15
#define CTL_RLY_1         8
#define CTL_RLY_2         37
#define CTL_PWM_EN        38
#define CTL_BUZZ          36

#define ADC_CHANNEL       ADC1_CHANNEL_6
#define ADC_WIDTH         ADC_WIDTH_BIT_12
#define ADC_ATTEN         ADC_ATTEN_DB_12

#define FORMAT_LITTLEFS_IF_FAILED true

// Maximum number of data sets in the array
#define MAX_DATA_SETS 10
#define JSON_DOC_SIZE 2048

// LittleFS file path for JSON data
#define JSON_DATA_FILE "/data.json"

// Array to hold all data sets
uint16_t allVP51[MAX_DATA_SETS][2] = {0};
uint16_t allVP52[MAX_DATA_SETS][3] = {0};
uint16_t allVP53[MAX_DATA_SETS][3] = {0};

// Current working arrays (point to current data set)
uint16_t vp51[2] = {0, 0};
uint16_t vp52[3] = {0, 0, 0};
uint16_t vp53[3] = {0, 0, 0};

// 5A A5 PAYLOAD_LENGTH INSTRUCTION VAR_ID_H VAR_ID_L DATA_LENGTH DATA_H DATA_L
uint8_t rxBuffer[9];

// uint16_t vp51[2] = {0, 0};
// uint16_t vp52[3] = {0, 0, 0};
// uint16_t vp53[3] = {0, 0, 0};

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
int totalPeriods[6] = {200, 200, 200, 800, 1000, 200};

// Current mode and duty cycle (start with mode 1)
int currentMode[2] = {1, 3};

int duration0 = 0;
int duration1 = 0;

rmt_channel_t channel = RMT_TX_CHANNEL_0;
rmt_item32_t items[20];
int item_num = 20;

unsigned char dataFrameTx[16] = {0x5A, 0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// Initialize LittleFS with proper error handling
bool initLittleFS() {
  if (!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)) {
    Serial.println("LittleFS Mount: NG");
    return false;
  } else {
    Serial.println("LittleFS Mount: OK");
  }

  bool fileExists = LittleFS.exists(JSON_DATA_FILE);
  Serial.print("File exists: ");
  Serial.println(fileExists);
  
  if (!fileExists) {
    Serial.println("File doesn't exist");
    Serial.println("Creating file...");
    
    // Create initial JSON structure
    StaticJsonDocument<JSON_DOC_SIZE> doc;
    JsonArray dataArray = doc.to<JsonArray>();
    
    // Create initial data sets with zeros
    for (int datasetIndex = 0; datasetIndex < MAX_DATA_SETS; datasetIndex++) {
      JsonObject dataset = dataArray.createNestedObject();
      
      JsonArray vp51Array = dataset.createNestedArray("vp51");
      for (int i = 0; i < 2; i++) {
        vp51Array.add(0);
      }
      
      JsonArray vp52Array = dataset.createNestedArray("vp52");
      for (int i = 0; i < 3; i++) {
        vp52Array.add(0);
      }
      
      JsonArray vp53Array = dataset.createNestedArray("vp53");
      for (int i = 0; i < 3; i++) {
        vp53Array.add(0);
      }
    }
    
    // Write initial file
    File file = LittleFS.open(JSON_DATA_FILE, "w");
    if (!file) {
      Serial.println("Failed to create file");
      return false;
    }
    
    if (serializeJson(doc, file) == 0) {
      Serial.println("Failed to write initial JSON");
      file.close();
      return false;
    }
    
    file.close();
    Serial.println("Initial JSON file created");
  } else {
    Serial.println("File already exists");
  }
  
  return true;
}

// Helper function to write file (if needed elsewhere)
/*
void writeFile(fs::FS &fs, const char * path, const char * message) {
  File file = fs.open(path, "w");
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  
  file.close();
}
*/

// Helper function to read file (if needed elsewhere)
/*
String readFile(fs::FS &fs, const char * path) {
  File file = fs.open(path, "r");
  if (!file || file.isDirectory()) {
    Serial.println("Failed to open file for reading");
    return String();
  }
  
  String fileContent;
  while (file.available()) {
    fileContent += String((char)file.read());
  }
  
  file.close();
  return fileContent;
}
*/

bool switchMainPage() {
  dataFrameTx[2] = 0x07;  // PAYLOAD_LENGTH
  dataFrameTx[3] = 0x82;  // INSTRUCTION
  dataFrameTx[4] = 0x00;  // VAR_ID:H (SYSTEM_VAR_ID:PIC_SET)
  dataFrameTx[5] = 0x84;  // VAR_ID:L (SYSTEM_VAR_ID:PIC_SET)
  dataFrameTx[6] = 0x5A;  // ENABLE_PAGE_OPERATION_ONCE
  dataFrameTx[7] = 0x01;  // PROCESSING_MODE (0X01 PAGE SWITCH)
  dataFrameTx[8] = 0x00;  // DATA:H

  if (vp51[0] == 0 && vp51[1] == 0) {dataFrameTx[9] = 0x00; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp51[0] == 1 && vp51[1] == 0) {dataFrameTx[9] = 0x01; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp51[0] == 2 && vp51[1] == 0) {dataFrameTx[9] = 0x02; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp51[0] == 0 && vp51[1] == 1) {dataFrameTx[9] = 0x03; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp51[0] == 1 && vp51[1] == 1) {dataFrameTx[9] = 0x04; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp51[0] == 2 && vp51[1] == 1) {dataFrameTx[9] = 0x05; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp51[0] == 0 && vp51[1] == 2) {dataFrameTx[9] = 0x06; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp51[0] == 1 && vp51[1] == 2) {dataFrameTx[9] = 0x07; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp51[0] == 2 && vp51[1] == 2) {dataFrameTx[9] = 0x08; Serial1.write(dataFrameTx, 10); return true;}
  else {dataFrameTx[9] = 0x00; Serial1.write(dataFrameTx, 10); return false;}
}

bool setMainPageVp() {
  dataFrameTx[2] = 0x05;                // PAYLOAD_LENGTH (BYTE)
  dataFrameTx[3] = 0x82;                // INSTRUCTION
  dataFrameTx[4] = 0x51;                // VAR_ID:H
  dataFrameTx[5] = 0x00;                // VAR_ID:L
  dataFrameTx[6] = highByte(vp51[0]);   // DATA:H
  dataFrameTx[7] = lowByte(vp51[0]);    // DATA:L
  Serial1.write(dataFrameTx, 8);
  
  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x51;
  dataFrameTx[5] = 0x01;
  dataFrameTx[6] = highByte(vp51[1]);
  dataFrameTx[7] = lowByte(vp51[1]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x52;
  dataFrameTx[5] = 0x00;
  dataFrameTx[6] = highByte(vp52[0]);
  dataFrameTx[7] = lowByte(vp52[0]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x52;
  dataFrameTx[5] = 0x01;
  dataFrameTx[6] = highByte(vp52[1]);
  dataFrameTx[7] = lowByte(vp52[1]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x52;
  dataFrameTx[5] = 0x02;
  dataFrameTx[6] = highByte(vp52[2]);
  dataFrameTx[7] = lowByte(vp52[2]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x53;
  dataFrameTx[5] = 0x00;
  dataFrameTx[6] = highByte(vp53[0]);
  dataFrameTx[7] = lowByte(vp53[0]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x53;
  dataFrameTx[5] = 0x01;
  dataFrameTx[6] = highByte(vp53[1]);
  dataFrameTx[7] = lowByte(vp53[1]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x53;
  dataFrameTx[5] = 0x02;
  dataFrameTx[6] = highByte(vp53[2]);
  dataFrameTx[7] = lowByte(vp53[2]);
  Serial1.write(dataFrameTx, 8);

  return true;
}

// Save all data sets to JSON
bool saveToJSON() {
  StaticJsonDocument<JSON_DOC_SIZE> doc;
  
  // Create array of objects
  JsonArray dataArray = doc.to<JsonArray>();
  
  for (int datasetIndex = 0; datasetIndex < MAX_DATA_SETS; datasetIndex++) {
    JsonObject dataset = dataArray.createNestedObject();
    
    // Create vp51 array for this data set
    JsonArray vp51Array = dataset.createNestedArray("vp51");
    vp51Array.add(allVP51[datasetIndex][0]);
    vp51Array.add(allVP51[datasetIndex][1]);
    
    // Create vp52 array for this data set
    JsonArray vp52Array = dataset.createNestedArray("vp52");
    vp52Array.add(allVP52[datasetIndex][0]);
    vp52Array.add(allVP52[datasetIndex][1]);
    vp52Array.add(allVP52[datasetIndex][2]);
    
    
    // Create vp53 array for this data set
    JsonArray vp53Array = dataset.createNestedArray("vp53");
    vp53Array.add(allVP53[datasetIndex][0]);
    vp53Array.add(allVP53[datasetIndex][1]);
    vp53Array.add(allVP53[datasetIndex][2]);
  }
  
  // Write to file
  File file = LittleFS.open(JSON_DATA_FILE, "w");
  if (!file) {
    Serial.println("Failed to open JSON file for writing");
    return false;
  }
  
  if (serializeJson(doc, file) == 0) {
    Serial.println("Failed to write JSON");
    file.close();
    return false;
  }
  
  file.close();
  Serial.println("JSON data saved");
  return true;
}

// Load all data sets from JSON
bool loadFromJSON() {
  if (!LittleFS.exists(JSON_DATA_FILE)) {
    Serial.println("JSON file does not exist");
    return false;
  }
  
  File file = LittleFS.open(JSON_DATA_FILE, "r");
  if (!file) {
    Serial.println("Failed to open JSON file for reading");
    return false;
  }
  
  StaticJsonDocument<JSON_DOC_SIZE> doc;
  DeserializationError error = deserializeJson(doc, file);
  file.close();
  
  if (error) {
    Serial.print("JSON parse failed: ");
    Serial.println(error.c_str());
    return false;
  }
  
  if (!doc.is<JsonArray>()) {
    Serial.println("JSON is not an array");
    return false;
  }
  
  JsonArray dataArray = doc.as<JsonArray>();
  int setCount = min(dataArray.size(), (size_t)MAX_DATA_SETS);
  
  // Load each data set
  for (int datasetIndex = 0; datasetIndex < setCount; datasetIndex++) {
    JsonObject dataset = dataArray[datasetIndex];
    
    // Load vp51
    if (dataset.containsKey("vp51") && dataset["vp51"].is<JsonArray>()) {
      allVP51[datasetIndex][0] = dataset["vp51"][0];
      allVP51[datasetIndex][1] = dataset["vp51"][1];
    }
    
    // Load vp52
    if (dataset.containsKey("vp52") && dataset["vp52"].is<JsonArray>()) {
      allVP52[datasetIndex][0] = dataset["vp52"][0];
      allVP52[datasetIndex][1] = dataset["vp52"][1];
      allVP52[datasetIndex][2] = dataset["vp52"][2];
    }
    
    // Load vp53
    if (dataset.containsKey("vp53") && dataset["vp53"].is<JsonArray>()) {
      allVP53[datasetIndex][0] = dataset["vp53"][0];
      allVP53[datasetIndex][1] = dataset["vp53"][1];
      allVP53[datasetIndex][2] = dataset["vp53"][2];
    }
  }
  
  // Update current working arrays
  vp51[0] = allVP51[0][0];
  vp51[1] = allVP51[0][1];
  vp52[0] = allVP52[0][0];
  vp52[1] = allVP52[0][1];
  vp52[2] = allVP52[0][2];
  vp53[0] = allVP53[0][0];
  vp53[1] = allVP53[0][1];
  vp53[2] = allVP53[0][2];
  
  Serial.println("JSON data loaded");
  return true;
}

// Save by pointer: pointer = datasetIndex, saves current vp51, vp52, vp53 to that dataset index
bool saveByPointer(uint16_t datasetIndex) {
  
  // Validate data set index
  if (datasetIndex >= MAX_DATA_SETS) {
    Serial.println("Invalid data set index");
    return false;
  }
  
  // Copy current vp51, vp52, vp53 to the specified dataset index
  allVP51[datasetIndex][0] = vp51[0];
  allVP51[datasetIndex][1] = vp51[1];
  allVP52[datasetIndex][0] = vp52[0];
  allVP52[datasetIndex][1] = vp52[1];
  allVP52[datasetIndex][2] = vp52[2];
  allVP53[datasetIndex][0] = vp53[0];
  allVP53[datasetIndex][1] = vp53[1];
  allVP53[datasetIndex][2] = vp53[2];
  
  // Save to JSON file
  return saveToJSON();
}

// Load by pointer: pointer = datasetIndex, loads vp51, vp52, vp53 from that dataset index
bool loadByPointer(uint16_t datasetIndex) {
  
  // Validate data set index
  if (datasetIndex >= MAX_DATA_SETS) {
    Serial.println("Invalid data set index");
    return false;
  }
  
  vp51[0] = allVP51[datasetIndex][0];
  vp51[1] = allVP51[datasetIndex][1];
  vp52[0] = allVP52[datasetIndex][0];
  vp52[1] = allVP52[datasetIndex][1];
  vp52[2] = allVP52[datasetIndex][2];
  vp53[0] = allVP53[datasetIndex][0];
  vp53[1] = allVP53[datasetIndex][1];
  vp53[2] = allVP53[datasetIndex][2];
  
  
  Serial.print("Loaded data set ");
  Serial.println(datasetIndex);
  return true;
}

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
  duration0 = dutyCycles[mode];

  // Wait for setRelay to complete and return true
  if (setRelay(mode)) {
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
  if (Serial1.available()) {  // Ensure we have complete frame
    for (int i = 0; i < 9; i++) {
      rxBuffer[i] = Serial1.read();
    }

    for (int i = 0; i < 9; i++) {
      if (rxBuffer[i] < 0x10) Serial.print('0');
      Serial.print(rxBuffer[i], HEX);
      Serial.print(' ');
    }

    Serial.println();

    // rxBuffer[0]: HEADER
    // rxBuffer[1]: HEADER
    // rxBuffer[2]: PAYLOAD_LENGTH
    // rxBuffer[3]: INSTRUCTION
    // rxBuffer[4]: VAR_ID_H
    // rxBuffer[5]: VAR_ID_L
    // rxBuffer[6]: DATA_LENGTH
    // rxBuffer[7]: DATA_H
    // rxBuffer[8]: DATA_L
    
    if (rxBuffer[0] == 0x5A && rxBuffer[1] == 0xA5) {  // Check header
      if (rxBuffer[3] == 0x83) {
        uint16_t value = (rxBuffer[7] << 8) | rxBuffer[8];
        switch (rxBuffer[4]) {
          case 0x50:
            break;
          case 0x51:
            if (rxBuffer[5] == 0x00 || rxBuffer[5] == 0x01) {
              vp51[rxBuffer[5]] = value;
            }
            break;
          case 0x52:
            if (rxBuffer[5] == 0x00 || rxBuffer[5] == 0x01 || rxBuffer[5] == 0x02) {
              vp52[rxBuffer[5]] = value;
            }
            break;
          case 0x53:
            if (rxBuffer[5] == 0x00 || rxBuffer[5] == 0x01 || rxBuffer[5] == 0x02) {
              vp53[rxBuffer[5]] = value;
            }
            break;
          case 0x54:
            switch (rxBuffer[5]) {
              case 0x00:  // Load all data sets from JSON
                if (loadFromJSON()) {
                  Serial.println("All data sets loaded from JSON");
                } else {
                  Serial.println("Failed to load from JSON");
                }
                break;
              case 0x01:  // Save all data sets to JSON
                if (saveToJSON()) {
                  Serial.println("All data sets saved to JSON");
                } else {
                  Serial.println("Failed to save to JSON");
                }
                break;
              case 0x02:  // Load by pointer (dataset index)
                if (loadByPointer(value)) {
                  Serial.print("Loaded data set ");
                  Serial.println(value);
                } else {
                  Serial.println("Failed to load by pointer");
                }
                switchMainPage();
                setMainPageVp();
                break;
              case 0x03:  // Save by pointer (dataset index)
                if (saveByPointer(value)) {
                  Serial.print("Saved current data to set ");
                  Serial.println(value);
                } else {
                  Serial.println("Failed to save by pointer");
                }
                switchMainPage();
                break;
            }
          break;
        }
      }
    }
  }

  // if (Serial.available() > 0) {
  //   int data0 = 0;
  //   int data1 = 0;
  //   int data2 = 0;

  //   String serialData = Serial.readStringUntil('\n');
    
  //   int firstSpaceIndex = serialData.indexOf(' ');
  //   int secondSpaceIndex = serialData.indexOf(' ', firstSpaceIndex + 1);

  //   if (firstSpaceIndex > 0 && secondSpaceIndex > firstSpaceIndex) {
  //     // Three data values
  //     int data0 = serialData.substring(0, firstSpaceIndex).toInt();
  //     int data1 = serialData.substring(firstSpaceIndex + 1, secondSpaceIndex).toInt();
  //     int data2 = serialData.substring(secondSpaceIndex + 1).toInt();

  //     if (data0 == 1) {
  //       if (data1 == 0 || data1 == 1 || data1 == 2 ||data1 == 5 ) {
  //         if (data2 >= 0 && data2 <= 200) {
  //           rmtStop();
  //           dutyCycles[data1] = data2;
  //           Serial.println("00");
  //         } else {
  //           Serial.println("02");
  //         }
  //       }
  //       else if (data1 == 3) {
  //         if (data2 >= 0 && data2 <= 400) {
  //           rmtStop();
  //           dutyCycles[data1] = data2;
  //           Serial.println("00");
  //         } else {
  //           Serial.println("02");
  //         }
  //       }
  //       else if (data1 == 4) {
  //         if (data2 >= 0 && data2 <= 500) {
  //           rmtStop();
  //           dutyCycles[data1] = data2;
  //           Serial.println("00");
  //         } else {
  //           Serial.println("02");
  //         }
  //       }
  //       else {
  //         Serial.println("02");
  //       }
  //     }
  //   } else if (firstSpaceIndex > 0) {
  //     // Two data values (fallback to current logic)
  //     int data0 = serialData.substring(0, firstSpaceIndex).toInt();
  //     int data1 = serialData.substring(firstSpaceIndex + 1).toInt();
      
  //     if (data0 == 0) {
  //       if (data1 == 0 || data1 == 1 || data1 == 2) {
  //         rmtStop();
  //         currentMode[0] = data1;
  //         Serial.println("00");
  //       } else if (data1 == 3 || data1 == 4 || data1 == 5) {
  //         rmtStop();
  //         currentMode[1] = data1;
  //         Serial.println("00");
  //       } else {
  //         Serial.println("02");
  //       }
  //     } else {
  //       Serial.println("02");
  //     }
  //   } else {
  //     int data0 = serialData.toInt();
  //     if (data0 == 2) {
  //       rmtStop();
  //       Serial.println("00");
  //     } else {
  //       Serial.println("02");
  //     }
  //   }
  // }
}

void setup() {
  setupRelay();
  setupADC();

  Serial.begin(115200);
  Serial1.begin(115200, SERIAL_8N1, 18, 17);

  // Initialize LittleFS with proper error handling
  if (!initLittleFS()) {
    Serial.println("LittleFS initialization failed!");
    return;  // Stop execution if LittleFS fails
  }

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
  rmt_tx_1.clk_div = 4; // 80 MHz ÷ 4 = 20 MHz
  // rmt_tx_1.tx_config.idle_level = RMT_IDLE_LEVEL_LOW;
  rmt_tx_1.tx_config.idle_output_en = 1;
  rmt_tx_1.tx_config.loop_en = 1;
  rmt_config(&rmt_tx_1);
  rmt_driver_install(RMT_TX_CHANNEL_1, 0, 0);

  rmt_set_tx_intr_en(RMT_TX_CHANNEL_0, 1);
  rmt_set_tx_intr_en(RMT_TX_CHANNEL_1, 1);

  setupButtons();
  
  // Load saved data from JSON on startup
  loadFromJSON();

  switchMainPage();
  setMainPageVp();
}

void loop() {
  readSerialData();  // Keep serial input functionality
  if(readREM() < REM_UPPER_LIMIT) {
    readButtons();     // Add button input functionality
  }
}