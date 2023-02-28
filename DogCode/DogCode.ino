/*

*/

#include <Servo.h>
#include <math.h>
#include <list>

class Ovres : public Servo { //extends servo to add an angle variable
  private:
    int ang;
    int pin;

  public:
    Ovres() {
      ang = 0;
      Servo();
    }

    void setAngle(int pos) {
      ang = pos;
      Servo.write(ang);
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
    Leg(int hipYPin, int hipXPin, int kneePin, boolean onRightQuestionMark) {
      x = 0.0;
      y = 0.0;
      z = 0.0;

      right = onRightQuestionMark;

      Ovres *hipY = new Ovres();
      hipY.attach(hipYPin);
      
      Ovres *hipX = new Ovres();
      hipX.attach(hipXPin);
      
      Ovres *knee = new Ovres();
      knee.attach(kneePin);

      boolean right = true;
    }

    setYZ(int y, int z){
      
    }
};

Class Dog {
  private:
    Leg FL;
    Leg FR;
    Leg RR;
    Leg RL;

    list<Leg>legList;
    
  public:
    Dog() {
      Leg *FL = new Leg(9,7,4,false);      
      Leg *FR = new Leg(8,6,2,true);
      Leg *RR = new Leg(10,12,5,true);
      Leg *RL = new Leg(11,13,3,false);

      legList.add(FL);
      legList.add(FR);
      legList.add(RR);
      legList.add(RL);
    }
    //setLegs(int FLx, int FLy, int FLz, int FRx, int FRy, int FRz, int RRx, int RRy, int RRz, int RLx, int RLy, int RLz){
    setLegsYZ(int y, int z){
      for (Leg temp : legList){
        temp.setYZ(y,z)
        }
      }
    }
};

void setup() {
  Dog skorupi = new Dog();
  
  
}

void loop() {
  for (double i = 0; i <= 2 * 3.14; testPos += 0.01) { // goes from
    leg.goToYZ(3.35 * cos(i), 0.81 * sin(i));        // tell leg to go to position
  }
}

int specialSauce(int asdf) {
  return 39;
}
