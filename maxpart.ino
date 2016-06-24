#include <Servo.h>
#include <Wire.h>

//change address
#define  SLAVE_ADDRESS 5  //sl aveaddress, any number from 0x01 to 0x7F

//CMD-defines
#define ENABLE 0b00000001
#define ACTION_1 0b00000010
#define ACTION_2 0b00000100
#define ACTION_3 0b00001000
#define ACTION_4 0b00010000
#define ACTION_5 0b00100000
#define ACTION_6 0b01000000
#define ACTION_7 0b10000000

//SEND-defines
#define GOON     0b00000001
#define ERR     0b00000010
#define IN_PROGRESS 0b00000100
#define STATUS_1  0b00001000
#define STATUS_2  0b00010000
#define STATUS_3  0b00100000
#define STATUS_4  0b01000000
#define STATUS_5  0b10000000


//hardcoded container-sizes
#define  REG_MAP_SIZE 2
#define  MAX_SENT_BYTES 2

int Magnet = 4;
int i;
int colour=5;

Servo servobase;
Servo servolinks;
Servo servorechts;

/********* Global  Variables  ***********/
//store variables for sending to station here
byte registerMap[REG_MAP_SIZE] = {0};
volatile byte receivedCommands[MAX_SENT_BYTES] = {0};


void stempeln()
{
  int i;
  for (i = 0; i < 30; i++)
  {
    servobase.write(40 + 90 / 30 * i);
    delay(70);
  }
  delay(100);
  for (i = 0; i < 10; i++)
  {
    servorechts.write(20 +10/ 10 * i);
    servolinks.write(40 - 29 / 10 * i);
    delay(70);
  }
  delay (200);
  for (i = 10; i >= 0; i--)
  {
    servorechts.write(20 +10 / 10 * i);
    servolinks.write(40 - 29 / 10 * i);
    delay(70);
  }
  for (i = 30; i > 0; i--)
  {
    servobase.write(40 + 90 / 30 * i);
    delay(70);
  }
}

void requestEvent()
{
  Wire.write(registerMap, REG_MAP_SIZE);  //Set the buffer up to send all 14 bytes of data
}

void receiveEvent(int bytesReceived)
{
  Serial.print("ff");
  for (int a = 0; a < bytesReceived; a++) {
    if ( a < MAX_SENT_BYTES) {
      receivedCommands[a] = Wire.read();
    } else {
      Wire.read();  // if we receive more data then allowed just throw it away
    }
  }
}

void setup()
{
  pinMode(Magnet, OUTPUT);
  Wire.begin(SLAVE_ADDRESS);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  servobase.attach(6);
  servolinks.attach(5);
  servorechts.attach(3);
  servobase.write(40);
  servorechts.write(20);
  servolinks.write(40);
  Serial.begin(115200);
}

void loop()
{
 // send data to central!
  registerMap[0] = 0;
 // registerMap[0] |= IN_PROGRESS;
Serial.println(registerMap[0]);
delay(400);
  if (receivedCommands[0] & ENABLE)
  {
      if (receivedCommands[0] & ACTION_1)   
  {
    colour=0;
  }
    else  if (receivedCommands[0] & ACTION_2)
  {
    colour=1;
  }
     else   if (receivedCommands[0] & ACTION_3)
  {
    colour=2;
  }
     else   if (receivedCommands[0] & ACTION_4)
  {
   colour=3;
  }
  else
  {
    colour=5;
  }
  }

  switch (colour)
  {
    case 0:
      {
        for (i = 0; i < 30; i++)
        {
          servobase.write(40 + 30 / 30 * i);
          servorechts.write(20 + 65 / 30 * i);
          servolinks.write(40 - 25 / 30 * i);
          delay(70);
        }
       digitalWrite(Magnet, HIGH);
        delay(100);
        for (i = 30; i >= 0; i--)
        {
          servobase.write(40 + 30 / 30 * i);
          servorechts.write(20 + 65 / 30 * i);
          servolinks.write(40 - 25 / 30 * i);
          delay(70);
        }
        delay(200);
        stempeln();
        for (i = 0; i < 30; i++)
        {
          servobase.write(40 + 30 / 30 * i);
          servorechts.write(20 + 65 / 30 * i);
          servolinks.write(40 - 25 / 30 * i);
          delay(70);
        }
        digitalWrite(Magnet, LOW);
        delay(200);
        for (i = 30; i >= 0; i--)
        {
          servobase.write(40 + 30 / 30 * i);
          servorechts.write(20 + 65 / 30 * i);
          servolinks.write(40 - 25 / 30 * i);
          delay(80);
        }
        delay(200);        
        break;
      }
    case 1:
      {

        
          for (i = 0; i < 30; i++)
          {
            servobase.write(40 + (29 / 30) * i);
            servorechts.write(20 - (5 / 30) * i);
            servolinks.write(40 - (32 / 30) * i);
            delay(70);
          }
         digitalWrite(Magnet, HIGH);
          delay(100);
          for (i = 30; i >= 0; i--)
          {
            servobase.write(40 + (29 / 30) * i);
            servorechts.write(20 -( 5 / 30) * i);
            servolinks.write(40 - (32 / 30) * i);
            delay(70);
          }
          delay(500);
          stempeln();
          for (i = 0; i < 30; i++)
          {
            servobase.write(40 + (29 / 30) * i);
            servorechts.write(20 - (5 / 30) * i);
            servolinks.write(40 - (32 / 30) * i);
            delay(70);
          }
          digitalWrite(Magnet, LOW);
          delay(100);
          for (i = 30; i >= 0; i--)
          {
            servobase.write(40 + (29 / 30) * i);
            servorechts.write(20 - (5 / 30) * i);
            servolinks.write(40 - (32 / 30) * i);
            delay(70);
          }
          delay(100);
          break;
      }
        case 2:
          {
            for (i = 0; i < 30; i++)
            {
              servobase.write(40 - 30 / 30 * i);
              servorechts.write(20 + 65 / 30 * i);
              servolinks.write(40 - (29/30)*i);
              delay(70);
            }
          digitalWrite(Magnet, HIGH);
            delay(100);
            for (i = 30; i >= 0; i--)
            {
              servobase.write(40 - 30 / 30 * i);
              servorechts.write(20 + 65 / 30 * i);
              servolinks.write(40 - (29/30)*i);
              delay(70);
            }
            delay(500);
            stempeln();
            for (i = 0; i < 30; i++)
            {
              servobase.write(40 - 30 / 30 * i);
              servorechts.write(20 +65 / 30 * i);
              servolinks.write(40 -(29/30) * i);
              delay(70);
            }
            digitalWrite(Magnet, LOW);
            delay(100);
            for (i = 30; i >= 0; i--)
            {
              servobase.write(40 - 30 / 30 * i);
              servorechts.write(20 + 65 / 30 * i);
              servolinks.write(40 -  (29/30)*i);
              delay(70);
            }
            delay(100);
            break;
          }
        case 3:
          {
            for (i = 0; i < 30; i++)
            {
              servobase.write(40 - 30 / 30 * i);
              servorechts.write(20 - 5 / 30 * i);
              servolinks.write(40 - 32 / 30 * i);
              delay(70);
            }
           digitalWrite(Magnet, HIGH);
            delay(100);
            for (i = 30; i >= 0; i--)
            {
              servobase.write(40 - 30 / 30 * i);
              servorechts.write(20 - 5 / 30 * i);
              servolinks.write(40 - 32 / 30 * i);
              delay(70);
            }
            delay(500);
            stempeln();
            for (i = 0; i < 30; i++)
            {
              servobase.write(40 - 30 / 30 * i);
              servorechts.write(20 - 5 / 30 * i);
              servolinks.write(40 - 32 / 30 * i);
              delay(70);
            }
            digitalWrite(Magnet, LOW);
            delay(100);
            for (i = 30; i >= 0; i--)
            {
              servobase.write(40 - 30 / 30 * i);
              servorechts.write(20 - 5 / 30 * i);
              servolinks.write(40 - 32 / 30 * i);
              delay(70);
            }
            delay(100);
            break;
          }
          default:
          {
            break;
          }

        }
               registerMap[0] |= GOON;
        while(receivedCommands[0] & ENABLE);
      }
  


