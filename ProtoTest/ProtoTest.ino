
#include <Servo.h>

Servo test;  // create servo object to control a servo
double zeroPos = 0.0;    // variable to store the servo position
double testSpeed = 0.0; //max is 600.0
int testPol = 1;


void setup() {
  test.attach(31);  // attaches the servo on pin 9 to the servo object
  zeroPos = test.read();
  testSpeed = 600;
}

void loop() {
  //test.write(0);
    test.writeMicroseconds(specialSauce(135.0)); //550 is 0, 2400 is max is 160
//  delay(random(100)*1000);
  delay(1000);
  test.write(0); //45 is 50, 90 is 100, 160 is 180(max), 10 is 10, 20 is 20, 30 is 30, 
    //test.writeMicroseconds(4000);
//  delay(random(100)*1000);
  delay(1000);
}

int specialSauce(double angle){
  int result = (angle/160.0)*(2400-550) + 550;
  return result;
}
