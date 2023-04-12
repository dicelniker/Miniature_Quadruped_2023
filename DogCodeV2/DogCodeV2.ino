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
      if (!right) {
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

  hipY.writeMicroseconds(specialSauce(hipYZero));
  hipX.writeMicroseconds(specialSauce(hipXZero));
  knee.writeMicroseconds(specialSauce(kneeZero));

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

    //Leg legs[4];

    Leg fl;
    Leg rl;
    Leg rr;
    Leg fr;

  public:
    Dog();
  
    Dog(Leg frontLeft, Leg rearLeft, Leg rearRight, Leg frontRight);

    void setLegsYZ(double hipXAng, double kneeAng) {
      /*
        for (Leg& leg : legs) {
        leg.setYZ(hipXAng, kneeAng);
        Serial.print("hipX to ");
        Serial.println(hipXAng);
        Serial.println();

        Serial.print("knee to ");
        Serial.println(kneeAng);
        Serial.println();
        }
      */
      fl.setYZ(hipXAng, kneeAng);
      rl.setYZ(hipXAng, kneeAng);
      rr.setYZ(hipXAng, kneeAng);
      fr.setYZ(hipXAng, kneeAng);
    }

    void setHips(double ang) {
      /*
        for (Leg& leg : legs) {
        leg.setHip(ang); //not sure if this works with the for loop and setting a variable to smthn
        Serial.print("hipY to ");
        Serial.println(ang);
        Serial.println();
        }
      */
      fl.setHip(ang);
      rl.setHip(ang);
      rr.setHip(ang);
      fr.setHip(ang);
    }

    void zeroAll() {
      /*
        for (Leg& leg : legs) {
        leg.zero(); //not sure if this works with the for loop and setting a variable to smthn
        Serial.println("zeroed all");
        Serial.println();
        }
      */
      fl.zero();
      rl.zero();
      rr.zero();
      fr.zero();
    }

};

int pinOffset = 24;

Dog::Dog() {
  
}

Dog::Dog(Leg frontLeft, Leg rearLeft, Leg rearRight, Leg frontRight) {
  fl = frontLeft;
  fl.zero();
  rl = rearLeft;
  rl.zero();
  rr = rearRight;
  rr.zero();
  fr = frontRight;
  fr.zero();

  //legs[0] = Fl;
  //legs[1] = Rl;
  //legs[2] = Rr;
  //legs[3] = Fr;
}


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
Dog skorupi;


void setup() {
  testLeg = Leg(22, 24, 26, false);
  test.attach(28);

  Leg frontLeft;
  frontLeft = Leg(2, 3, 4, false);
 
  Leg rearLeft;
  rearLeft = Leg(5, 6, 7, false);
 
  Leg rearRight;
  rearRight = Leg(8, 9, 10, true);
  
  Leg frontRight;
  frontRight = Leg(11, 12, 13, true);
  
  skorupi = Dog(frontLeft, rearLeft, rearRight, frontRight);

    //the problem could be that you are supposed to attatch the servos in the setup and not in the base code
    //ok its prolly cuz we're doing -> and * instead of Servo test; and test.akshjf();
    //could be either of these actually
    // so i got it to just set to default and not do random things at least that's good

    skorupi.zeroAll();

  test.write(160);
  testLeg.setHip(0);
  testLeg.setYZ(160, 260);
}

void loop() {
  test.write(0);
  testLeg.setHip(0);
  testLeg.setYZ(160, 260);

  skorupi.setHips(0);
  skorupi.setLegsYZ(160, 260);

  delay(1000);

  test.write(160);
  testLeg.setHip(160);
  testLeg.setYZ(320, 60);

  skorupi.setHips(160);
  skorupi.setLegsYZ(320, 60);

    delay(1000);

}
