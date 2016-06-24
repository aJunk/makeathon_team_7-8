#include <Servo.h>

Servo dauer;
Servo kleiner;
Servo kipper;
Servo blocker;

int servoHuellePin = 8;
int servoSchSchPin = 9;



//=====================================================================

void setup() {
  dauer.attach(8);
  kleiner.attach(9);
  kipper.attach(10);
  blocker.attach(11);


}

//=====================================================================

void loop() {

  dauer.write(110);
   delay(2000);
   kleiner.write(110);
    delay(1000);
    kleiner.write(90);
    delay(500);
    kipper.write(100);
    delay(500);
    kipper.write(90);
    delay(1000);
    kipper.write(80);
    delay(500);
    kipper.write(90);
    delay(1000);
    kleiner.write(50);
    delay(1000);
    kleiner.write(90);


//dauer.write(100);

blocker.write(10);   // block an
delay(2000);
blocker.write(90);   // block aus
delay(2000);
blocker.write(10);


      
}
