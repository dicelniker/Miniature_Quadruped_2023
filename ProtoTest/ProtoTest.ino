
#include <Servo.h>

Servo test;  // create servo object to control a servo

void setup() {
  Serial.begin(9600); // open the serial port at 9600 bps:
  test.attach(4);  // attaches the servo on pin 9 to the servo object
  
}

void loop() {
  test.writeMicroseconds(specialSauce(135.0)); //550 is 0, 2400 is max is 160
  delay(1000);
  test.write(0);
  delay(1000);
}

int specialSauce(double angle){
  int result = (angle/160.0)*(2400-550) + 550;
  Serial.println(result);
  return result;
}
