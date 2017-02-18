#define LEFT 0
#define RIGHT 1

// PID Gains
const double Kp = 3;
const double Ki = 8;
const double Kd = 0.005;
const double maxint = 10.0;


// memory for I and D terms
double lastlefterror = 0;
double lastrighterror = 0;
double lefti = 0;
double righti = 0;

// Velocity stuff
double leftVelocity = 0; //Left Velocity in meters/sec
double rightVelocity = 0;  //Right Velocity in meters/sec
const double maxPossibleSpeedL = 0.79; //Maximum velocity running at 100% PWM, 9 volts.
//(90 mm diameter wheels running at 262.5 rpm)
const double maxPossibleSpeedR= 0.79; //Maximum velocity running at 100% PWM, 9 volts.

const int maxSpeed = 255;

// Sample time for velocity calculation.  Lower resolution encoders need a longer sample time for an accurate reading
#define DELAY_TIME 30
#define WHEEL_CIRCUMFERENCE_LEFT 0.2825 // In meters
#define WHEEL_CIRCUMFERENCE_RIGHT 0.2825 // In meters
#define WHEEL_TICKS 928            // The number of encoder 'ticks' for a full wheel cycle
                                   // This is the number for one of the quadrature encoders.

// Where the magic happens
void SetWheelSpeeds() //This needs to be called in our main loop.
{
  GetSpeeds();

  // find errors, dt
  double lefterror = (double)targetLvelocity - (double)leftVelocity;
  double righterror = (double)targetRvelocity - (double)rightVelocity;
  double dt = (millis() - LastUpdate);

  // convert to seconds
  dt /= 1000.0;
  // i
  lefti += lefterror * dt;
  righti += righterror * dt;
  
  // prevent integral windup
  lefti = constrain(lefti, -maxint, maxint);
  righti = constrain(righti, -maxint, maxint);
  // d
  double leftd = (lefterror - lastlefterror) / dt;
  double rightd = (righterror - lastrighterror) / dt;
  // sum
  leftSpeedCmd = (Kp * lefterror) + (Ki * lefti) + (Kd * leftd);
  rightSpeedCmd = (Kp * righterror) + (Ki * righti) + (Kd * rightd);

    //Convert the Cmd speeds from m/s to -256 to 255 range;

    leftSpeedCmd = (leftSpeedCmd/maxPossibleSpeedL)*255;
    rightSpeedCmd = (rightSpeedCmd/maxPossibleSpeedR)*255;
        
  // bounds check
  leftSpeedCmd = constrain(leftSpeedCmd, -maxSpeed, maxSpeed);
  rightSpeedCmd = constrain(rightSpeedCmd, -maxSpeed, maxSpeed);
        
//        Serial.println("Left Speed CMD = " + String(leftSpeedCmd));
//        Serial.println();
        
  // output speeds
  SetSpeed(LEFT, leftSpeedCmd);
  SetSpeed(RIGHT, rightSpeedCmd);

  // store info for next time
  LastUpdate = millis();
  lastlefterror = lefterror;
  lastrighterror = righterror;
}

void SetSpeed(int motorNumber, int newSpeed)
{
  //Sets the vehicle speed

  if (motorNumber == 0)
  {
    if (newSpeed > 0)
    {
      leftMotor.setSpeed(abs(newSpeed));
      leftMotor.run(FORWARD);
    }
    else
    {
      leftMotor.setSpeed(abs(newSpeed));
      leftMotor.run(BACKWARD);
    }
  }
  if (motorNumber == 1)
  {
    if (newSpeed > 0)
    {
      rightMotor.setSpeed(abs(newSpeed));
      rightMotor.run(FORWARD);
    }
    else
    {
      rightMotor.setSpeed(abs(newSpeed));
      rightMotor.run(BACKWARD);
    }
  }
}



// Return the real-world vehicle speed (in meters per second)
void GetSpeeds()
{
  long NewLPos;
  long NewRPos;
  long OldLPos;
  long OldRPos;
  double LdVal = 0; //Left encoder ticks per second
  double RdVal = 0; //Right encoder ticks per second

  // Find the old and new encoder positions
  OldLPos = LeftEncoderPos;
  OldRPos = RightEncoderPos;
  delay(DELAY_TIME);
  NewLPos = LeftEncoderPos;
  NewRPos = RightEncoderPos;

  long deltaLPos = NewLPos - OldLPos; //Amount of L ticks counted during delay time
  long deltaRPos = NewRPos - OldRPos; //Amount of R ticks counted during delay time

  // If an integer overflow hasn't occured, throw out the new value for LdVal and RdVal
  if (abs(deltaLPos) < 60000)
    LdVal = ((double)(deltaLPos) / (double)DELAY_TIME) * 1000.0;
  if (abs(deltaRPos) < 60000)
    RdVal = ((double)(deltaRPos) / (double)DELAY_TIME) * 1000.0;
        
//        Serial.println("Old L pos = " + String(OldLPos));
//        Serial.println("New R pos = " + String(NewLPos));
//        Serial.println("ticks per sec = " + String(LdVal));

  // Find the linear velocity
  double LangVel = LdVal / (double)WHEEL_TICKS;
  leftVelocity = (LangVel * WHEEL_CIRCUMFERENCE_LEFT);

  double RangVel = RdVal / (double)WHEEL_TICKS;
  rightVelocity = (RangVel * WHEEL_CIRCUMFERENCE_RIGHT);

  UpdatePosition();
}

void UpdatePosition()
{
  //We update the distance traveled. We do this taking the average of both

  double avgVelocity = (leftVelocity + rightVelocity) / 2;
  oldPosUpdateTime = newPosUpdateTime;
  newPosUpdateTime = millis();
  position = position + ( avgVelocity * (double)(newPosUpdateTime - oldPosUpdateTime) );
}

