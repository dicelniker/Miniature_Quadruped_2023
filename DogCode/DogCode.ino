/*
  Dogcode
*/

#include <Servo.h>
#include <math.h>
//#include <list>


class Leg { //includes the three servos of the leg as well as wether or not the leg is on the right or the left

  private:

    Servo hipY; //IRL Range: (280,80)
    double hipYZero = 80.0; //Should be placed at IRL 0.0deg when zeroed
    double hipYIRLZero = 0.0;

    Servo hipX; //IRL Range: (160,320)
    double hipXZero = 110.0; //should be placed at IRL 270.0deg when zeroed
    double hipXIRLZero = 270.0;

    Servo knee; //IRL Range: (260,60) (based on knee frame of reference)
    double kneeZero = 10.0; //should be placed at IRL 270.0deg when zeroed
    double kneeIRLZero = 270.0;

    boolean right;

  public:
    Leg(int hipYPin, int hipXPin, int kneePin, boolean onRightQuestionMark);

    Leg();

    void setYZ(double hipXAng, double kneeAng) {

      kneeAng = kneeAng - (kneeIRLZero - kneeZero) + (270.0 - hipXAng); //convert to correct servo value
      hipXAng = hipXAng - (hipXIRLZero - hipXZero); //convert to correct servo value

      if (!right) { //account for leg polarity
        hipXAng = 160.0 - hipXAng;
        kneeAng = 160.0 - kneeAng;
      }
      
      hipX.writeMicroseconds(specialSauce(hipXAng));
      knee.writeMicroseconds(specialSauce(kneeAng));
    }

    void setHip(double ang) {
      if(!right) {
        ang = 160.0 - ang;
      }
      
      hipY.writeMicroseconds(specialSauce(ang));
    }

    void zero() {
      hipY.writeMicroseconds(specialSauce(80));
      hipX.writeMicroseconds(specialSauce(110));
      knee.writeMicroseconds(specialSauce(10));
     }

    static double specialSauce(double angle) {
      double result = (angle / 160.0) * (2400 - 550) + 550;
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

Leg::Leg() {
  right = false;
}

class Dog {
  private:

    //list<Leg>legList;
    Leg legs[4];
    
  public:
    Dog();

    //setLegs(int FLx, int FLy, int FLz, int FRx, int FRy, int FRz,] int RRx, int RRy, int RRz, int RLx, int RLy, int RLz){
  void setLegsYZ(double hipXAng, double kneeAng) {
    for (Leg& leg : legs) {
      leg.setYZ(hipXAng, kneeAng);
    }
  }

    void setHips(double ang) {
      for (Leg& leg : legs) {
        leg.setHip(ang); //not sure if this works with the for loop and setting a variable to smthn
      }
    }

    void zeroAll() {
      for (Leg& leg : legs) {
        leg.zero(); //not sure if this works with the for loop and setting a variable to smthn
      }
    }
};

Dog::Dog() :
legs{Leg(1, 2, 3, false),
     Leg(4, 5, 6, false),
     Leg(7, 8, 9, true),
     Leg(10, 11, 12, true)
     }
     {};


Dog* skorupi = new Dog();

int gaitOne[10][2] = {{199, 320},
                      {242, 346},
                      {287, 342},
                      {304, 322},
                      {273, 316},
                      {233, 299},
                      {218, 269},
                      {219, 237},
                      {189, 248},
                      {177, 279},
                                };
void setup() {

  skorupi->zeroAll();
  /*
  skorupi->setHips(80.0);
  skorupi->setLegsYZ(340.8, 220.2);
  delay(5000);
*/
}

void loop() {
  /*
  int lngth = 10;
  
  for (int i = 0; i < lngth; i++) {
    skorupi->setLegsYZ(gaitOne[i][0], gaitOne[i][1]);
    
    //delay for the servo with the most movement
    if (i != 0){
      delay(1.67 * max(abs(gaitOne[i][0]-gaitOne[i-1][0]),
                       abs(gaitOne[i][1]-gaitOne[i-1][1])));
    }
    else{
      delay(1.67 * max(abs(gaitOne[lngth-1][0]-gaitOne[0][0]),
                       abs(gaitOne[lngth-1][1]-gaitOne[0][1])));
    }
  }
  */
}
