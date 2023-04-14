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
    double hipYAngle; //current angle of servo

    Servo hipX; //IRL Range: (160,320)
    double hipXZero = 110.0; //should be placed at IRL 270.0deg when zeroed
    double hipXIRLZero = 270.0;
    double hipXAngle; //current angle of servo

    Servo knee; //IRL Range: (260,60) (based on knee frame of reference)
    double kneeZero = 150.0; //should be placed at IRL 270.0deg when zeroed
    double kneeIRLZero = 90.0; //note: this servo has reversed polarity
    double kneeAngle; //current angle of servo

    boolean right;

  public:
    Leg(int hipYPin, int hipXPin, int kneePin, boolean onRightQuestionMark);

    Leg(int hipYPin);

    Leg();


    void setYZ(double hipXAng, double kneeAng, double spd) {

      double prevHipXAng = hipXAngle + (hipXIRLZero - hipXZero);
      double prevKneeAng = (360 - kneeAngle) - (kneeIRLZero - kneeZero) - (270.0 - hipXAngle);

       if (!right) { //account for leg polarity
        prevHipXAng = 160.0 - prevHipXAng;
        prevKneeAng = 160.0 - prevKneeAng;
      }
      
      double prevY = 2.165*cos(prevHipXAng*3.14/180)+2.675*cos(prevKneeAng*3.14/180);
      double prevZ = 2.165*sin(prevHipXAng*3.14/180)+2.675*sin(prevKneeAng*3.14/180); 

      double currY = 2.165*cos(hipXAng*3.14/180)+2.675*cos(kneeAng*3.14/180);
      double currZ = 2.165*sin(hipXAng*3.14/180)+2.675*sin(kneeAng*3.14/180);

      double distance = sqrt(sq(currY - prevY) + sq(currZ - prevZ));
      Serial.print("distance to next point: ");
      Serial.print(distance);
      Serial.println(" inches");
//1.6, 1.6, .56, 1.4, 2.0, 1.8, 1.5, .79, 1.6, 1.8

      double t = (double) round(100*(distance/spd))/100;
      Serial.print("time: ");
      Serial.print(t);
      Serial.println(" seconds");

      kneeAng = (360 - kneeAng) - (kneeIRLZero - kneeZero) - (270.0 - hipXAng); //convert to correct servo value
      hipXAng = hipXAng - (hipXIRLZero - hipXZero); //convert to correct servo value

      if (!right) { //account for leg polarity
        hipXAng = 160.0 - hipXAng;
        kneeAng = 160.0 - kneeAng;
      }

      int steps = t/0.01;
      double hipXStep = (hipXAng - hipXAngle)/steps;
      double kneeStep = (kneeAng - kneeAngle)/steps;

      if ((hipXStep > 6) || (kneeStep > 6)){
        Serial.println("woah there buckaroo");
        delay(10000);
      }
      
      for (int i = steps; i>0; i = i-0.1){
         hipXAngle += hipXStep;
         kneeAngle += kneeStep;
         hipX.writeMicroseconds(specialSauce(hipXAngle));
         knee.writeMicroseconds(specialSauce(kneeAngle));
         
         //Serial.println("delay 10");
         delay(10);
         
      }
      hipX.writeMicroseconds(specialSauce(hipXAng));
      knee.writeMicroseconds(specialSauce(kneeAng));
    
      hipXAngle = hipXAng;
      kneeAngle = kneeAng;
    }

    void setHip(double ang) {
      if (!right) {
        ang = 160.0 - ang;
      }

      hipY.writeMicroseconds(specialSauce(ang));
      hipYAngle = ang;
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

    void set(double hipYAng, double hipXAng, double kneeAng, double spd){
      setHip(hipYAng);
      setYZ(hipXAng, kneeAng, spd);
    }
};

Leg::Leg(int hipYPin, int hipXPin, int kneePin, boolean onRightQuestionMark) {
  hipY.attach(hipYPin);
  hipYAngle = hipYZero;

  hipX.attach(hipXPin);
  hipXAngle = hipXZero;

  knee.attach(kneePin);
  kneeAngle = kneeZero;

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

    void setLegsYZ(double hipXAng, double kneeAng, double spd) {
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
      fl.setYZ(hipXAng, kneeAng, spd);
      rl.setYZ(hipXAng, kneeAng, spd);
      rr.setYZ(hipXAng, kneeAng, spd);
      fr.setYZ(hipXAng, kneeAng, spd);

      Serial.print("YZ set to: ");
      Serial.print(hipXAng);    
      Serial.print(", ");
      Serial.println(kneeAng);
      Serial.println();
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

      Serial.print("Hips set to: ");
      Serial.println(ang);
      Serial.println();
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

      Serial.println("Zeroed");    
      Serial.println();
    }
    //String to_String(){
      //Serial.println("This is a dog fam");      
    //}

    void setLegs(double hipYAng, double hipXAng, double kneeAng, double spd) {
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
      fl.set(hipYAng, hipXAng, kneeAng, spd);
      rl.set(hipYAng, hipXAng, kneeAng, spd);
      rr.set(hipYAng, hipXAng, kneeAng, spd);
      fr.set(hipYAng, hipXAng, kneeAng, spd);

      Serial.print("leg set to: ");
      Serial.print(hipYAng);    
      Serial.print(", ");
      Serial.print(hipXAng);
      Serial.print(", ");
      Serial.println(kneeAng);
      Serial.println();
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


Dog skorupi;
double footSpeed;


void setup() {
  Serial.begin(9600);

  footSpeed = 4.0; //footspeed in inches/s - may be limited by servo max speed

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
  delay(5000);
}

void loop() {

  //skorupi.zeroAll();

  int lngth = 10;
  
  for (int i = 0; i < lngth; i++) {
    
    skorupi.setLegs(80, gaitOne[i][0], gaitOne[i][1], footSpeed);
 /*   
    //delay for the servo with the most movement
    if (i != 0){
      delay(10.67 * max(abs(gaitOne[i][0]-gaitOne[i-1][0]),
                       abs(gaitOne[i][1]-gaitOne[i-1][1])));
    }
    else{
      delay(10.67 * max(abs(gaitOne[lngth-1][0]-gaitOne[0][0]),
                       abs(gaitOne[lngth-1][1]-gaitOne[0][1])));
     }
   */
}


}
