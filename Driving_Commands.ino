
// Wheel speed setpoints
double targetLvelocity = 0;
double targetRvelocity = 0;


void driveForward()
{
        PIDenable = false;
    SetSpeed(0, 80);
  SetSpeed(1, 106);
}

void driveBackwards()
{
        PIDenable = false;
    SetSpeed(0, -80);
  SetSpeed(1, -106);
}

void driveForwardPID()
{
        PIDenable = true;
    targetLvelocity = PIDforwardSpeed;
  targetRvelocity = PIDforwardSpeed;
}

void driveBackwardsPID()
{
        PIDenable = true;
    targetLvelocity = PIDbackwardSpeed;
  targetRvelocity = PIDbackwardSpeed;
}

void stopDriving()
{
        PIDenable = false;
    SetSpeed(0, 0);
  SetSpeed(1, 0);
}

void stopDrivingPID()
{
  PIDenable = true;
        targetLvelocity = 0;
  targetRvelocity = 0;
}

void turnLeft()
{
  PIDenable = false;
  SetSpeed(0, -100);
  SetSpeed(1, 100);
}

void turnRight()
{
        PIDenable = false;
  SetSpeed(0, 100);
  SetSpeed(1, -100);
}

void command1()
{
    driveToPosition(200);
}


void driveToPosition(double targetPosition)
{
  if (position > targetPosition) //Car is ahead of our target position
  {
    //We stop and coast when we are 7 millimeters away
    while (position > (targetPosition + 7))
    {
      driveBackwardsPID();
                        SetWheelSpeeds();
    }
    stopDrivingPID();
                SetWheelSpeeds();
  }
  else //Car is behind our target position
  {
    while (position < (targetPosition - 7))
    {
      driveForwardPID();
                        SetWheelSpeeds();
    }
    stopDrivingPID();
                SetWheelSpeeds();
  }
}

void driveToPositionWithoutPID(double targetPosition)
{
  if (position > targetPosition) //Car is ahead of our target position
  {
    while (position > (targetPosition + 2)) //We stop and coast when we are 2 milimeters away
    {
      driveBackwards();
      GetSpeeds();
    }
    stopDriving();
    GetSpeeds();
  }
  else //Car is behind our target position
  {
    while (position < (targetPosition - 2)) //We stop when we are 2 milimeters away
    {
      driveForward();
      GetSpeeds();
    }
    stopDriving();
     GetSpeeds();
  }
}



