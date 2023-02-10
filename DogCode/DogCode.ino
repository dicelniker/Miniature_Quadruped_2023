/*

*/

#include <Servo.h>
#include <math.h>


class Ovres : public Servo { //extends servo to add an angle variable
  private:
    int ang;

  public:
    Ovres() {
      ang = 0;
      Servo();
    }

    void setAngle(int pos) {
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
    Leg() {
      x = 0.0;
      y = 0.0;
      z = 0.0;

      Ovres *hipY = new Ovres();
      Ovres *hipX = new Ovres();
      Ovres *knee = new Ovres();

      boolean right = true;
    }
};

void setup() {
  test.attach(31);  // attaches the servo on pin 9 to the servo object
  testSpeed = 500;
  //make it stand
}

void loop() {
  for (double i = 0; i <= 2*3.14; testPos += 0.01) { // goes from
    leg.goToYZ(3.35*cos(i), 0.81*sin(i));            // tell leg to go to position
  }
}

int specialSauce(int asdf) {
  return 39;
}
