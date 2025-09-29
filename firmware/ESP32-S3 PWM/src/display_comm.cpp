#include "config.h"
#include "display_comm.h"
#include "data_manager.h"

// Data frame for display communication
unsigned char dataFrameTx[16] = {0x5A, 0xA5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

// 5A A5 PAYLOAD_LENGTH INSTRUCTION VAR_ID_H VAR_ID_L DATA_LENGTH DATA_H DATA_L
uint8_t rxBuffer[32];

uint16_t sp80_90_A0_x = SP80_90_A0_DEFAULT_X;
uint16_t spB0_C0_D0_x = SPB0_C0_D0_DEFAULT_X;

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
  else {return false;}
}

bool fireSignalState(bool mode) {
  dataFrameTx[2] = 0x07;  // PAYLOAD_LENGTH
  dataFrameTx[3] = 0x82;  // INSTRUCTION
  dataFrameTx[4] = 0x00;  // VAR_ID:H (SYSTEM_VAR_ID:PIC_SET)
  dataFrameTx[5] = 0x84;  // VAR_ID:L (SYSTEM_VAR_ID:PIC_SET)
  dataFrameTx[6] = 0x5A;  // ENABLE_PAGE_OPERATION_ONCE
  dataFrameTx[7] = 0x01;  // PROCESSING_MODE (0X01 PAGE SWITCH)
  dataFrameTx[8] = 0x00;  // DATA:H

       if (vp51[0] == 0 && vp51[1] == 0) { dataFrameTx[9] = mode == 0 ? 0x0D : 0x0E; Serial1.write(dataFrameTx, 10); return true; }
  else if (vp51[0] == 1 && vp51[1] == 0) { dataFrameTx[9] = mode == 0 ? 0x1F : 0x10; Serial1.write(dataFrameTx, 10); return true; }
  else if (vp51[0] == 2 && vp51[1] == 0) { dataFrameTx[9] = mode == 0 ? 0x11 : 0x12; Serial1.write(dataFrameTx, 10); return true; }
  else if (vp51[0] == 0 && vp51[1] == 1) { dataFrameTx[9] = mode == 0 ? 0x13 : 0x14; Serial1.write(dataFrameTx, 10); return true; }
  else if (vp51[0] == 1 && vp51[1] == 1) { dataFrameTx[9] = mode == 0 ? 0x15 : 0x16; Serial1.write(dataFrameTx, 10); return true; }
  else if (vp51[0] == 2 && vp51[1] == 1) { dataFrameTx[9] = mode == 0 ? 0x17 : 0x18; Serial1.write(dataFrameTx, 10); return true; }
  else if (vp51[0] == 0 && vp51[1] == 2) { dataFrameTx[9] = mode == 0 ? 0x19 : 0x1A; Serial1.write(dataFrameTx, 10); return true; }
  else if (vp51[0] == 1 && vp51[1] == 2) { dataFrameTx[9] = mode == 0 ? 0x1B : 0x1C; Serial1.write(dataFrameTx, 10); return true; }
  else if (vp51[0] == 2 && vp51[1] == 2) { dataFrameTx[9] = mode == 0 ? 0x1D : 0x1E; Serial1.write(dataFrameTx, 10); return true; }
  else { return false; }
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
  dataFrameTx[6] = highByte(vp52[3]);
  dataFrameTx[7] = lowByte(vp52[3]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x53;
  dataFrameTx[5] = 0x01;
  dataFrameTx[6] = highByte(vp52[4]);
  dataFrameTx[7] = lowByte(vp52[4]);
  Serial1.write(dataFrameTx, 8);

  dataFrameTx[2] = 0x05;
  dataFrameTx[3] = 0x82;
  dataFrameTx[4] = 0x53;
  dataFrameTx[5] = 0x02;
  dataFrameTx[6] = highByte(vp52[5]);
  dataFrameTx[7] = lowByte(vp52[5]);
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
              if (value < 10) {
                if (sp80_90_A0_x != SP80_90_A0_DEFAULT_X) {
                  sp80_90_A0_x = SP80_90_A0_DEFAULT_X;

                  dataFrameTx[2] = 0x07;                        // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                        // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] & 0x03) | 0x80; // SP:H
                  dataFrameTx[5] = 0x01;                        // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp80_90_A0_x);          // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp80_90_A0_x);           // DATA:L (x)
                  dataFrameTx[8] = highByte(SP80_90_A0_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP80_90_A0_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
              else if (value < 20) {
                if (sp80_90_A0_x != (SP80_90_A0_DEFAULT_X + 20)) {         // LEFT -65 RIGHT +20
                  sp80_90_A0_x = SP80_90_A0_DEFAULT_X + 20;
                  
                  dataFrameTx[2] = 0x07;                        // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                        // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] & 0x03) | 0x80; // SP:H
                  dataFrameTx[5] = 0x01;                        // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp80_90_A0_x);          // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp80_90_A0_x);           // DATA:L (x)
                  dataFrameTx[8] = highByte(SP80_90_A0_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP80_90_A0_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
              else if (value < 100) {
                if (sp80_90_A0_x != SP80_90_A0_DEFAULT_X + 40) {         // LEFT -35 RIGHT +40
                  sp80_90_A0_x = SP80_90_A0_DEFAULT_X + 40;
                  
                  dataFrameTx[2] = 0x07;                        // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                        // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] & 0x03) | 0x80; // SP:H
                  dataFrameTx[5] = 0x01;                        // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp80_90_A0_x);          // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp80_90_A0_x);           // DATA:L (x)
                  dataFrameTx[8] = highByte(SP80_90_A0_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP80_90_A0_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
              else if (value < 200){
                if (sp80_90_A0_x != SP80_90_A0_DEFAULT_X + 60) {         // LEFT -85 RIGHT +60
                  sp80_90_A0_x = SP80_90_A0_DEFAULT_X + 60;
                  
                  dataFrameTx[2] = 0x07;                        // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                        // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] & 0x03) | 0x80; // SP:H
                  dataFrameTx[5] = 0x01;                        // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp80_90_A0_x);          // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp80_90_A0_x);           // DATA:L (x)
                  dataFrameTx[8] = highByte(SP80_90_A0_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP80_90_A0_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              } else {  // default
                if (sp80_90_A0_x != SP80_90_A0_DEFAULT_X) {
                  sp80_90_A0_x = SP80_90_A0_DEFAULT_X;

                  dataFrameTx[2] = 0x07;                        // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                        // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] & 0x03) | 0x80; // SP:H
                  dataFrameTx[5] = 0x01;                        // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(sp80_90_A0_x);          // DATA:H (x)
                  dataFrameTx[7] = lowByte(sp80_90_A0_x);           // DATA:L (x)
                  dataFrameTx[8] = highByte(SP80_90_A0_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SP80_90_A0_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
            } else if (rxBuffer[5] == 0x03 || rxBuffer[5] == 0x04 || rxBuffer[5] == 0x05) {
              vp52[rxBuffer[5]] = value;
              if (value < 10) {
                if (spB0_C0_D0_x != SPB0_C0_D0_DEFAULT_X) {
                  spB0_C0_D0_x = SPB0_C0_D0_DEFAULT_X;

                  dataFrameTx[2] = 0x07;                        // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                        // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] & 0x03) + 0x83; // SP:H
                  dataFrameTx[5] = 0x01;                        // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(spB0_C0_D0_x);          // DATA:H (x)
                  dataFrameTx[7] = lowByte(spB0_C0_D0_x);           // DATA:L (x)
                  dataFrameTx[8] = highByte(SPB0_C0_D0_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SPB0_C0_D0_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
              else if (value < 20) {
                if (spB0_C0_D0_x != (SPB0_C0_D0_DEFAULT_X + 20)) {
                  spB0_C0_D0_x = SPB0_C0_D0_DEFAULT_X + 20;
                  
                  dataFrameTx[2] = 0x07;                        // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                        // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] & 0x03) + 0x83; // SP:H
                  dataFrameTx[5] = 0x01;                        // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(spB0_C0_D0_x);          // DATA:H (x)
                  dataFrameTx[7] = lowByte(spB0_C0_D0_x);           // DATA:L (x)
                  dataFrameTx[8] = highByte(SPB0_C0_D0_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SPB0_C0_D0_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
              else if (value < 100) {
                if (spB0_C0_D0_x != SPB0_C0_D0_DEFAULT_X + 40) {
                  spB0_C0_D0_x = SPB0_C0_D0_DEFAULT_X + 40;
                  
                  dataFrameTx[2] = 0x07;                        // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                        // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] & 0x03) + 0x83; // SP:H
                  dataFrameTx[5] = 0x01;                        // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(spB0_C0_D0_x);          // DATA:H (x)
                  dataFrameTx[7] = lowByte(spB0_C0_D0_x);           // DATA:L (x)
                  dataFrameTx[8] = highByte(SPB0_C0_D0_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SPB0_C0_D0_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
              else if (value < 200){
                if (spB0_C0_D0_x != SPB0_C0_D0_DEFAULT_X + 60) {
                  spB0_C0_D0_x = SPB0_C0_D0_DEFAULT_X + 60;
                  
                  dataFrameTx[2] = 0x07;                        // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                        // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] & 0x03) + 0x83; // SP:H
                  dataFrameTx[5] = 0x01;                        // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(spB0_C0_D0_x);          // DATA:H (x)
                  dataFrameTx[7] = lowByte(spB0_C0_D0_x);           // DATA:L (x)
                  dataFrameTx[8] = highByte(SPB0_C0_D0_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SPB0_C0_D0_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              } else {  // default
                if (spB0_C0_D0_x != SPB0_C0_D0_DEFAULT_X) {
                  spB0_C0_D0_x = SPB0_C0_D0_DEFAULT_X;

                  dataFrameTx[2] = 0x07;                        // PAYLOAD_LENGTH (BYTE)
                  dataFrameTx[3] = 0x82;                        // INSTRUCTION
                  dataFrameTx[4] = (rxBuffer[5] & 0x03) + 0x83; // SP:H
                  dataFrameTx[5] = 0x01;                        // SP:L (OFFSET 0x01)
                  dataFrameTx[6] = highByte(spB0_C0_D0_x);          // DATA:H (x)
                  dataFrameTx[7] = lowByte(spB0_C0_D0_x);           // DATA:L (x)
                  dataFrameTx[8] = highByte(SPB0_C0_D0_DEFAULT_Y);  // DATA:H (y)
                  dataFrameTx[9] = lowByte(SPB0_C0_D0_DEFAULT_Y);   // DATA:L (y)
                  Serial1.write(dataFrameTx, 10);
                }
              }
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
                if (value != KEY_CANCEL ? loadByPointer(value) : false) {
                  Serial.print("Loaded data set ");
                  Serial.println(value);

                  switchMainPage();
                  setMainPageVp();
                } else {
                  Serial.println(value == KEY_CANCEL ? "Loading was cancelled by user" : "Failed to load by pointer");
                }
                break;
              case 0x03:  // Save by pointer (dataset index)
                if (value != KEY_CANCEL ? saveByPointer(value) : false) {
                  Serial.print("Saved current data to set ");
                  Serial.println(value);
                  
                  switchMainPage();
                } else {
                  Serial.println(value == KEY_CANCEL ? "Save was cancelled by user" : "Failed to save by pointer");
                }
                break;
            }
          break;
        }
      }
    }
  }
}