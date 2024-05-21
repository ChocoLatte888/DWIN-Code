#include <Arduino.h>
#include <SoftwareSerial.h>
#define rxPin 7 
#define txPin 8
#define intervalsend 10000
#define intervalbar 500
SoftwareSerial dwinSerial (rxPin, txPin);
// put function declarations here:
unsigned char c1r [8] = {0x5A, 0xA5, 0x05, 0x82, 0x42 , 0x00, 0x00, 0x00}; //header, header, 5 bytes to follow, write command, address, address, address
unsigned char c1g [8] = {0x5A, 0xA5, 0x05, 0x82, 0x42 , 0x00, 0x00, 0x00}; //header, header, 5 bytes to follow, write command, address, address, address
unsigned char f1r [8] = {0x5A, 0xA5, 0x05, 0x82, 0x43 , 0x00, 0x00, 0x00}; //header, header, 5 bytes to follow, write command, address, address, address
unsigned char f1g [8] = {0x5A, 0xA5, 0x05, 0x82, 0x43 , 0x00, 0x00, 0x00}; //header, header, 5 bytes to follow, write command, address, address, address
unsigned char c2r [8] = {0x5A, 0xA5, 0x05, 0x82, 0x44 , 0x00, 0x00, 0x00}; //header, header, 5 bytes to follow, write command, address, address, address
unsigned char c2g [8] = {0x5A, 0xA5, 0x05, 0x82, 0x44 , 0x00, 0x00, 0x00}; //header, header, 5 bytes to follow, write command, address, address, address
unsigned char f2r [8] = {0x5A, 0xA5, 0x05, 0x82, 0x45 , 0x00, 0x00, 0x00}; //header, header, 5 bytes to follow, write command, address, address, address
unsigned char f2g [8] = {0x5A, 0xA5, 0x05, 0x82, 0x45 , 0x00, 0x00, 0x00}; //header, header, 5 bytes to follow, write command, address, address, address
unsigned char Buffer[9];
unsigned char incomingData[100];
char textString[100];
unsigned long timersend = 0;
unsigned long timereceived = 0;

int ledku = 13;
byte sensor= A0;
byte sensor2= A2;
int nilai,nilai2,output,output2,percent,percent2,mydata,mydata2,datac1,datac2,dataf1,dataf2;
void sendText();
void eraseText (int vpAddress, int textLength);
void fetchText();

void setup(){
Serial.begin(115200);
dwinSerial.begin(115200);  
pinMode(ledku, OUTPUT);
digitalWrite(ledku, LOW);
pinMode(rxPin, INPUT);
pinMode(txPin, OUTPUT);
}

void loop(){
  sendText(); //send to display
  fetchText(); //receive from display

unsigned long receivedtime = millis();
if (receivedtime-timereceived >= intervalbar)
{
nilai= analogRead(sensor);
nilai2= analogRead(sensor2);
output= map(nilai, 0, 1023, 0, 255);
output2= map(nilai2, 0, 1023, 0, 255);
percent = (output/2.54)*2;
percent2 = (output2/2.54)*2;
datac1 = percent - 100;
dataf1 = percent - 100;
datac2 = percent2 - 100;
dataf2 = percent2 - 100;

  
  //Serial.print("sensor: ");
  //Serial.println(nilai);
  //Serial.print("output: ");
  //Serial.println(output);
  Serial.print("persentase: ");
  Serial.println(percent);
  Serial.print("Data C1: ");
  Serial.println(datac1);
  Serial.println("  "); 
  //Serial.print("sensor2: ");
  //Serial.println(nilai2);
  //Serial.print("output2: ");
  //Serial.println(output2);
  Serial.print("persentase2: ");
  Serial.println(percent2);
  Serial.print("Data C2: ");
  Serial.println(datac2);
  Serial.println("  "); 

  
  
  if (datac1 >= 0){
  c1g[6]=highByte(datac1);
  c1g[7]=lowByte(datac1);
  }
  else{
  datac1=datac1*-1;
  c1r[6]=highByte(datac1);
  c1r[7]=lowByte(datac1);
  }

  if (dataf1 >= 0){
  f1r[6]=highByte(dataf1);
  f1g[7]=lowByte(dataf1);
  }
  else{
  dataf1=dataf1*-1;
  f1r[6]=highByte(dataf1);
  f1g[7]=lowByte(dataf1);
  }

  if (datac2 >= 0){
  c2r[6]=highByte(datac2);
  c2g[7]=lowByte(datac2);
  }
  else{
  datac2=datac2*-1;
  c2r[6]=highByte(datac2);
  c2g[7]=lowByte(datac2);
  }

  if (dataf2 >= 0){
  c2r[6]=highByte(dataf2);
  c2g[7]=lowByte(dataf2);
  }
  else{
  dataf2=dataf2*-1;
  c2r[6]=highByte(dataf2);
  c2g[7]=lowByte(dataf2);
  }

  dwinSerial.write(c1g,8);
  dwinSerial.write(c1r,8);
  dwinSerial.write(c2g,8);
  dwinSerial.write(c2r,8);
  timereceived = receivedtime;
  }
}

void fetchText(){ //dwin > arduino
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

void sendText() //arduino > dwin
{
  if (Serial.available() > 0) //Wait for the pc serial because the data is coming from there
  {
    while (Serial.available()>0)
    {
      unsigned long sendingtime = millis();
      if (sendingtime-timersend >= intervalsend)
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

      timersend = sendingtime;
      }
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