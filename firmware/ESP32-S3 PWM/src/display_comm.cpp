#include "config.h"
#include "display_comm.h"
#include "data_manager.h"

// Data frame for display communication
unsigned char dataFrameTx[16] = {0x5A, 0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 5A A5 PAYLOAD_LENGTH INSTRUCTION VAR_ID_H VAR_ID_L DATA_LENGTH DATA_H DATA_L
uint8_t rxBuffer[9];

uint16_t sp20_30_40_x = SP20_30_40_DEFAULT_X;
uint16_t sp50_60_70_x = SP50_60_70_DEFAULT_X;

bool switchMainPage() {
  dataFrameTx[2] = 0x07;  // PAYLOAD_LENGTH
  dataFrameTx[3] = 0x82;  // INSTRUCTION
  dataFrameTx[4] = 0x00;  // VAR_ID:H (SYSTEM_VAR_ID:PIC_SET)
  dataFrameTx[5] = 0x84;  // VAR_ID:L (SYSTEM_VAR_ID:PIC_SET)
  dataFrameTx[6] = 0x5A;  // ENABLE_PAGE_OPERATION_ONCE
  dataFrameTx[7] = 0x01;  // PROCESSING_MODE (0X01 PAGE SWITCH)
  dataFrameTx[8] = 0x00;  // DATA:H

  if (vp81[0] == 0 && vp81[1] == 0) {dataFrameTx[9] = 0x00; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp81[0] == 1 && vp81[1] == 0) {dataFrameTx[9] = 0x01; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp81[0] == 2 && vp81[1] == 0) {dataFrameTx[9] = 0x02; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp81[0] == 0 && vp81[1] == 1) {dataFrameTx[9] = 0x03; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp81[0] == 1 && vp81[1] == 1) {dataFrameTx[9] = 0x04; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp81[0] == 2 && vp81[1] == 1) {dataFrameTx[9] = 0x05; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp81[0] == 0 && vp81[1] == 2) {dataFrameTx[9] = 0x06; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp81[0] == 1 && vp81[1] == 2) {dataFrameTx[9] = 0x07; Serial1.write(dataFrameTx, 10); return true;}
  else if (vp81[0] == 2 && vp81[1] == 2) {dataFrameTx[9] = 0x08; Serial1.write(dataFrameTx, 10); return true;}
  else {dataFrameTx[9] = 0x00; Serial1.write(dataFrameTx, 10); return false;}
}

bool setMainPageVp() {
  dataFrameTx[2] = 0x05;                // PAYLOAD_LENGTH (BYTE)
  dataFrameTx[3] = 0x82;                // INSTRUCTION
  dataFrameTx[4] = 0x81;                // VAR_ID:H
  dataFrameTx[5] = 0x00;                // VAR_ID:L
  dataFrameTx[6] = highByte(vp81[0]);   // DATA:H
  dataFrameTx[7] = lowByte(vp81[0]);    // DATA:L
  Serial1.write(dataFrameTx, 8);
  
  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x81;
  dataFrameTx[5] = 0x01;
  dataFrameTx[6] = highByte(vp81[1]);
  dataFrameTx[7] = lowByte(vp81[1]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x82;
  dataFrameTx[5] = 0x00;
  dataFrameTx[6] = highByte(vp82[0]);
  dataFrameTx[7] = lowByte(vp82[0]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x82;
  dataFrameTx[5] = 0x01;
  dataFrameTx[6] = highByte(vp82[1]);
  dataFrameTx[7] = lowByte(vp82[1]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x82;
  dataFrameTx[5] = 0x02;
  dataFrameTx[6] = highByte(vp82[2]);
  dataFrameTx[7] = lowByte(vp82[2]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x83;
  dataFrameTx[5] = 0x00;
  dataFrameTx[6] = highByte(vp83[0]);
  dataFrameTx[7] = lowByte(vp83[0]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x83;
  dataFrameTx[5] = 0x01;
  dataFrameTx[6] = highByte(vp83[1]);
  dataFrameTx[7] = lowByte(vp83[1]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x83;
  dataFrameTx[5] = 0x02;
  dataFrameTx[6] = highByte(vp83[2]);
  dataFrameTx[7] = lowByte(vp83[2]);
  Serial1.write(dataFrameTx, 8);

  return true;
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
          case 0x80:
            break;
          case 0x81:
            if (rxBuffer[5] == 0x00 || rxBuffer[5] == 0x01) {
              vp81[rxBuffer[5]] = value;
            }
            break;
          case 0x82:
            if (rxBuffer[5] == 0x00 || rxBuffer[5] == 0x01 || rxBuffer[5] == 0x02) {
              vp82[rxBuffer[5]] = value;
              if (value < 10) {
                if (sp20_30_40_x != SP20_30_40_DEFAULT_X) {
                  sp20_30_40_x = SP20_30_40_DEFAULT_X;

                  dataFrameTx[2] = 0x07;                      // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                      // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] << 4) | 0x20; // SP:H
                  dataFrameTx[5] = 0x01;                      // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp20_30_40_x);          // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp20_30_40_x);           // DATA:L (x)
                  dataFrameTx[8] = highByte(SP20_30_40_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP20_30_40_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
              else if (value < 20) {
                if (sp20_30_40_x != (SP20_30_40_DEFAULT_X + 20)) {         // LEFT -65 RIGHT +20
                  sp20_30_40_x = SP20_30_40_DEFAULT_X + 20;
                  
                  dataFrameTx[2] = 0x07;                      // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                      // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] << 4) | 0x20; // SP:H
                  dataFrameTx[5] = 0x01;                      // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp20_30_40_x);           // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp20_30_40_x);            // DATA:L (x)
                  dataFrameTx[8] = highByte(SP20_30_40_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP20_30_40_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
              else if (value < 100) {
                if (sp20_30_40_x != SP20_30_40_DEFAULT_X + 40) {         // LEFT -35 RIGHT +40
                  sp20_30_40_x = SP20_30_40_DEFAULT_X + 40;
                  
                  dataFrameTx[2] = 0x07;                      // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                      // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] << 4) | 0x20; // SP:H
                  dataFrameTx[5] = 0x01;                      // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp20_30_40_x);           // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp20_30_40_x);            // DATA:L (x)
                  dataFrameTx[8] = highByte(SP20_30_40_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP20_30_40_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
              else if (value < 200){
                if (sp20_30_40_x != SP20_30_40_DEFAULT_X + 60) {         // LEFT -85 RIGHT +60
                  sp20_30_40_x = SP20_30_40_DEFAULT_X + 60;
                  
                  dataFrameTx[2] = 0x07;                      // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                      // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] << 4) | 0x20; // SP:H
                  dataFrameTx[5] = 0x01;                      // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp20_30_40_x);           // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp20_30_40_x);            // DATA:L (x)
                  dataFrameTx[8] = highByte(SP20_30_40_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP20_30_40_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              } else {  // default
                if (sp20_30_40_x != SP20_30_40_DEFAULT_X) {
                  sp20_30_40_x = SP20_30_40_DEFAULT_X;

                  dataFrameTx[2] = 0x07;                      // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                      // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] << 4) | 0x50; // SP:H
                  dataFrameTx[5] = 0x01;                      // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp20_30_40_x);           // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp20_30_40_x);            // DATA:L (x)
                  dataFrameTx[8] = highByte(SP20_30_40_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP20_30_40_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
            }
            break;
          case 0x83:
            if (rxBuffer[5] == 0x00 || rxBuffer[5] == 0x01 || rxBuffer[5] == 0x02) {
              vp83[rxBuffer[5]] = value;
              if (value < 10) {
                if (sp50_60_70_x != SP50_60_70_DEFAULT_X) {
                  sp50_60_70_x = SP50_60_70_DEFAULT_X;

                  dataFrameTx[2] = 0x07;                      // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                      // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] << 4) | 0x50; // SP:H
                  dataFrameTx[5] = 0x01;                      // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp50_60_70_x);           // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp50_60_70_x);            // DATA:L (x)
                  dataFrameTx[8] = highByte(SP50_60_70_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP50_60_70_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
              else if (value < 20) {
                if (sp50_60_70_x != (SP50_60_70_DEFAULT_X - 65)) {
                  sp50_60_70_x = SP50_60_70_DEFAULT_X - 65;
                  
                  dataFrameTx[2] = 0x07;                      // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                      // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] << 4) | 0x50; // SP:H
                  dataFrameTx[5] = 0x01;                      // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp50_60_70_x);           // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp50_60_70_x);            // DATA:L (x)
                  dataFrameTx[8] = highByte(SP50_60_70_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP50_60_70_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
              else if (value < 100) {
                if (sp50_60_70_x != SP50_60_70_DEFAULT_X - 35) {
                  sp50_60_70_x = SP50_60_70_DEFAULT_X - 35;
                  
                  dataFrameTx[2] = 0x07;                      // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                      // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] << 4) | 0x50; // SP:H
                  dataFrameTx[5] = 0x01;                      // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp50_60_70_x);           // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp50_60_70_x);            // DATA:L (x)
                  dataFrameTx[8] = highByte(SP50_60_70_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP50_60_70_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
              else if (value < 200){
                if (sp50_60_70_x != SP50_60_70_DEFAULT_X - 85) {
                  sp50_60_70_x = SP50_60_70_DEFAULT_X - 85;
                  
                  dataFrameTx[2] = 0x07;                      // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                      // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] << 4) | 0x50; // SP:H
                  dataFrameTx[5] = 0x01;                      // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp50_60_70_x);           // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp50_60_70_x);            // DATA:L (x)
                  dataFrameTx[8] = highByte(SP50_60_70_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP50_60_70_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              } else {  // default
                if (sp50_60_70_x != SP50_60_70_DEFAULT_X) {
                  sp50_60_70_x = SP50_60_70_DEFAULT_X;

                  dataFrameTx[2] = 0x07;                      // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                      // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] << 4) | 0x20; // SP:H
                  dataFrameTx[5] = 0x01;                      // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp50_60_70_x);           // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp50_60_70_x);            // DATA:L (x)
                  dataFrameTx[8] = highByte(SP50_60_70_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP50_60_70_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
            }
            break;
          case 0x84:
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
}