#include "data_manager.h"

// Initialize data arrays
uint16_t allVP51[MAX_DATA_SETS][2] = {0};
uint16_t allVP52[MAX_DATA_SETS][3] = {0};

// Current working arrays (point to current data set)
uint16_t vp51[2] = {0, 0};
uint16_t vp52[6] = {0, 0, 0, 0, 0, 0};

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
    vp52Array.add(allVP52[datasetIndex][3]);
    vp52Array.add(allVP52[datasetIndex][4]);
    vp52Array.add(allVP52[datasetIndex][5]);
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
      allVP52[datasetIndex][3] = dataset["vp52"][3];
      allVP52[datasetIndex][4] = dataset["vp52"][4];
      allVP52[datasetIndex][5] = dataset["vp52"][5];
    }
  }
  
  // Update current working arrays
  vp51[0] = allVP51[0][0];
  vp51[1] = allVP51[0][1];
  vp52[0] = allVP52[0][0];
  vp52[1] = allVP52[0][1];
  vp52[2] = allVP52[0][2];
  vp52[3] = allVP52[0][3];
  vp52[4] = allVP52[0][4];
  vp52[5] = allVP52[0][5];
  
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
  allVP52[datasetIndex][3] = vp52[3];
  allVP52[datasetIndex][4] = vp52[4];
  allVP52[datasetIndex][5] = vp52[5];
  
  
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
  vp52[3] = allVP52[datasetIndex][3];
  vp52[4] = allVP52[datasetIndex][4];
  vp52[5] = allVP52[datasetIndex][5];
  
  
  Serial.print("Loaded data set ");
  Serial.println(datasetIndex);
  return true;
}