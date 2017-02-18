
void LeftEncoderEvent()
{
  if (digitalRead(leftEncoderA) == HIGH)
  {
    if (digitalRead(leftEncoderB) == LOW)
      LeftEncoderPos++;
    else
      LeftEncoderPos--;
  }
  else
  {
    if (digitalRead(leftEncoderB) == LOW)
      LeftEncoderPos--;
    else
      LeftEncoderPos++;
  }
}

void RightEncoderEvent()
{
  if (digitalRead(rightEncoderA) == HIGH)
  {
    if (digitalRead(rightEncoderB) == LOW)
      RightEncoderPos++;
    else
      RightEncoderPos--;
  }
  else
  {
    if (digitalRead(rightEncoderB) == LOW)
      RightEncoderPos--;
    else
      RightEncoderPos++;
  }
}

//INTERRUPT SERVICE ROUTINE FOR LANDMARK DETECTION
void ResetPositionEvent() 
{
  currentTime = millis();
  if ((currentTime - lastLandmarkTime) > LM_UPDATE_TIME)
  {
      ResetPosition();
            lastLandmarkTime = currentTime;
  }
}

