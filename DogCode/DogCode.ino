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
    Leg(int hipYPin, int hipXPin, int kneePin, boolean onRightQuestionMark);
    
    Leg();
   
    void setYZ(double hipXAng, double kneeAng){
        hipX.writeMicroseconds(specialSauce(hipXAng));
        knee.writeMicroseconds(specialSauce(180+hipX.read()+ kneeAng));
    }

    void setHip(double ang){
      hipY.writeMicroseconds(specialSauce(ang));
    }
    
    static double specialSauce(double angle){
      double result = (angle/160.0)*(2400-550) + 550;
      return result;
    }
};

Leg::Leg(int hipYPin, int hipXPin, int kneePin, boolean onRightQuestionMark) {
      Servo *hipY = new Servo();
      hipY->attach(hipYPin);
      
      Servo *hipX = new Servo();
      hipX->attach(hipXPin);
      
      Servo *knee = new Servo();
      knee->attach(kneePin);

      right = onRightQuestionMark;
}

Leg::Leg(){
  right = false;
}

class Dog {
  private:
    Leg FL;
    Leg FR;
    Leg RR;
    Leg RL;

    //list<Leg>legList;
    Leg legs[4] = {Leg(9,7,4,false),
                   Leg(9,7,4,false),
                   Leg(10,12,5,true),
                   Leg(11,13,3,false)
                                    };    
  public:
    Dog();
    
    //setLegs(int FLx, int FLy, int FLz, int FRx, int FRy, int FRz,] int RRx, int RRy, int RRz, int RLx, int RLy, int RLz){
    void setLegsYZ(double hipXAng, double kneeAng){
      for (Leg temp : legs){
        temp.setYZ(hipXAng,kneeAng);
      }
    }

    void setHips(double ang){
      for (Leg temp : legs){
        temp.setHip(ang); //not sure if this works with the for loop and setting a variable to smthn
      }
    }
};

Dog::Dog() {
      //Leg *fl = new Leg(9,7,4,false);      
      //Leg *fr = new Leg(8,6,2,true);
      //Leg *rr = new Leg(10,12,5,true);
      //Leg *rl = new Leg(11,13,3,false);

      //legs[0] = fl;
      //legs[1] = fr;
      //legs[2] = rr;
      //legs[3] = rl;
}

Dog* skorupi = new Dog();

void setup() {
  skorupi->setHips(80.0);
  skorupi->setLegsYZ(340.8,220.2);
  delay(5000);
}

void loop() {
  skorupi->setLegsYZ(0.0,0.0);
}
