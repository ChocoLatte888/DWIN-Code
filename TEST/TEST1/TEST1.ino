const byte rxPin = 7;
const byte txPin = 8;
unsigned char Buffer[9];
int ledku = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(ledku, OUTPUT);
  digitalWrite(ledku, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    for (int i = 0; i <= 8; i++)
    {
      Buffer[i] = Serial.read();
    }

    if (Buffer[0] == 0X5A)
    {
      switch (Buffer[4])
      {  
        case 0x55:
        if (Buffer[8] == 1)
        {
          digitalWrite(ledku, HIGH);
          Serial.println("LED ON"); 
        }
        else
        {
          digitalWrite(ledku, LOW);
          Serial.println("LED OFF");
        }
       

        default:
        Serial.println("...");
        break;
      }
    }
  }
  delay(10);
}
