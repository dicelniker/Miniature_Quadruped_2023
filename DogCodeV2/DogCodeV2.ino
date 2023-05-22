/*
  Dogcode
*/

#include <Servo.h>
#include <math.h>
//#include <list>


class Leg { //includes the three servos of the leg
    //as well as wether or not the leg is on the right or the left

  private:
    String nombre;

    Servo hipY; //IRL Range: (280,80)
    double hipYZero; //Should be placed at IRL 0.0deg when zeroed
    double hipYIRLZero = 0.0;
    double hipYAngle; //current IRL angle of limb

    Servo hipX; //IRL Range: (160,320)
    double hipXZero; //should be placed at IRL 270.0deg when zeroed
    double hipXIRLZero = 270.0;
    double hipXAngle; //current IRL angle of limb

    Servo knee; //IRL Range: (260,60) (based on knee frame of reference)
    double kneeZero; //should be placed at IRL 270.0deg when zeroed
    double kneeIRLZero = 90.0; //note: this servo has reversed polarity
    double kneeAngle; //current IRL angle of limb

    boolean right;

    int path[10][2];
    int pathIndex;
    int steps;
    int currStep;
    double footSpeed;
    double hipXStep;
    double kneeStep;

  public:
    Leg(String nombre, int hipYPin, double hipYZ, int hipXPin, double hipXZ, int kneePin, double kneeZ, boolean onRightQuestionMark);

    Leg(int hipYPin);

    Leg();

    double getHipYIRLZero(){
      return hipYIRLZero;
    }

    double getHipXIRLZero(){
      return hipXIRLZero;
    }

    double getKneeIRLZero(){
      return kneeIRLZero;
    }

    double getHipYAngle() {
      return hipYAngle;
    }

    double getHipXAngle() {
      return hipXAngle;
    }

    double getKneeAngle() {
      return kneeAngle;
    }

    double getHipXStep() {
      return hipXStep;
    }

    double getKneeStep() {
      return kneeStep;
    }

    void setSteps(int stps) {
      steps = stps;
    }

    void setCurrStep(int stp) {
      currStep = stp;
    }

    void setHipXStep(double stp) {
      hipXStep = stp;
    }

    void setKneeStep(double stp) {
      kneeStep = stp;
    }

    void setHipX(double hipXAng) {
      hipXAngle = hipXAng;

      //Serial.print(nombre); Serial.print("'s hipX to "); Serial.println(hipXAng);

      if (right) { //account for leg polarity
        hipXAng = hipXAng - (hipXIRLZero - hipXZero); //convert to correct servo value
      }

      else {
        hipXAng = (360 - hipXAng) - ((360 - hipXIRLZero) - hipXZero); //convert to correct servo value
      }

      hipX.writeMicroseconds(specialSauce(hipXAng));
    }

    void setKnee(double kneeAng) {

      //Serial.print(nombre); Serial.print("'s knee to "); Serial.println(kneeAng);

      kneeAngle = kneeAng;

      if (right) { //account for leg polarity
        kneeAng = (360 - kneeAng) - (kneeIRLZero - kneeZero) - (270.0 - hipXAngle); //convert to correct servo value
      }

      else {
        kneeAng = kneeAng - ((360 - kneeIRLZero) - kneeZero) + (270.0 - hipXAngle); //convert to correct servo value
      }

      knee.writeMicroseconds(specialSauce(kneeAng));
    }

    void setYZ(double hipXAng, double kneeAng) {
      hipXAngle = hipXAng;
      kneeAngle = kneeAng;

      //Serial.println(); Serial.print("HipX to IRL: "); Serial.println(hipXAng); Serial.print("Knee to IRL: "); Serial.println(kneeAng);

      if (right) { //account for leg polarity
        kneeAng = (360 - kneeAng) - (kneeIRLZero - kneeZero) - (270.0 - hipXAng); //convert to correct servo value
        hipXAng = hipXAng - (hipXIRLZero - hipXZero); //convert to correct servo value
      }

      else {
        kneeAng = kneeAng - ((360 - kneeIRLZero) - kneeZero) + (270.0 - hipXAng); //convert to correct servo value
        hipXAng = (360 - hipXAng) - ((360 - hipXIRLZero) - hipXZero); //convert to correct servo value
      }

      hipX.writeMicroseconds(specialSauce(hipXAng));
      knee.writeMicroseconds(specialSauce(kneeAng));

      //Serial.println(); Serial.println(nombre); Serial.print("HipX to: "); Serial.print(hipXAng); Serial.print(", Knee to: "); Serial.println(kneeAng);

    }

    void setHip(double ang) {

      //checks if value is too high or too low and could cause internal collisions
      //if (((ang > 28) && (ang < 332)) || (ang > 360) || (ang < 0)) {
      if (0 != 0) {
        Serial.println("woah there bucko that's too much... or too little");
        delay(100000);
      }

      else {
        hipYAngle = ang;

        if (right) {
          ang = ang - (hipYIRLZero - hipYZero);
          if (ang > 360) {
            ang -= 360;
          }
        }

        else {
          ang = ang - ((180 - hipYIRLZero) - hipYZero);
        }

        hipY.writeMicroseconds(specialSauce(ang));

        //Serial.println(); Serial.println(nombre); Serial.print("HipY to: "); Serial.print(ang); Serial.print(", ");
      }

    }

    void zero() {
      hipY.writeMicroseconds(specialSauce(hipYZero));
      hipX.writeMicroseconds(specialSauce(hipXZero));
      knee.writeMicroseconds(specialSauce(kneeZero));

      hipYAngle = hipYIRLZero;
      hipXAngle = hipXIRLZero;
      kneeAngle = kneeIRLZero;
    }

    static double specialSauce(double angle) {
      double result = (angle / 160.0) * (2400 - 550) + 550;
      return result;
    }

    void set(double hipYAng, double hipXAng, double kneeAng) {
      setHip(hipYAng);
      setYZ(hipXAng, kneeAng);
    }

    void setPath(int pth[10][2], double spd) {
      //Serial.println("Path set to:");
      for (int i = 0; i < 10; i++) {
        path[i][0] = pth[i][0];
        path[i][1] = pth[i][1];
        //Serial.print(path[i][0]); Serial.print(", "); Serial.println(path[i][1]); Serial.println();
      }
      footSpeed = spd;
      currStep = 0;
      steps = 0;
      pathIndex = 0;
    }

    void updateAngs() {
      if (steps == currStep) {
        if (pathIndex != 9) {
          pathIndex++;
        }
        else {
          pathIndex = 0;
        }

        if (path[pathIndex][0] < 0) {
          steps = -1 * (path[pathIndex][0] / 10);
          hipXStep = 0;
          kneeStep = 0;
          //Serial.println(steps);
        }

        else {
          //Serial.print("pathIndex: "); Serial.println(pathIndex); Serial.println(); Serial.println(path[pathIndex][0]); Serial.println(path[pathIndex][1]); Serial.println();

          double distance = dist(hipXAngle, kneeAngle, path[pathIndex][0], path[pathIndex][1]);

          if (distance < 0.1) {
            hipXStep = 0;
            kneeStep = 0;
            steps = 1;
          }
          else {
            //Serial.println(); Serial.println(path[pathIndex][0]); Serial.println(path[pathIndex][1]); Serial.println();

            double t = (double) round(100 * (distance / footSpeed)) / 100;

            //Serial.print("time: "); Serial.print(t); Serial.println(" seconds");

            steps = t / 0.01;
            //Serial.print("steps: "); Serial.println(steps);
            hipXStep = (path[pathIndex][0] - hipXAngle) / steps;

            //Serial.print("hipXStep: ");Serial.print(hipXStep); Serial.println(" deg");

            kneeStep = (path[pathIndex][1] - kneeAngle) / steps;

            //Serial.print("kneeStep: "); Serial.print(kneeStep); Serial.println(" deg");

            if ((abs(hipXStep) > 6) || (abs(kneeStep) > 6)) {
              //Serial.println("woah there buckaroo");
              delay(100000);
            }
          }
        }
        currStep = 0;
        //Serial.print("currStep: "); Serial.println(0);
      }

      setYZ(hipXAngle + hipXStep, kneeAngle + kneeStep);

      currStep++;
      //Serial.print("currStep: "); Serial.println(currStep);

    }

    // finds the distance in the YZ plane between 2 foot positions based on the IRLangles of the
    //hipX and knee servos before and after the movement
    static double dist(double prevHipX, double prevKnee, double newHipX, double newKnee) {

      double prevY = 2.165 * cos(prevHipX * 3.14 / 180) + 2.675 * cos(prevKnee * 3.14 / 180);
      double prevZ = 2.165 * sin(prevHipX * 3.14 / 180) + 2.675 * sin(prevKnee * 3.14 / 180);

      double currY = 2.165 * cos(newHipX * 3.14 / 180) + 2.675 * cos(newKnee * 3.14 / 180);
      double currZ = 2.165 * sin(newHipX * 3.14 / 180) + 2.675 * sin(newKnee * 3.14 / 180);

      double distance = sqrt(sq(currY - prevY) + sq(currZ - prevZ));

      return distance;
    }
};










//leg constructors
Leg::Leg(String nm, int hipYPin, double hipYZ, int hipXPin, double hipXZ, int kneePin, double kneeZ, boolean onRightQuestionMark) {
  nombre = nm;

  hipY.attach(hipYPin);
  hipYZero = hipYZ;
  hipYAngle = hipYZero;

  hipX.attach(hipXPin);
  hipXZero = hipXZ;
  hipXAngle = hipXZero;

  knee.attach(kneePin);
  kneeZero = kneeZ;
  kneeAngle = kneeZero;

  right = onRightQuestionMark;

  pathIndex = 0;
  steps = 0;
  currStep = 0;
  footSpeed = 0.0;

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
    Leg fl;
    Leg rl;
    Leg rr;
    Leg fr;

  public:
    Dog();

    Dog(Leg frontLeft, Leg rearLeft, Leg rearRight, Leg frontRight);

    void setLegsYZ(double hipXAng, double kneeAng) {
      fl.setYZ(hipXAng, kneeAng);
      rl.setYZ(hipXAng, kneeAng);
      rr.setYZ(hipXAng, kneeAng);
      fr.setYZ(hipXAng, kneeAng);

      //Serial.print("YZ set to: "); Serial.print(hipXAng); Serial.print(", "); Serial.println(kneeAng); Serial.println();

    }

    void setHips(double ang) {
      rr.setHip(ang);
      fr.setHip(ang);

      if (ang < 180) {
        ang = 180 - ang;
      }

      else {
        ang = 540 - ang;
      }

      fl.setHip(ang);
      rl.setHip(ang);

      //Serial.print("Hips set to: "); Serial.println(ang); Serial.println();
    }

    void setHips(double flAng, double rlAng, double rrAng, double frAng) {
      fl.setHip(flAng);
      rl.setHip(rlAng);
      rr.setHip(rrAng);
      fr.setHip(frAng);
    }

    void zeroAll() {

      fl.zero();
      rl.zero();
      rr.zero();
      fr.zero();

      //Serial.println("Zeroed"); Serial.println();
    }

    void setLegs(double hipYAng, double hipXAng, double kneeAng) {

      setHips(hipYAng);
      setLegsYZ(hipXAng, kneeAng);

      //Serial.print("leg set to: "); Serial.print(hipYAng); Serial.print(", "); Serial.print(hipXAng); Serial.print(", "); Serial.println(kneeAng); Serial.println();
    }

    void loadZero(double footSpeed) {
      /*fl.setHip(180-fl.getHipYIRLZero());
      rl.setHip(180-rl.getHipYIRLZero());
      rr.setHip(rr.getHipYIRLZero());
      fr.setHip(fr.getHipYIRLZero());*/

      zeroAll();

      int paths[4][10][2] = {
        //FL
        {
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
        },
        //RL
        {
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
        },
        //RR
        {
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
        },
        //FR
        {
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
          {-100.0, 0},
        }
/*        
        //FL
        {
          {fl.getHipXIRLZero(), fl.getKneeIRLZero()},
          {fl.getHipXIRLZero(), fl.getKneeIRLZero()},
          {fl.getHipXIRLZero(), fl.getKneeIRLZero()},
          {fl.getHipXIRLZero(), fl.getKneeIRLZero()},
          {fl.getHipXIRLZero(), fl.getKneeIRLZero()},
          {fl.getHipXIRLZero(), fl.getKneeIRLZero()},
          {fl.getHipXIRLZero(), fl.getKneeIRLZero()},
          {fl.getHipXIRLZero(), fl.getKneeIRLZero()},
          {fl.getHipXIRLZero(), fl.getKneeIRLZero()},
          {fl.getHipXIRLZero(), fl.getKneeIRLZero()},
        },
        //RL
        {
          {rl.getHipXIRLZero(), rl.getKneeIRLZero()},
          {rl.getHipXIRLZero(), rl.getKneeIRLZero()},
          {rl.getHipXIRLZero(), rl.getKneeIRLZero()},
          {rl.getHipXIRLZero(), rl.getKneeIRLZero()},
          {rl.getHipXIRLZero(), rl.getKneeIRLZero()},
          {rl.getHipXIRLZero(), rl.getKneeIRLZero()},
          {rl.getHipXIRLZero(), rl.getKneeIRLZero()},
          {rl.getHipXIRLZero(), rl.getKneeIRLZero()},
          {rl.getHipXIRLZero(), rl.getKneeIRLZero()},
          {rl.getHipXIRLZero(), rl.getKneeIRLZero()},
        },
        //RR
        {
          {rr.getHipXIRLZero(), rr.getKneeIRLZero()},
          {rr.getHipXIRLZero(), rr.getKneeIRLZero()},
          {rr.getHipXIRLZero(), rr.getKneeIRLZero()},
          {rr.getHipXIRLZero(), rr.getKneeIRLZero()},
          {rr.getHipXIRLZero(), rr.getKneeIRLZero()},
          {rr.getHipXIRLZero(), rr.getKneeIRLZero()},
          {rr.getHipXIRLZero(), rr.getKneeIRLZero()},
          {rr.getHipXIRLZero(), rr.getKneeIRLZero()},
          {rr.getHipXIRLZero(), rr.getKneeIRLZero()},
          {rr.getHipXIRLZero(), rr.getKneeIRLZero()},
        },
        //FR
        {
          {fr.getHipXIRLZero(), fr.getKneeIRLZero()},
          {fr.getHipXIRLZero(), fr.getKneeIRLZero()},
          {fr.getHipXIRLZero(), fr.getKneeIRLZero()},
          {fr.getHipXIRLZero(), fr.getKneeIRLZero()},
          {fr.getHipXIRLZero(), fr.getKneeIRLZero()},
          {fr.getHipXIRLZero(), fr.getKneeIRLZero()},
          {fr.getHipXIRLZero(), fr.getKneeIRLZero()},
          {fr.getHipXIRLZero(), fr.getKneeIRLZero()},
          {fr.getHipXIRLZero(), fr.getKneeIRLZero()},
          {fr.getHipXIRLZero(), fr.getKneeIRLZero()},
        }
*/
      };

      setPaths(paths, footSpeed);
      //Serial.println("Zero loaded");
    }


    void loadSit(double footSpeed) {
      fl.setHip(164);
      rl.setHip(164);
      rr.setHip(16);
      fr.setHip(16);

      int paths[4][10][2] = {
        //FL
        {
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
        },
        //RL
        {
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
        },
        //RR
        {
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
        },
        //FR
        {
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
          {215, 320},
        }
      };

      setPaths(paths, footSpeed);
      //Serial.println("Sit loaded");
    }

    void loadStand(double footSpeed) {
      setHips(0);

      int paths[4][10][2] = {
        //FL
        {
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
        },
        //RL
        {
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
        },
        //RR
        {
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
        },
        //FR
        {
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
          {233, 299},
        }
      };

      setPaths(paths, footSpeed);
      //Serial.println("Stand loaded");
    }

    void loadGaitOne(double footSpeed) {

      int paths[4][10][2] = {
        //FL
        {
          {199, 320},
          {242, 346},
          {287, 342},
          {304, 322},
          {273, 316},
          {233, 299},
          {218, 269},
          {219, 237},
          {189, 248},
          {177, 279},
        },
        //RL
        {
          {233, 299},
          {218, 269},
          {219, 237},
          {189, 248},
          {177, 279},
          {199, 320},
          {242, 346},
          {287, 342},
          {304, 322},
          {273, 316},
        },
        //RR
        {
          {199, 320},
          {242, 346},
          {287, 342},
          {304, 322},
          {273, 316},
          {233, 299},
          {218, 269},
          {219, 237},
          {189, 248},
          {177, 279},
        },
        //FR
        {
          {233, 299},
          {218, 269},
          {219, 237},
          {189, 248},
          {177, 279},
          {199, 320},
          {242, 346},
          {287, 342},
          {304, 322},
          {273, 316},
        }
      };

      setPaths(paths, footSpeed);

      stand();
      delay(2000);
      //Serial.println("gaitOne loaded");
    }



    void loadGaitTwo(double footSpeed) {

      int paths[4][10][2] = {
        //FL
        {
          {259, 279},
          {258, 262},
          {235, 259},
          {211, 260},
          {191, 278},
          {192, 298},
          {224, 332},
          {274, 323},
          {278, 302},
          {277, 282},
        },
        //RL
        {
          {258, 262},
          {235, 259},
          {211, 260},
          {191, 278},
          {192, 298},
          {224, 332},
          {274, 323},
          {278, 302},
          {277, 282},
          {259, 279},
        },
        //RR
        {
          {235, 259},
          {211, 260},
          {191, 278},
          {192, 298},
          {224, 332},
          {274, 323},
          {278, 302},
          {277, 282},
          {259, 279},
          {258, 262},
        },
        //FR
        {
          {211, 260},
          {191, 278},
          {192, 298},
          {224, 332},
          {274, 323},
          {278, 302},
          {277, 282},
          {259, 279},
          {258, 262},
          {235, 259},
        }
      };

      setPaths(paths, footSpeed);

      setLegs(0, 259, 279);
      delay(2000);

      fl.setYZ(paths[0][0][0], paths[0][0][1]);
      rl.setYZ(paths[1][0][0], paths[1][0][1]);
      rr.setYZ(paths[2][0][0], paths[2][0][1]);
      fr.setYZ(paths[3][0][0], paths[3][0][1]);
      delay(1000 * (2.481 / footSpeed));
      //Serial.println("gaitTwo loaded");
    }

    void loadGaitThree(double footSpeed) {

      int paths[4][10][2] = {
        //FL
        {
          {216, 270},
          {214, 290},
          {239, 316},
          {292, 303},
          {289, 293},
          {277, 282},
          {259, 279},
          {258, 262},
          {247, 251},
          {235, 247},
        },
        //RL
        {
          {247, 251},
          {235, 247},
          {216, 270},
          {214, 290},
          {239, 316},
          {292, 303},
          {289, 293},
          {277, 282},
          {259, 279},
          {258, 262},
        },
        //RR
        {
          {239, 316},
          {292, 303},
          {289, 293},
          {277, 282},
          {259, 279},
          {258, 262},
          {247, 251},
          {235, 247},
          {216, 270},
          {214, 290},
        },
        //FR
        {
          {259, 279},
          {258, 262},
          {247, 251},
          {235, 247},
          {216, 270},
          {214, 290},
          {239, 316},
          {292, 303},
          {289, 293},
          {277, 282},
        }
      };

      setPaths(paths, footSpeed);

      setLegs(0, 259, 279);
      delay(2000);

      fl.setYZ(paths[0][0][0], paths[0][0][1]);
      rl.setYZ(paths[1][0][0], paths[1][0][1]);
      rr.setYZ(paths[2][0][0], paths[2][0][1]);
      fr.setYZ(paths[3][0][0], paths[3][0][1]);
      delay(1000 * (2.335 / footSpeed));
      //Serial.println("gaitThree loaded");
    }

    void loadGaitFour(double footSpeed) {

      int paths[4][10][2] = {
        //FL
        {
          {214, 290},
          {239, 316},
          {239, 316},
          {239, 316},
          {277, 282},
          {259, 279},
          {258, 262},
          {258, 262},
          {258, 262},
          {258, 262},
        },
        //RL
        {
          {239, 316},
          {239, 316},
          {239, 316},
          {277, 282},
          {259, 279},
          {258, 262},
          {258, 262},
          {258, 262},
          {258, 262},
          {214, 290},
        },
        //RR
        {
          {258, 262},
          {258, 262},
          {258, 262},
          {258, 262},
          {214, 290},
          {239, 316},
          {239, 316},
          {239, 316},
          {277, 282},
          {259, 279},
        },
        //FR
        {
          {277, 282},
          {259, 279},
          {258, 262},
          {258, 262},
          {258, 262},
          {258, 262},
          {214, 290},
          {239, 316},
          {239, 316},
          {239, 316},
        }
      };

      setPaths(paths, footSpeed);

      setLegs(0, 259, 279);
      delay(2000);

      fl.setYZ(paths[0][0][0], paths[0][0][1]);
      rl.setYZ(paths[1][0][0], paths[1][0][1]);
      rr.setYZ(paths[2][0][0], paths[2][0][1]);
      fr.setYZ(paths[3][0][0], paths[3][0][1]);
      delay(1000 * (1.336 / footSpeed));
      //Serial.println("gaitFour loaded");
    }

    void loadGaitFive(double footSpeed) {

      int paths[4][10][2] = {
        //FL
        {
          {235, 259},
          { -1000 * (8.5 / footSpeed), 0},
          {191, 278},
          {192, 298},
          {224, 332},
          {277, 282},
          { -1000 * (5.9 / footSpeed), 0},
          {259, 279},
          //{258, 262}, thud idy jsyus
          {235, 259},
          { -1000 * (5.9 / footSpeed), 0},
        },
        //RL
        {
          {235, 259},
          {191, 278},
          {192, 298},
          {224, 332},
          {224, 332},
          {277, 282},
          {259, 279},
          {258, 262},
          {235, 259},
          { -1000 * (20.3 / footSpeed), 0},
        },
        //RR
        {
          {235, 259},
          { -1000 * (14.4 / footSpeed), 0},
          {191, 278},
          {192, 298},
          {224, 332},
          {277, 282},
          {259, 279},
          {258, 262},
          {235, 259},
          { -1000 * (5.9 / footSpeed), 0},
        },
        //FR
        {
          {277, 282},
          { -1000 * (5.9 / footSpeed), 0},
          {277, 282},
          {259, 279},
          {258, 262},
          {235, 259},
          { -1000 * (14.4 / footSpeed), 0},
          {191, 278},
          {192, 298},
          {224, 332},
        }
      };

      setPaths(paths, footSpeed);

      setLegs(0, 259, 279);
      delay(2000);

      fl.setYZ(paths[0][0][0], paths[0][0][1]);
      rl.setYZ(paths[1][0][0], paths[1][0][1]);
      rr.setYZ(paths[2][0][0], paths[2][0][1]);
      fr.setYZ(paths[3][0][0], paths[3][0][1]);
      delay(1000 * (2.481 / footSpeed));
      //Serial.println("gaitFive loaded");
    }


    void loadGaitSix(double footSpeed) {

      int paths[4][10][2] = {
        //FL
        {
          {238, 295},
          {238, 295},
          { -1000 * (8.1 / footSpeed), 0},
          {215, 326},
          {288, 340},
          {301, 303},
          { -1000 * (5.6 / footSpeed), 0},
          {262, 308},
          {238, 295},
          { -1000 * (5.6 / footSpeed), 0},
        },
        //RL
        {
          {238, 295},
          {238, 295},
          {238, 295},
          {238, 295},
          {215, 326},
          {288, 340},
          {301, 303},
          {262, 308},
          {238, 295},
          { -1000 * (19.3 / footSpeed), 0},
        },
        //RR
        {
          {238, 295},
          {238, 295},
          {238, 295},
          { -1000 * (13.7 / footSpeed), 0},
          {215, 326},
          {288, 340},
          {301, 303},
          {262, 308},
          {238, 295},
          { -1000 * (5.6 / footSpeed), 0},
        },
        //FR
        {
          {301, 303},
          { -1000 * (5.6 / footSpeed), 0},
          {262, 308},
          {238, 295},
          {238, 295},
          {238, 295},
          {238, 295},
          { -1000 * (13.7 / footSpeed), 0},
          {215, 326},
          {288, 340},
        }
      };

      setPaths(paths, footSpeed);

      setLegs(0, 238, 295);
      delay(2000);

      fl.setYZ(paths[0][0][0], paths[0][0][1]);
      rl.setYZ(paths[1][0][0], paths[1][0][1]);
      rr.setYZ(paths[2][0][0], paths[2][0][1]);
      fr.setYZ(paths[3][0][0], paths[3][0][1]);
      delay(1000 * (2.56 / 8));
      //Serial.println("gaitSix loaded");
    }


    void loadGaitSeven(double footSpeed) {
      int paths[4][10][2] = {
        //FL
        {
          {214, 303},
          { -1000 * (7.9 / footSpeed), 0},
          {203, 308},
          {326, 292},
          { -1000 * (3.9 / footSpeed), 0},
          {298, 305},
          {299, 272},
          {236, 301},
          {214, 303},
          { -1000 * (3.9 / footSpeed), 0},
        },
        //RL
        {
          {214, 303},
          {214, 303},
          {214, 303},
          {203, 308},
          {326, 292},
          {298, 305},
          {299, 272},
          {236, 301},
          {214, 303},
          { -1000 * (15.7 / footSpeed), 0},
        },
        //RR
        {
          {214, 303},
          {214, 303},
          { -1000 * (11.8 / footSpeed), 0},
          {203, 308},
          {326, 292},
          {298, 305},
          {299, 272},
          {236, 301},
          {214, 303},
          { -1000 * (3.9 / footSpeed), 0},
        },
        //FR
        {
          {326, 292},
          {326, 292},
          {326, 292},
          { -1000 * (3.9 / footSpeed), 0},
          {298, 305},
          {299, 272},
          {236, 301},
          {214, 303},
          { -1000 * (11.8 / footSpeed), 0},
          {203, 308},
        }
      };
      
      setPaths(paths, footSpeed);
      
      setLegs(0, 209, 305);
      delay(2000);

      fl.setYZ(paths[0][0][0], paths[0][0][1]);
      rl.setYZ(paths[1][0][0], paths[1][0][1]);
      rr.setYZ(paths[2][0][0], paths[2][0][1]);
      fr.setYZ(paths[3][0][0], paths[3][0][1]);
      delay(1000 * (3.57 / 8));
      //Serial.println("gaitSeven loaded");
    }


    void loadGaitEight(double footSpeed) {
      int paths[4][10][2] = {
        //FL
        {
          {213, 304},
          { -1000 * (6.9 / footSpeed), 0},//
          {191, 312},
          {253, 329},
          { -1000 * (3.2 / footSpeed), 0},
          {270, 303},
          {265, 289},
          {242, 291},
          {213, 304},
          { -1000 * (3.2 / footSpeed), 0},
        },
        //RL
        {
          {213, 304},
          {213, 304},
          {213, 304},
          {191, 312},
          {253, 329},
          {270, 303},
          {265, 289},
          {242, 291},
          {213, 304},
          { -1000 * (13.3 / footSpeed), 0},//
        },
        //RR
        {
          {213, 304},
          {213, 304},
          { -1000 * (10.1 / footSpeed), 0},//
          {191, 312},
          {253, 329},
          {270, 303},
          {265, 289},
          {242, 291},
          {213, 304},
          { -1000 * (3.2 / footSpeed), 0},
        },
        //FR
        {
          {253, 329},
          {253, 329},
          {253, 329},
          { -1000 * (3.2 / footSpeed), 0},
          {270, 303},
          {265, 289},
          {242, 291},
          {213, 304},
          { -1000 * (10.1 / footSpeed), 0},
          {191, 312},
        }
      };
      
      setPaths(paths, footSpeed);
      
      setLegs(0, 213, 304);
      delay(2000);

      fl.setYZ(paths[0][0][0], paths[0][0][1]);
      rl.setYZ(paths[1][0][0], paths[1][0][1]);
      rr.setYZ(paths[2][0][0], paths[2][0][1]);
      fr.setYZ(paths[3][0][0], paths[3][0][1]);
      delay(1000 * (2.0 / 8));
      //Serial.println("gaitEight loaded");
    }

    

    void updateLegAngs() {
      fl.updateAngs();
      rl.updateAngs();
      rr.updateAngs();
      fr.updateAngs();
    }

    void setPaths(int paths[4][10][2], double footSpeed) {
      fl.setPath(paths[0], footSpeed);
      rl.setPath(paths[1], footSpeed);
      rr.setPath(paths[2], footSpeed);
      fr.setPath(paths[3], footSpeed);
    }

    void stand() {
      setLegs(0, 233, 299);
    }

    void sit() {
      setLegs(5, 220, 360);
    }

    void stand(double footSpeed) {

      setHips(164, 164, 16, 16);
      fl.setYZ(215, 320);
      rl.setYZ(215, 320);
      rr.setYZ(215, 320);
      fr.setYZ(215, 320);
      delay(2000);

      setHips(180, 180, 0, 0);

      double distMax = 0;
      double distTemp = 0;

      distTemp = fl.dist(fl.getHipXAngle(), fl.getKneeAngle(), 233, 299);
      if (distTemp > distMax) {
        distMax = distTemp;
      }

      distTemp = rl.dist(rl.getHipXAngle(), rl.getKneeAngle(), 233, 299);
      if (distTemp > distMax) {
        distMax = distTemp;
      }

      distTemp = rr.dist(rr.getHipXAngle(), rr.getKneeAngle(), 233, 299);
      if (distTemp > distMax) {
        distMax = distTemp;
      }

      distTemp = fr.dist(fr.getHipXAngle(), fr.getKneeAngle(), 233, 299);
      if (distTemp > distMax) {
        distMax = distTemp;
      }

      if (distMax < 0.1) {
        exit(1);
      }

      double t = (double) round(100 * (distMax / footSpeed)) / 100;

      int steps = t / 0.01;

      double diff = 0;

      fl.setHipXStep((233 - fl.getHipXAngle()) / steps);
      fl.setKneeStep((299 - fl.getKneeAngle()) / steps);

      rl.setHipXStep((233 - rl.getHipXAngle()) / steps);
      rl.setKneeStep((299 - rl.getKneeAngle()) / steps);

      rr.setHipXStep((233 - rr.getHipXAngle()) / steps);
      rr.setKneeStep((299 - rr.getKneeAngle()) / steps);

      fr.setHipXStep((233 - fr.getHipXAngle()) / steps);
      fr.setKneeStep((299 - fr.getKneeAngle()) / steps);

      for (int i = 0; i < steps; i++) {
        if (abs(233 - fl.getHipXAngle()) > 0.1 ) {
          fl.setHipX(fl.getHipXAngle() + fl.getHipXStep());
        }
        if (abs(299 - fl.getKneeAngle()) > 0.1 ) {
          fl.setKnee(fl.getKneeAngle() + fl.getKneeStep());
        }


        if (abs(233 - rl.getHipXAngle()) > 0.1 ) {
          rl.setHipX(rl.getHipXAngle() + rl.getHipXStep());
        }
        if (abs(299 - rl.getKneeAngle()) > 0.1 ) {
          rl.setKnee(rl.getKneeAngle() + rl.getKneeStep());
        }


        if (abs(233 - rr.getHipXAngle()) > 0.1 ) {
          rr.setHipX(rr.getHipXAngle() + rr.getHipXStep());
        }
        if (abs(299 - rr.getKneeAngle()) > 0.1 ) {
          rr.setKnee(rr.getKneeAngle() + rr.getKneeStep());
        }


        if (abs(233 - fr.getHipXAngle()) > 0.1 ) {
          fr.setHipX(fr.getHipXAngle() + fr.getHipXStep());
        }
        if (abs(299 - fr.getKneeAngle()) > 0.1 ) {
          fr.setKnee(fr.getKneeAngle() + fr.getKneeStep());
        }

        delay(10);
      }
    }


    void sit(double footSpeed) {

      double distMax = 0;
      double distTemp = 0;

      distTemp = fl.dist(fl.getHipXAngle(), fl.getKneeAngle(), 233, 299);
      if (distTemp > distMax) {
        distMax = distTemp;
      }

      distTemp = rl.dist(rl.getHipXAngle(), rl.getKneeAngle(), 215, 320);
      if (distTemp > distMax) {
        distMax = distTemp;
      }

      distTemp = rr.dist(rr.getHipXAngle(), rr.getKneeAngle(), 215, 320);
      if (distTemp > distMax) {
        distMax = distTemp;
      }

      distTemp = fr.dist(fr.getHipXAngle(), fr.getKneeAngle(), 233, 299);
      if (distTemp > distMax) {
        distMax = distTemp;
      }

      if (distMax < 0.1) {
        exit(1);
      }

      double t = (double) round(100 * (distMax / footSpeed)) / 100;

      int steps = t / 0.01;

      fl.setHipXStep((233 - fl.getHipXAngle()) / steps);
      fl.setKneeStep((299 - fl.getKneeAngle()) / steps);

      rl.setHipXStep((215 - rl.getHipXAngle()) / steps);
      rl.setKneeStep((320 - rl.getKneeAngle()) / steps);

      rr.setHipXStep((215 - rr.getHipXAngle()) / steps);
      rr.setKneeStep((320 - rr.getKneeAngle()) / steps);

      fr.setHipXStep((233 - fr.getHipXAngle()) / steps);
      fr.setKneeStep((299 - fr.getKneeAngle()) / steps);

      for (int i = 0; i < steps; i++) {
        if (abs(233 - fl.getHipXAngle()) > 0.1 ) {
          fl.setHipX(fl.getHipXAngle() + fl.getHipXStep());
        }
        if (abs(299 - fl.getKneeAngle()) > 0.1 ) {
          fl.setKnee(fl.getKneeAngle() + fl.getKneeStep());
        }


        if (abs(215 - rl.getHipXAngle()) > 0.1 ) {
          rl.setHipX(rl.getHipXAngle() + rl.getHipXStep());
        }
        if (abs(320 - rl.getKneeAngle()) > 0.1 ) {
          rl.setKnee(rl.getKneeAngle() + rl.getKneeStep());
        }


        if (abs(215 - rr.getHipXAngle()) > 0.1 ) {
          rr.setHipX(rr.getHipXAngle() + rr.getHipXStep());
        }
        if (abs(320 - rr.getKneeAngle()) > 0.1 ) {
          rr.setKnee(rr.getKneeAngle() + rr.getKneeStep());
        }


        if (abs(233 - fr.getHipXAngle()) > 0.1 ) {
          fr.setHipX(fr.getHipXAngle() + fr.getHipXStep());
        }
        if (abs(299 - fr.getKneeAngle()) > 0.1 ) {
          fr.setKnee(fr.getKneeAngle() + fr.getKneeStep());
        }

        delay(10);
      }


      setHips(180, 164, 16, 0);
    }

};






//dog contructors

Dog::Dog() {

}

Dog::Dog(Leg frontLeft, Leg rearLeft, Leg rearRight, Leg frontRight) {
  fl = frontLeft;

  rl = rearLeft;

  rr = rearRight;

  fr = frontRight;
}






//universal stuff
Dog skorupi;
double footSpeed;







void setup() {
  //comment this out because it slows down the dog a lot
  Serial.begin(9600);

  footSpeed = 3; //footspeed in inches/s - may be limited by servo max speed

  Leg frontLeft;
  frontLeft = Leg("frontLeft", 23, 76.0,     25, 60.0,    27, -5.0, false);

  Leg rearLeft;
  rearLeft = Leg("rearLeft", 29, 70.0,    31, 50.0,    33, 26.0, false);

  Leg rearRight;
  rearRight = Leg("rearRight", 35, 60.0,    37, 120.0,    39, 129.0, true);

  Leg frontRight;
  frontRight = Leg("frontRight", 41, 68.0,    43, 123.0,    3, 154.0, true);
  //m1, m10, m12 don't work

  skorupi = Dog(frontLeft, rearLeft, rearRight, frontRight);


  pinMode(11, INPUT_PULLUP);
  digitalWrite(11, HIGH);

  pinMode(12, INPUT_PULLUP);
  digitalWrite(12, HIGH);

  pinMode(13, INPUT_PULLUP);
  digitalWrite(13, HIGH);

  skorupi.sit();
}

boolean on = false;
void loop() {

  if((!on) && ((digitalRead(11) == LOW) || 
               (digitalRead(12) == LOW) ||
               (digitalRead(13) == LOW))){
    on = true;
    skorupi.stand(footSpeed);
    delay(2000);
    skorupi.loadStand(footSpeed); 
  }

  if (digitalRead(13) == LOW) {
    if (digitalRead(12) == LOW){
      Serial.println("11 and 13 low");
      skorupi.loadZero(footSpeed);
      delay(1000);
    }
    
    else if (digitalRead(11) == LOW){
      Serial.println("11 and 12 low");
      footSpeed-=0.5;
      Serial.println(footSpeed);
      delay(1000);
    }
    else{
      Serial.println("11 low");
      skorupi.loadSit(footSpeed);
    }
  }

  else if (digitalRead(11) == LOW) {
    if (digitalRead(12) == LOW){
      Serial.println("12 and 13 low");
      footSpeed+=0.5;
      Serial.println(footSpeed);
      delay(1000);
    }
    else{
      Serial.println("12 low");
      skorupi.loadStand(footSpeed);
    }
  }
  
  else if (digitalRead(12) == LOW){
    Serial.println("13 low");
    skorupi.loadGaitEight(footSpeed);
  }

  //skorupi.zeroAll();
  if(on){
    skorupi.updateLegAngs();
  }
  delay(10);

  //Serial.println(); Serial.println(); Serial.println(); Serial.println("delayed .01 sec"); Serial.println(); Serial.println(); Serial.println();

}
