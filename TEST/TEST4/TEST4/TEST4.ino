#include <SoftwareSerial.h>
const byte rxPin = 7;
const byte txPin = 8;

SoftwareSerial dwinSerial (rxPin, txPin);
unsigned char incomingData[100];
unsigned char textString[100];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dwinSerial.begin(115200);
  Serial.print("DWIN DISPLAY TEST");
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  delay(500);
}

void loop() {
  // put your main code here, to run repeatedly:
  sendText(); //send to display
  fetchText(); //receive from display
}

void fetchText(){
  if (dwinSerial.available() > 0) //available() returns the number of bytes stored in the buffer
  {
    int i = 0; //position in the buffer array
    
    while (dwinSerial.available() > 0 ) //empty the buffer into our array
    {
      char incomingByte = dwinSerial.read(); //read 1 character from the dwin display
      delay(2);
      incomingData[i] = incomingByte; //put the character in the array
      i++;
      //Serial.write(incomingByte);
    }

    if (incomingData[3] == (byte)0x83) //VP read instruction is in the returned char array
    {
      int k = 7; //starting point for the message - it is always a fixed distance
      while(incomingData[k] != 0xFF) //end character is 0xFFFF
      {
        Serial.write(incomingData[k]);
        k++;
      }
      Serial.println();
    }
    memset(incomingData, 0, sizeof(incomingData)); //Empty array
  }
}

void sendText()
{
  if (Serial.available() > 0) //Wait for the pc serial because the data is coming from there
  {
    while (Serial.available()>0)
    {
      eraseText(0x20, 100); //Erase display
      memset(textString, 0, sizeof(textString)); //Empty array
    
      int numBytes = Serial.available();
      //Serial.println(numBytes);

      for (int n = 0; n < numBytes; n++)
      {
        textString[n] = Serial.read();
      }

      dwinSerial.write(0x5A); //header
      dwinSerial.write(0xA5); //header
      dwinSerial.write(strlen(textString)+4); //Length: Write command(1), VP address (2), Extra space (1)
      dwinSerial.write(0x82); //write command
      dwinSerial.write(0x20); //write address
      dwinSerial.write((byte)0x00); //write address
      dwinSerial.write(0x20); //add an extra space to indent the text
      dwinSerial.write(textString, strlen(textString));
    }
  }
}

void eraseText (int vpAddress, int textLength)
{
  dwinSerial.write(0x5A); //header
  dwinSerial.write(0xA5); //header
  dwinSerial.write(3 + textLength); //Length (3: write, address low and high bytes
  dwinSerial.write(0x82); //write command
  dwinSerial.write(lowByte(vpAddress)); //write address
  dwinSerial.write(highByte(vpAddress)); //write address
  
  for (int i = 0; i < textLength; i++)
  {
    dwinSerial.write((byte)0x20); //write message (0x20 is ASCII code for space)
  }
}
