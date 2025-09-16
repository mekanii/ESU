/* For Software serial */
#include <SoftwareSerial.h>
const byte rxPin = 2;
const byte txPin = 3;

SoftwareSerial mySerial (rxPin, txPin);
/* For BME280 sensor */
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C
/* appliances pins */
int light = 4;
int fan = 5;
int ac = 6;
int tv = 7;

/* Adresses of all sensors */
unsigned char Buffer[9];

#define temperature_add 0x10
#define humidity_add 0x20
#define pressure_add 0x30
#define altitude_add 0x40
#define dewpoint_add 0x50

unsigned char Temperature[8] = {0x5a, 0xa5, 0x05, 0x82, temperature_add , 0x00, 0x00, 0x00};
unsigned char Humidity[8] = {0x5a, 0xa5, 0x05, 0x82, humidity_add, 0x00, 0x00, 0x00};
unsigned char Pressure[8] = {0x5a, 0xa5, 0x05, 0x82, pressure_add , 0x00, 0x00, 0x00};
unsigned char Altitude[8] = {0x5a, 0xa5, 0x05, 0x82, altitude_add, 0x00, 0x00, 0x00};
unsigned char DewPoint[8] = {0x5a, 0xa5, 0x05, 0x82, dewpoint_add, 0x00, 0x00, 0x00};

void setup() {
  Serial.begin(115200);
  mySerial.begin(115200);

  unsigned status;
  status = bme.begin();
  if (!status) {
    Serial.println("Could not find a valid  BME280 sensor, check wiring, address, sensor ID!");
    while (1) delay(10);
  }

  pinMode(light, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(ac, OUTPUT);
  pinMode(tv, OUTPUT);
  
  digitalWrite(light, HIGH);
  digitalWrite(fan, HIGH);
  digitalWrite(ac, HIGH);
  digitalWrite(tv, HIGH);
}

void loop() {
  Data_Arduino_to_Display();
  delay(50);
  Data_Display_to_Arduino();
  delay(80);
}

void Data_Display_to_Arduino() {
  if (mySerial.available()) {
    for (int i = 0; i <= 8; i++) { //this loop will store whole frame in buffer array.
      Buffer[i] = mySerial.read();
    }
    if (Buffer[0] == 0X5A) { // 5A A5 [LEN] [CMD] [DATA]
      switch (Buffer[4]) {
        case 0x51:   //for light
          if (Buffer[8] == 1) {
            digitalWrite(light, LOW);
          } else {
            digitalWrite(light, HIGH);
          }
          break;
        case 0x52:   //for fan
          if (Buffer[8] == 1) {
            digitalWrite(fan, LOW);
          } else {
            digitalWrite(fan, HIGH);
          }
          break;
        case 0x53:   //for AC
          if (Buffer[8] == 1) {
            digitalWrite(ac, LOW);
          } else {
            digitalWrite(ac, HIGH);
          }
          break;
        case 0x54:   //for TV
          if (Buffer[8] == 1) {
            digitalWrite(tv, LOW);
          } else {
            digitalWrite(tv, HIGH);
          }
          break;
        default:
          break;
      }
    }
  }
}

void Data_Arduino_to_Display()
{
  int t = bme.readTemperature();
  int h = bme.readHumidity();
  int p = bme.readPressure() / 100.0F;
  int a = bme.readAltitude(SEALEVELPRESSURE_HPA);
  int d = dewPointFast(t, h);
  /*------Print data to Serial Monitor------*/
    Serial.print("Temperature = ");
  Serial.print(t);
  Serial.println(" °C");
  Serial.print("Humidity = ");
  Serial.print(h);
  Serial.println(" %");
  Serial.print("Pressure = ");
  Serial.print(p);
  Serial.println(" hPa");
  Serial.print("Approx. Altitude = ");
  Serial.print(a);
  Serial.println(" m");
  Serial.print("DewPoint = ");
  Serial.print(d);
  Serial.println(" °C");
  Serial.println();
  Temperature[6] = highByte(t);
  Temperature[7] = lowByte(t);
  mySerial.write(Temperature, 8);
  Humidity[6] = highByte(h);
  Humidity[7] = lowByte(h);
  mySerial.write(Humidity, 8);
  Pressure[6] = highByte(p);
  Pressure[7] = lowByte(p);
  mySerial.write(Pressure, 8);
  Altitude[6] = highByte(a);
  Altitude[7] = lowByte(a);
  mySerial.write(Altitude, 8);
  DewPoint[6] = highByte(d);
  DewPoint[7] = lowByte(d);
  mySerial.write(DewPoint, 8);
}

/*----------DewPoint Calculation--------*/
double dewPointFast(double celsius, double humidity)
{
  double a = 17.271;
  double b = 237.7;
  double temp = (a * celsius) / (b + celsius) + log(humidity * 0.01);
  double Td = (b * temp) / (a - temp);
  return Td;
}