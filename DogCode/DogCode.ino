/*
Dogcode
*/

#include <Servo.h>
#include <math.h>
//#include <list>


class Leg { //includes the three servos of the leg as well as wether or not the leg is on the right or the left
  private:
  
    Servo hipY;
    Servo hipX;
    Servo knee;

    boolean right;

   public:
    Leg(int hipYPin, int hipXPin, int kneePin, boolean onRightQuestionMark) {
      
      right = onRightQuestionMark;

      Servo *hipY = new Servo();
      hipY.attach(hipYPin);
      
      Servo *hipX = new Servo();
      hipX.attach(hipXPin);
      
      Servo *knee = new Servo();
      knee.attach(kneePin);

      boolean right = true;
    }

    setYZ(int y, int z){
      
    }
};

class Dog {
  private:
    Leg FL;
    Leg FR;
    Leg RR;
    Leg RL;

    //list<Leg>legList;
    Leg[] legs;
    
  public:
    Dog() {
      Leg *FL = new Leg(9,7,4,false);      
      Leg *FR = new Leg(8,6,2,true);
      Leg *RR = new Leg(10,12,5,true);
      Leg *RL = new Leg(11,13,3,false);

      Leg[] legs = new Leg[4];
      legs[0] = FL;
      legs[1] = FR;
      legs[2] = RR;
      legs[3] = RL;
      //legList.add(FL);
      //legList.add(FR);
      //legList.add(RR);
      //legList.add(RL);
      
    }
    //setLegs(int FLx, int FLy, int FLz, int FRx, int FRy, int FRz,] int RRx, int RRy, int RRz, int RLx, int RLy, int RLz){
    setLegsYZ(int hipXAng, int kneeAng){
      for (Leg temp : legs){
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
