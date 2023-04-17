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
    double hipYZero = 72.0; //Should be placed at IRL 0.0deg when zeroed
    double hipYIRLZero = 0.0;
    double hipYAngle; //current IRL angle of limb

    Servo hipX; //IRL Range: (160,320)
    double hipXZero = 116.0; //should be placed at IRL 270.0deg when zeroed
    double hipXIRLZero = 270.0;
    double hipXAngle; //current IRL angle of limb

    Servo knee; //IRL Range: (260,60) (based on knee frame of reference)
    double kneeZero = 144.0; //should be placed at IRL 270.0deg when zeroed
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
    Leg(int hipYPin, int hipXPin, int kneePin, boolean onRightQuestionMark);

    Leg(int hipYPin);

    Leg();


    void setYZ(double hipXAng, double kneeAng) {
      hipXAngle = hipXAng;
      kneeAngle = kneeAng;
      Serial.println();
      Serial.print("HipX to IRL: ");
      Serial.println(hipXAng);
      Serial.print("Knee to IRL: ");
      Serial.println(kneeAng);
      
      kneeAng = (360 - kneeAng) - (kneeIRLZero - kneeZero) - (270.0 - hipXAng); //convert to correct servo value
      hipXAng = hipXAng - (hipXIRLZero - hipXZero); //convert to correct servo value

      if (!right) { //account for leg polarity
        hipXAng = 160.0 - hipXAng;
        kneeAng = 160.0 - kneeAng;
      }
      
      hipX.writeMicroseconds(specialSauce(hipXAng));
      knee.writeMicroseconds(specialSauce(kneeAng));
      /*
      Serial.println();
      Serial.print("HipX to: ");
      Serial.println(hipXAng);
      Serial.print("Knee to: ");
      Serial.println(kneeAng);
      */
    }

    void setHip(double ang) {
      if (!right) {
        ang = 160.0 - ang;
      }

      hipY.writeMicroseconds(specialSauce(ang+hipYZero));
      hipYAngle = ang;
    }

    void zero() {
      hipY.writeMicroseconds(specialSauce(hipYZero));
      hipX.writeMicroseconds(specialSauce(hipXZero));
      knee.writeMicroseconds(specialSauce(kneeZero));
    }

    static double specialSauce(double angle) {
      double result = (angle / 160.0) * (2400 - 550) + 550;
      return result;
    }

    void set(double hipYAng, double hipXAng, double kneeAng){
      setHip(hipYAng);
      setYZ(hipXAng, kneeAng);
    }

    void setPath(int pth[10][2], double spd){
      Serial.println("Path set to:");
      for (int i = 0; i < 10; i++) {
          path[i][0] = pth[i][0];
          path[i][1] = pth[i][1];
          //Serial.print(path[i][0]);
          //Serial.print(", ");
          //Serial.println(path[i][1]);
          //Serial.println();
      }
      footSpeed = spd;
    }
    
    void updateAngs(){
      if (steps == currStep) {
       if (pathIndex != 9){
         pathIndex++;
       }
       else{pathIndex = 0;}
       /*
       Serial.print("pathIndex: ");
       Serial.println(pathIndex);
       Serial.println();
       Serial.println(path[pathIndex][0]);
       Serial.println(path[pathIndex][1]);
       Serial.println();
       */
       double distance = dist(hipXAngle, kneeAngle, path[pathIndex][0], path[pathIndex][1]);
       /*
       Serial.println();
       Serial.println(path[pathIndex][0]);
       Serial.println(path[pathIndex][1]);
       Serial.println();
      */
       double t = (double) round(100*(distance/footSpeed))/100; 
      /*
       Serial.print("time: ");
       Serial.print(t);
       Serial.println(" seconds");
      */
       steps = t/0.01;
       //Serial.print("steps: ");
       //Serial.println(steps);   
       hipXStep = (path[pathIndex][0] - hipXAngle)/steps;
       /*
       Serial.print("hipXStep: ");
       Serial.print(hipXStep);
       Serial.println(" deg");
       */
       kneeStep = (path[pathIndex][1] - kneeAngle)/steps;
       /*
       Serial.print("kneeStep: ");
       Serial.print(kneeStep);
       Serial.println(" deg");
       */
       if ((abs(hipXStep) > 6) || (abs(kneeStep) > 6)){
        Serial.println("woah there buckaroo");
        delay(100000);
       }
  
       currStep = 0;
       //Serial.print("currStep: ");
       //Serial.println(0);
      }
      
      setYZ(hipXAngle + hipXStep, kneeAngle + kneeStep);
      
      currStep++;
      //Serial.print("currStep: ");
      //Serial.println(currStep);
        
    }

    // finds the distance in the YZ plane between 2 foot positions based on the IRLangles of the
    //hipX and knee servos before and after the movement
    double dist(double prevHipX, double prevKnee, double newHipX, double newKnee){
     /*
      Serial.print("finding distance from current angles: (");
      Serial.print(prevHipX);
      Serial.print(", ");
      Serial.print(prevKnee);
      Serial.print(") to next angles: (");
      Serial.print(newHipX);
      Serial.print(", ");
      Serial.print(newKnee);          
      Serial.println(")");
      */
      double prevY = 2.165*cos(prevHipX*3.14/180)+2.675*cos(prevKnee*3.14/180);
      double prevZ = 2.165*sin(prevHipX*3.14/180)+2.675*sin(prevKnee*3.14/180); 

      double currY = 2.165*cos(newHipX*3.14/180)+2.675*cos(newKnee*3.14/180);
      double currZ = 2.165*sin(newHipX*3.14/180)+2.675*sin(newKnee*3.14/180);

      double distance = sqrt(sq(currY - prevY) + sq(currZ - prevZ));
      /*
      Serial.print("distance from current point: (");
      Serial.print(prevY);
      Serial.print(", ");
      Serial.print(prevZ);
      Serial.print(") to next point: (");
      Serial.print(currY);
      Serial.print(", ");
      Serial.print(currZ);          
      Serial.print(") is ");
      Serial.print(distance);
      Serial.println(" inches");
      */
      
      //1.6, 1.6, .56, 1.4, 2.0, 1.8, 1.5, .79, 1.6, 1.8
     
      return distance;
    }
};








//leg constructors
Leg::Leg(int hipYPin, int hipXPin, int kneePin, boolean onRightQuestionMark) {
  hipY.attach(hipYPin);
  hipYAngle = hipYZero;

  hipX.attach(hipXPin);
  hipXAngle = hipXZero;

  knee.attach(kneePin);
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



      Serial.print("YZ set to: ");
      Serial.print(hipXAng);    
      Serial.print(", ");
      Serial.println(kneeAng);
      Serial.println();
    }

    void setHips(double ang) {

      fl.setHip(ang);
      rl.setHip(ang);
      rr.setHip(ang);
      fr.setHip(ang);

      Serial.print("Hips set to: ");
      Serial.println(ang);
      Serial.println();
    }

    void zeroAll() {

      fl.zero();
      rl.zero();
      rr.zero();
      fr.zero();

      Serial.println("Zeroed");    
      Serial.println();
    }

    void setLegs(double hipYAng, double hipXAng, double kneeAng) {

      setHips(hipYAng);
      setLegsYZ(hipXAng, kneeAng);

      Serial.print("leg set to: ");
      Serial.print(hipYAng);    
      Serial.print(", ");
      Serial.print(hipXAng);
      Serial.print(", ");
      Serial.println(kneeAng);
      Serial.println();
    }


    void loadGaitOne(double footSpeed){
      setHips(0);
      
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
      
    }


    void updateLegAngs(){
      fl.updateAngs();
      rl.updateAngs();
      rr.updateAngs();
      fr.updateAngs();
    }

    void setPaths(int paths[4][10][2], double footSpeed){
      fl.setPath(paths[0], footSpeed);
      rl.setPath(paths[1], footSpeed);
      rr.setPath(paths[2], footSpeed);
      fr.setPath(paths[3], footSpeed);
    }

    void stand(){
      setLegs(0, 233, 299);
    }
};




//dog contructors

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
}






//universal stuff
Dog skorupi;
double footSpeed;







void setup() {
  //comment this out because it slows down the dog a lot
  Serial.begin(19200);

  footSpeed = 10; //footspeed in inches/s - may be limited by servo max speed

  Leg frontLeft;
  frontLeft = Leg(23, 25, 27, false);
 
  Leg rearLeft;
  rearLeft = Leg(29, 31, 33, false);
 
  Leg rearRight;
  rearRight = Leg(35, 37, 39, true);
  
  Leg frontRight;
  frontRight = Leg(41, 43, 2, true);
  
  skorupi = Dog(frontLeft, rearLeft, rearRight, frontRight);


  skorupi.zeroAll();
  delay(1000);
  
  skorupi.stand();
  delay(3000);
  
  skorupi.loadGaitOne(footSpeed);
}





void loop() {

  //skorupi.zeroAll();
  //skorupi.setLegs(0,180, 180,footSpeed);
  skorupi.updateLegAngs();
  delay(10);
  Serial.println();Serial.println();Serial.println();
  Serial.println("delayed .01 sec");
  Serial.println();Serial.println();Serial.println();

}
