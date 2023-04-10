/*
  Dogcode
*/

#include <Servo.h>
#include <math.h>
//#include <list>


class Leg { //includes the three servos of the leg 
            //as well as wether or not the leg is on the right or the left

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

    Leg(int hipYPin);
    
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
      hipY.writeMicroseconds(specialSauce(hipYZero));
      hipX.writeMicroseconds(specialSauce(hipXZero));
      knee.writeMicroseconds(specialSauce(kneeZero));
     }

    static int specialSauce(double angle) {
      int result = (angle / 160.0) * (2400 - 550) + 550;
      return result;
    }
};

Leg::Leg(int hipYPin, int hipXPin, int kneePin, boolean onRightQuestionMark) {
  hipY.attach(hipYPin);

  hipX.attach(hipXPin);

  knee.attach(kneePin);

  right = onRightQuestionMark;
}

Leg::Leg() {
  right = false;
}

Leg::Leg(int hipYPin) {
  Servo hipY;
  hipY.attach(hipYPin);
  right = false;
}

class Dog {
  private:

    Leg legs[4];
    
  public:
    Dog();

  void setLegsYZ(double hipXAng, double kneeAng) {
    for (Leg& leg : legs) {
      leg.setYZ(hipXAng, kneeAng);
      Serial.print("hipX to ");
      Serial.println(hipXAng);
      Serial.println();
      
      Serial.print("knee to ");
      Serial.println(kneeAng);
      Serial.println();
    }
  }

    void setHips(double ang) {
      for (Leg& leg : legs) {
        leg.setHip(ang); //not sure if this works with the for loop and setting a variable to smthn
        Serial.print("hipY to ");
        Serial.println(ang);
        Serial.println();
      }
    }

    void zeroAll() {
      for (Leg& leg : legs) {
        leg.zero(); //not sure if this works with the for loop and setting a variable to smthn
        Serial.println("zeroed all");
        Serial.println();
      }
    }

};

int pinOffset = 24;
Dog::Dog() :
legs{
     Leg(1, 2, 3, false),
     Leg(4, 5, 6, false),
     Leg(7, 8, 9, true),
     Leg(10, 11, 12, true)
     }
     {};

//Dog *skorupi = new Dog();


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

Servo test;

Leg testLeg; 

void setup() {
  testLeg = Leg(1,2,3,false);
  Serial.begin(9600); // open the serial port at 9600 bps:
  test.attach(7); 
  
  //the problem could be that you are supposed to attatch the servos in the setup and not in the base code
  //ok its prolly cuz we're doing -> and * instead of Servo test; and test.akshjf();
  //could be either of these actually
// so i got it to just set to default and not do random things at least that's good

  test.write(0);
  test.write(160);
  testLeg.setHip(0);
  testLeg.setYZ(160, 160);
  
  
}

void loop() {
  test.write(0);
  testLeg.setHip(0);
  testLeg.setYZ(0, 0);
  
  delay(1000);
  
  test.write(160);
  testLeg.setHip(160);
  testLeg.setYZ(160, 160);
  
  delay(1000);
}
