  #include <Arduino.h>
  #include <SoftwareSerial.h>
  const byte rxPin = 7;
  const byte txPin = 8;
  SoftwareSerial dwinSerial (rxPin, txPin);
  // put function declarations here:
  unsigned char data [8] = {0x5A, 0xA5, 0x05, 0x82, 0x48, 0x00, 0x00, 0x00}; //header, header, 5 bytes to follow, write command, address, address, address
  unsigned char data2 [8] = {0x5A, 0xA5, 0x05, 0x82, 0x64 , 0x00, 0x00, 0x00}; //header, header, 5 bytes to follow, write command, address, address, address
  unsigned char c1 [8] = {0x5A, 0xA5, 0x05, 0x82, 0x47 , 0x00, 0x00, 0x00}; //header, header, 5 bytes to follow, write command, address, address, address
  unsigned char c2 [8] = {0x5A, 0xA5, 0x05, 0x82, 0x46 , 0x00, 0x00, 0x00}; //header, header, 5 bytes to follow, write command, address, address, address
  unsigned char Buffer[9];
  int ledku = 13;
  byte sensor= A0;
  byte sensor2= A2;
  int nilai,nilai2,output,output2,percent,percent2,mydata,mydata2,datac1,datac2;

  void setup(){
  Serial.begin(115200);
  dwinSerial.begin(115200);  
  pinMode(ledku, OUTPUT);
  digitalWrite(ledku, LOW);
  }

  void loop(){
  if (Serial.available())
    {
      for (int i = 0; i <= 8; i++) //this loop will store whole frame in buffer array.
      {
        Buffer[i] = Serial.read();
      }
  
      if (Buffer[0] == 0X5A)
      {
        switch (Buffer[4])
        {
          /*case 0x51:   //for servo
            Serial.println(Buffer[8]);
            analogWrite(servo, Buffer[8]);
            break;
  
          case 0x52:   //for red
            Serial.println(Buffer[8]);
            analogWrite(red, Buffer[8]);
            break;
  
          case 0x53:   //for green
            Serial.println(Buffer[8]);
            analogWrite(green, Buffer[8]);
            break;
  
  
          case 0x54:   //for blue
            Serial.println(Buffer[8]);
            analogWrite(blue, Buffer[8]);
            break; */
  
          case 0x55:
            if (Buffer[8] == 1)
            { digitalWrite(ledku, LOW);
              Serial.println("LEDKU OFF");
            }
            /*
            else
            { digitalWrite(ledku, LOW);
              Serial.println("LEDKU OFF");
            }*/
            break;
  
  
          default:
            Serial.println("Nothing");
            break;
        }
      }
    }
    delay(10);

  nilai= analogRead(sensor);
  nilai2= analogRead(sensor2);
  output= map(nilai, 0, 1023, 0, 255);
  output2= map(nilai2, 0, 1023, 0, 255);
  percent = (output/2.54)*2;
  percent2 = (output2/2.54)*2;
  datac1 = percent - 100;
  datac2 = percent2 - 100;

  if (datac1 == 100){
    digitalWrite(ledku, HIGH);
  }
  else {
    digitalWrite(ledku, LOW);
  }

    Serial.print("sensor: ");
    Serial.println(nilai);
    Serial.print("output: ");
    Serial.println(output);
    Serial.print("persentase: ");
    Serial.println(percent);
    Serial.println("  "); 
    Serial.print("sensor2: ");
    Serial.println(nilai2);
    Serial.print("output2: ");
    Serial.println(output2);
    Serial.print("persentase2: ");
    Serial.println(percent2);
    Serial.println("  "); 

    c1[6]=highByte(datac1);
    c1[7]=lowByte(datac1);
    c2[6]=highByte(datac2);
    c2[7]=lowByte(datac2);
    data[6]=highByte(percent);
    data[7]=lowByte(percent);
    data2[6]=highByte(percent2);
    data2[7]=lowByte(percent2);
    dwinSerial.write(data,8); //buffer,length
    dwinSerial.write(data2,8);
    dwinSerial.write(c1, 8);
    dwinSerial.write(c2, 8);
    delay(1000);
  }