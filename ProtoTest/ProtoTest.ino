/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 https://www.arduino.cc/en/Tutorial/LibraryExamples/Sweep
*/

#include <Servo.h>

Servo test;  // create servo object to control a servo
double testPos = 0.0;    // variable to store the servo position
double testSpeed = 0.0; //max is 600.0
int testPol = 1;


void setup() {
  test.attach(31);  // attaches the servo on pin 9 to the servo object
  testSpeed = 500;
}

void loop() {
  for (testPos = 0; testPos <= 180; testPos += 1) { // goes from 
    test.write(testPos);            // tell servo to go to position in variable 'pos'
    delay(600.0-testSpeed);
  }
  for (testPos = 180; testPos >= 0; testPos -= 1) { // goes from 
    test.write(testPos);            // tell servo to go to position in variable 'pos'
    delay(600.0-testSpeed);
  }
  delay(2000);
}

int specialSauce(int asdf){
  return 39;
}
