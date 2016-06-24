#include <Wire.h>
#define LED_PIN 13

//define addresses
#define ADDRESS_COMBINE 2
#define ADDRESS_PACKAGE 3
#define ADDRESS_WRAPPER 4
#define ADDRESS_STAMPER 5


//define station-pins here

//COLOR_DETECT
//---DO
#define COLOR_DETECT_ENABLE 2 
//---DI
#define COLOR_DETECT_READ_SUCCESS 3
#define COLOR_DETECT_MSB 4
#define COLOR_DETECT_LSB 5

//CARGO_CHECK
//---DO
//---DI

#define CARGO_PLASTIC_PIN 11
#define CARGO_SCREW_PIN 12

#define COLOR_LIGHT_BARRIER_PIN A0
#define STAMP_LIGHT_BARRIER_PIN A1

#define COMBINE_PIN 6
#define RUN_CONV_PIN 7


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

#define CARGO_PLASTIC 1
#define CARGO_SCREWS 2

byte readByte(uint8_t address);

void setup() {
  //pinMode(,INPUT);
  //pinMode(, OUTPUT);

  //color detect
  pinMode(COLOR_DETECT_ENABLE, OUTPUT);
  digitalWrite(COLOR_DETECT_ENABLE, LOW);
  pinMode(COLOR_DETECT_READ_SUCCESS, INPUT);
  pinMode(COLOR_DETECT_MSB,INPUT);
  pinMode(COLOR_DETECT_LSB,INPUT);

  pinMode(CARGO_PLASTIC_PIN, INPUT);
  pinMode(CARGO_SCREW_PIN, INPUT);

  pinMode(COLOR_LIGHT_BARRIER_PIN, INPUT);
  pinMode(STAMP_LIGHT_BARRIER_PIN, INPUT);

  pinMode(COMBINE_PIN, OUTPUT);
  digitalWrite(COMBINE_PIN, LOW);

  pinMode(RUN_CONV_PIN, OUTPUT);
  digitalWrite(RUN_CONV_PIN, LOW);

}

void loop() {
  uint8_t statTmp = 0;
  uint8_t cargo = 0;
  boolean allThere = false;
  
  while(!allThere){
    if(digitalRead(CARGO_PLASTIC_PIN)){
      delay(100);
      cargo |= CARGO_PLASTIC;
    }

    if(digitalRead(CARGO_SCREW_PIN)){
      delay(100);
      cargo |= CARGO_SCREWS;
    }

    if((cargo & CARGO_PLASTIC)&&(cargo && CARGO_SCREWS)) allThere = true;
  }


  allThere = false;
  //check if cargo has arrived
  
  

  //pick up boxes
  
  //analyse
  digitalWrite(RUN_CONV_PIN, HIGH);
  while(analogRead(COLOR_LIGHT_BARRIER_PIN) < 8);
  digitalWrite(RUN_CONV_PIN, LOW);
  
  digitalWrite(COLOR_DETECT_ENABLE, HIGH);
  //while(digitalRead(COLOR_DETECT_READ_SUCCESS));
  delay(3000);
  uint8_t color_id = 0;
  if(digitalRead(COLOR_DETECT_LSB)) color_id != 1;
  if(digitalRead(COLOR_DETECT_MSB)) color_id != 2;
  digitalWrite(COLOR_DETECT_ENABLE, LOW);
  
  //combine
  Wire.beginTransmission(ADDRESS_COMBINE);
  Wire.write(ACTION_1 | ENABLE);
  Wire.endTransmission();
  while(!(readByte(ADDRESS_COMBINE) & GOON)) delay(500);
  Wire.beginTransmission(ADDRESS_COMBINE);
  Wire.write(0);
  Wire.endTransmission();

  digitalWrite(COMBINE_PIN, HIGH);
  delay(1000);
  digitalWrite(COMBINE_PIN, LOW);
  digitalWrite(RUN_CONV_PIN, HIGH);
   /*
  //package
  Wire.beginTransmission(ADDRESS_PACKAGE);
  Wire.write(ENABLE);
  Wire.endTransmission();
  while(!(readByte(ADDRESS_PACKAGE) & GOON)) delay(500);
  Wire.beginTransmission(ADDRESS_PACKAGE);
  Wire.write(0);
  Wire.endTransmission();
*/
/*
  //wrap
  Wire.beginTransmission(ADDRESS_WRAPPER);
  Wire.write(ENABLE | ACTION_1);
  Wire.endTransmission();
  while(!(readByte(ADDRESS_PACKAGE) & GOON)) delay(500);
  Wire.beginTransmission(ADDRESS_WRAPPER);
  Wire.write(0);
  Wire.endTransmission();

  //stamp
*/
  while(analogRead(STAMP_LIGHT_BARRIER_PIN) < 8);
  digitalWrite(RUN_CONV_PIN, LOW);
  Wire.beginTransmission(ADDRESS_STAMPER);
  if(color_id == 0){
    Wire.write(ENABLE | ACTION_1);
  }else if(color_id == 1){
    Wire.write(ENABLE | ACTION_2);
  }else if(color_id == 2){
    Wire.write(ENABLE | ACTION_3);
  }else if(color_id == 3){
    Wire.write(ENABLE | ACTION_4);
  }
  Wire.endTransmission();
  delay(500);
  while(!(readByte(ADDRESS_STAMPER) & GOON)) Serial.println("."), delay(500);
  Wire.beginTransmission(ADDRESS_STAMPER);
  Wire.write(0);
  Wire.endTransmission();
  digitalWrite(RUN_CONV_PIN, HIGH);

  //deliver

  Wire.beginTransmission(ADDRESS_WRAPPER);
  Wire.write(ENABLE | ACTION_2);
  Wire.endTransmission();
  while(!(readByte(ADDRESS_PACKAGE) & GOON)) delay(500);
  Wire.beginTransmission(ADDRESS_WRAPPER);
  Wire.write(0);
  Wire.endTransmission();
}

byte readByte(uint8_t address){
  uint8_t tmp = 0;
  
  Wire.requestFrom(address, 1);    // request 1 bytes from slave device #1

  while(Wire.available()) { // slave may send less than requested
    tmp = Wire.read(); // receive a byte as character
    while(Wire.available());
  }

  return tmp;
}
