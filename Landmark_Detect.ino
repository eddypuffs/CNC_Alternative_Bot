void ResetPosition()
{
  //We find the closest landmark to our current position using our landmarks array

  //First we round our position (double) to (int)
  int postitionInt = (int) round(position);

  //Then we match the closest value of this to our landmarks[] array

  int closestPosition = 0;
  unsigned int closestPositionError = 0xFFFF;

  for (int i = 0; i < NUM_OF_LANDMARKS; i++)
  {
    int tempError = abs(landmarks[i] - position);

    if (tempError < closestPositionError)
    {
      closestPosition = landmarks[i];
                        closestPositionError = tempError;
    }
  }

  position = closestPosition;
}
