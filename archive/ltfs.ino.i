#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

#define FORMAT_LITTLEFS_IF_FAILED true

int mydata;

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    file.close();
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}


void setup() {
  Serial.begin(115200);
  if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED)){
      Serial.println("LittleFS Mount: NG");
      return;
   }
   else{
      Serial.println("LittleFS Mount: OK");
   }

   bool fileexists = LittleFS.exists("/profile.json");
   Serial.print(fileexists);
   if(!fileexists) {
       Serial.println("File doesn’t exist");
       Serial.println("Creating file...");
       
       writeFile(LittleFS, "/profile.json", "MY ESP32 DATA \r\n");
   }
   else {
       Serial.println("File already exists");
   }
}

void loop() {
//   mydata = random (0, 1000);
//   appendFile(LittleFS, "/data.txt", (String(mydata)+ "\r\n").c_str());
//   readFile(LittleFS, "/data.txt");
//   delay(30000);
}