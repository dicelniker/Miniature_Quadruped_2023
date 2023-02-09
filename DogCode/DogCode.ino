/* 

*/

#include <Servo.h>
#include <math.h>


class Ovres : public Servo{  //extends servo to add an angle variable
  private:
    int ang;
    
  public:
    Ovres(){
      ang = 0;
      Servo();
    }
    
    void setAngle(int pos){
      ang = pos;
      Servo::write(ang);
    }
};

class Leg { //includes the three servos of the leg as well as the position of the foot, as well as wether or not the leg is on the right or the left
  private:
    double x;
    double y;
    double z;
    
    Ovres hipY;
    Ovres hipX;
    Ovres knee; 

    boolean right;

  public:
    Leg(){
     x = 0.0;
     y = 0.0;
     z = 0.0;

     Ovres *hipY = new Ovres();
     Ovres *hipX = new Ovres();
     Ovres *knee = new Ovres();

     boolean right = true;
    }
};

class pathFinder{ //includes the walking paths for the dog
  private:
 
  public:
    
};



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
