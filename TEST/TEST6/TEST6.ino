#include <SoftwareSerial.h>
SoftwareSerial mySerial(2,3);  // RX, TX pins for software serial

const byte relays_num = 4;
byte relays[relays_num] = {6, 7, 8, 9};
byte Buffer[10];
byte Buffer_Len = 0;
bool flag = false;


void setup() 
{
  Serial.begin(9600);
  mySerial.begin(115200);  // Set the baud rate for software serial
  for (byte i = 0; i < relays_num; i++) 
  {
    pinMode(relays[i], OUTPUT);
    digitalWrite(relays[i], HIGH);
  } 
}

void loop()
{
  if (mySerial.available())
  {
    byte receivedByte = mySerial.read();
    Buffer[Buffer_Len] = receivedByte;
    Buffer_Len++;
    flag = true;

    // Serial print the received byte
    Serial.print("Received Byte: 0x");
    if (receivedByte < 0x10)
    {
      Serial.print("0");
    }
    Serial.println(receivedByte, HEX);
  }
  else
  {
    if (flag)
    {
      if (Buffer[0] == 0X5A)
      {
        if (Buffer[4] == 0X20)
        {
          for (byte i = 0; i < relays_num; i++)
          {
            bool relayState = !bitRead(Buffer[8], i);
            digitalWrite(relays[i], relayState);
          }
        }
      }
      Buffer_Len = 0;
      flag = false;
    }
  }
}
