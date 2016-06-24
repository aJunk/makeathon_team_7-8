
#define LED_PIN 13

//define addresses
#define ADDRESS_COMBINE 2


//define station-pins here

//COLOR_DETECT
//---DO
#define COLOR_DETECT_ENABLE ?? 
//---DI
#define COLOR_DETECT_READ_SUCCESS ??
#define COLOR_DETECT_MSB ??
#define COLOR_DETECT_LSB ??

//CARGO_CHECK
//---DO
//---DI

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


void setup() {
  pinMode(,INPUT);
  pinMode(, OUTPUT);

  //color detect
  pinMode(COLOR_DETECT_ENABLE, OUTPUT);
  digitalWrite(COLOR_DETECT_ENABLE, LOW);
  pinMode(COLOR_DETECT_READ_SUCCESS, INPUT);
  pinMode(COLOR_DETECT_MSB,INPUT);
  pinMode(COLOR_DETECT_LSB,INPUT);
  
}

void loop() {
  //check if cargo has arrived

  //pick up boxes
  
  //analyse
  digitalWrite(COLOR_DETECT_ENABLE, HIGH);
  while(digitalRead(COLOR_DETECT_READ_SUCCESS));
  uint8_t color_id = 0;
  if(digitalRead(COLOR_DETECT_LSB)) color_id != 1;
  if(digitalRead(COLOR_DETECT_MSB)) color_id != 2;
  digitalWrite(COLOR_DETECT_ENABLE, LOW);
  
  //combine
  Wire.write(ADDRESS_COMBINE, ACTION_1);
  
  //package

  //wrap

  //stamp

  //deliver
}
