#include <Arduino.h>
#define rxPin 7
#define txPin 8
#define intervalbar 700
#define p1r 0x11
#define p1g 0x12
#define p2r 0x13
#define p2g 0x14
#define p3r 0x15
#define p3g 0x16
#define p4r 0x17
#define p4g 0x18
#define GPIO2 2
#define GPIO4 4
#define GPIO36 36

// Use Serial2 for your custom Serial2
#define dwinSerial Serial2

// Function declarations
unsigned char data1[8] = {0x5A, 0xA5, 0x05, 0x82, p1r, 0x00, 0x00, 0x00};
unsigned char data2[8] = {0x5A, 0xA5, 0x05, 0x82, p1g, 0x00, 0x00, 0x00};
unsigned char data3[8] = {0x5A, 0xA5, 0x05, 0x82, p2r, 0x00, 0x00, 0x00};
unsigned char data4[8] = {0x5A, 0xA5, 0x05, 0x82, p2g, 0x00, 0x00, 0x00};
unsigned char data5[8] = {0x5A, 0xA5, 0x05, 0x82, p3r, 0x00, 0x00, 0x00};
unsigned char data6[8] = {0x5A, 0xA5, 0x05, 0x82, p3g, 0x00, 0x00, 0x00};
unsigned long timereceived = 0;

int reset = 0;
byte sensor = GPIO2;
byte sensor1 = GPIO4;
byte sensor2 = GPIO36;
int nilai, nilai1, nilai2, output, output1, output2, percent, percent1, percent2, datac1, datac2, dataf1, dataf2;

void setup() {
  Serial.begin(115200);  // Use Serial for debugging
  Serial2.begin(115200); // Use Serial2 for your custom Serial2
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
}

void loop() {
  unsigned long receivedtime = millis();
  if (receivedtime - timereceived >= intervalbar) {
    nilai = analogRead(sensor);
    nilai1 = analogRead(sensor1);
    nilai2 = analogRead(sensor2);
    output = map(nilai, 0, 1023, 0, 255);
    output1 = map(nilai1, 0, 1023, 0, 255);
    output2 = map(nilai2, 0, 1023, 0, 255);
    percent = (output / 2.54) * 2;
    percent1 = (output1 / 2.54) * 2;
    percent2 = (output2 / 2.54) * 2;
    datac1 = percent - 100;
    dataf1 = percent1 - 100;
    datac2 = percent2 - 100;
    Serial.println(datac1);
    // Rest of your code remains the same...
    /*
    if (datac1 < 0) {
      int datacvt;
      datacvt = datac1 * -1;
      data1[6] = highByte(datacvt);
      data1[7] = lowByte(datacvt);
      dwinSerial.write(data1, 8);
      Serial.print("C1R: ");
      Serial.println(datacvt);
      for (int x = 0; x <= 5; x++) {
        data2[6] = highByte(0);
        data2[7] = lowByte(0);
        dwinSerial.write(data2, 8);
      }
    } else {
      data2[6] = highByte(datac1);
      data2[7] = lowByte(datac1);
      dwinSerial.write(data2, 8);
      Serial.print("C1G: ");
      Serial.println(datac1);
      for (int x = 0; x <= 5; x++) {
        data1[6] = highByte(0);
        data1[7] = lowByte(0);
        dwinSerial.write(data1, 8);
      }
    }

    if (dataf1 < 0) {
      int datacvt3;
      datacvt3 = dataf1 * -1;
      data3[6] = highByte(datacvt3);
      data3[7] = lowByte(datacvt3);
      dwinSerial.write(data3, 8);
      Serial.print("F1R: ");
      Serial.println(datacvt3);
      for (int x = 0; x <= 5; x++) {
        data4[6] = highByte(0);
        data4[7] = lowByte(0);
        dwinSerial.write(data4, 8);
      }
    } else {
      data4[6] = highByte(dataf1);
      data4[7] = lowByte(dataf1);
      dwinSerial.write(data4, 8);
      Serial.print("F1G: ");
      Serial.println(dataf1);
      for (int x = 0; x <= 5; x++) {
        data3[6] = highByte(0);
        data3[7] = lowByte(0);
        dwinSerial.write(data3, 8);
      }
    }

    if (datac2 < 0) {
      int datacvt2;
      datacvt2 = datac2 * -1;
      data5[6] = highByte(datacvt2);
      data5[7] = lowByte(datacvt2);
      dwinSerial.write(data5, 8);
      Serial.print("C2R: ");
      Serial.println(datacvt2);
      for (int x = 0; x <= 5; x++) {
        data6[6] = highByte(0);
        data6[7] = lowByte(0);
        dwinSerial.write(data6, 8);
      }
    } else {
      data6[6] = highByte(datac2);
      data6[7] = lowByte(datac2);
      dwinSerial.write(data6, 8);
      Serial.print("C2G: ");
      Serial.println(datac2);
      for (int x = 0; x <= 5; x++) {
        data5[6] = highByte(0);
        data5[7] = lowByte(0);
        dwinSerial.write(data5, 8);
      }
    }
    */
    timereceived = receivedtime;
  }
}