#include "data_manager.h"

// Initialize data arrays
uint16_t allVP81[MAX_DATA_SETS][2] = {0};
uint16_t allVP82[MAX_DATA_SETS][3] = {0};
uint16_t allVP83[MAX_DATA_SETS][3] = {0};

// Current working arrays (point to current data set)
uint16_t vp81[2] = {0, 0};
uint16_t vp82[3] = {0, 0, 0};
uint16_t vp83[3] = {0, 0, 0};

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
      
      JsonArray vp51Array = dataset.createNestedArray("vp81");
      for (int i = 0; i < 2; i++) {
        vp51Array.add(0);
      }
      
      JsonArray vp52Array = dataset.createNestedArray("vp82");
      for (int i = 0; i < 3; i++) {
        vp52Array.add(0);
      }
      
      JsonArray vp53Array = dataset.createNestedArray("vp83");
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
    
    // Create vp81 array for this data set
    JsonArray vp51Array = dataset.createNestedArray("vp81");
    vp51Array.add(allVP81[datasetIndex][0]);
    vp51Array.add(allVP81[datasetIndex][1]);
    
    // Create vp82 array for this data set
    JsonArray vp52Array = dataset.createNestedArray("vp82");
    vp52Array.add(allVP82[datasetIndex][0]);
    vp52Array.add(allVP82[datasetIndex][1]);
    vp52Array.add(allVP82[datasetIndex][2]);
    
    
    // Create vp83 array for this data set
    JsonArray vp53Array = dataset.createNestedArray("vp83");
    vp53Array.add(allVP83[datasetIndex][0]);
    vp53Array.add(allVP83[datasetIndex][1]);
    vp53Array.add(allVP83[datasetIndex][2]);
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
    
    // Load vp81
    if (dataset.containsKey("vp81") && dataset["vp81"].is<JsonArray>()) {
      allVP81[datasetIndex][0] = dataset["vp81"][0];
      allVP81[datasetIndex][1] = dataset["vp81"][1];
    }
    
    // Load vp82
    if (dataset.containsKey("vp82") && dataset["vp82"].is<JsonArray>()) {
      allVP82[datasetIndex][0] = dataset["vp82"][0];
      allVP82[datasetIndex][1] = dataset["vp82"][1];
      allVP82[datasetIndex][2] = dataset["vp82"][2];
    }
    
    // Load vp83
    if (dataset.containsKey("vp83") && dataset["vp83"].is<JsonArray>()) {
      allVP83[datasetIndex][0] = dataset["vp83"][0];
      allVP83[datasetIndex][1] = dataset["vp83"][1];
      allVP83[datasetIndex][2] = dataset["vp83"][2];
    }
  }
  
  // Update current working arrays
  vp81[0] = allVP81[0][0];
  vp81[1] = allVP81[0][1];
  vp82[0] = allVP82[0][0];
  vp82[1] = allVP82[0][1];
  vp82[2] = allVP82[0][2];
  vp83[0] = allVP83[0][0];
  vp83[1] = allVP83[0][1];
  vp83[2] = allVP83[0][2];
  
  Serial.println("JSON data loaded");
  return true;
}

// Save by pointer: pointer = datasetIndex, saves current vp81, vp82, vp83 to that dataset index
bool saveByPointer(uint16_t datasetIndex) {
  
  // Validate data set index
  if (datasetIndex >= MAX_DATA_SETS) {
    Serial.println("Invalid data set index");
    return false;
  }
  
  // Copy current vp81, vp82, vp83 to the specified dataset index
  allVP81[datasetIndex][0] = vp81[0];
  allVP81[datasetIndex][1] = vp81[1];
  allVP82[datasetIndex][0] = vp82[0];
  allVP82[datasetIndex][1] = vp82[1];
  allVP82[datasetIndex][2] = vp82[2];
  allVP83[datasetIndex][0] = vp83[0];
  allVP83[datasetIndex][1] = vp83[1];
  allVP83[datasetIndex][2] = vp83[2];
  
  // Save to JSON file
  return saveToJSON();
}

// Load by pointer: pointer = datasetIndex, loads vp81, vp82, vp83 from that dataset index
bool loadByPointer(uint16_t datasetIndex) {
  
  // Validate data set index
  if (datasetIndex >= MAX_DATA_SETS) {
    Serial.println("Invalid data set index");
    return false;
  }
  
  vp81[0] = allVP81[datasetIndex][0];
  vp81[1] = allVP81[datasetIndex][1];
  vp82[0] = allVP82[datasetIndex][0];
  vp82[1] = allVP82[datasetIndex][1];
  vp82[2] = allVP82[datasetIndex][2];
  vp83[0] = allVP83[datasetIndex][0];
  vp83[1] = allVP83[datasetIndex][1];
  vp83[2] = allVP83[datasetIndex][2];
  
  
  Serial.print("Loaded data set ");
  Serial.println(datasetIndex);
  return true;
}