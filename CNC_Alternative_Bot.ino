//Include the libraries

#include "AFMotor.h"
#include "LiquidCrystal.h"
#include "SoftwareSerial.h"

//Include our header files

#include "IOpins.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////       GLOBAL VARIABLES        ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//XBEE

SoftwareSerial xbee(xbeeRX, xbeeTX); // RX, TX

//Motors and Encoder information

volatile unsigned int LeftEncoderPos = 0;		//Left encoder count
volatile unsigned int RightEncoderPos = 0;		//Right encoder count


// time of last speed update
unsigned long LastUpdate = 0;


//Distance traveled from starting point
double position = 0; //Position from starting point in meters
long newPosUpdateTime = 0; //Time it was updated in ms since program started
long oldPosUpdateTime = 0; //Time it was last updated in ms since program started


// Speed output variables
double leftSpeedCmd = 0;
double rightSpeedCmd = 0;

//Terrain 
#define TERRAIN_LENGTH 2000				//Terrain length in milimeters
#define LANDMARK_RESOLUTION 50			//Distance between landmarks in milimiters
#define NUM_OF_LANDMARKS (TERRAIN_LENGTH/LANDMARK_RESOLUTION)

//Distance resetting with landmark
volatile long lastLandmarkTime = 0;
volatile long currentTime = 0;
volatile boolean resetLandmark = false;
#define LM_UPDATE_TIME 500	//Min time required for landmark update (in ms)
int landmarks[NUM_OF_LANDMARKS];	//Array containing positions of all landmarks

int loopx = 0;

//LCD

LiquidCrystal lcd(40, 41, 45, 44, 43, 42); 
#define DISPLAY_REFRESH_DELAY 50				//Refresh delay every some milliseconds

//AF MOTOR

AF_DCMotor leftMotor(1, MOTOR12_64KHZ); // create motor #1, 64KHz pwm
AF_DCMotor rightMotor(2, MOTOR12_64KHZ); // motor #2

boolean PIDenable = true;
float PIDforwardSpeed = 0.15;
float PIDbackwardSpeed = -0.15;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////		SETUP			//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
	Serial.begin(9600);
        
    // Set input pin state for the line sensors. Everything else is output by default.
	pinMode(leftEncoderA, INPUT);
	pinMode(leftEncoderB, INPUT);
	pinMode(rightEncoderA, INPUT);
	pinMode(rightEncoderB, INPUT);
  pinMode(19, INPUT_PULLUP);

	newPosUpdateTime = millis();

	// This sets the EncoderEvent() function to be called whenever a line detector detects a change
	// See http://arduino.cc/en/Reference/attachInterrupt for interrupt reference
	attachInterrupt(2, RightEncoderEvent, CHANGE); //Interrupt num 2 is pin 21
	attachInterrupt(3, LeftEncoderEvent, CHANGE); //Interrupt num 3 is pin

    //This encoder will take care of reseting positions every rising edge
	attachInterrupt(4, ResetPositionEvent, RISING); //Interrupt num 4 is pin 19

	//We setup our display
	setupDisplay();

    //Setup xbee communication
    xbee.begin(9600);
    Serial.begin(9600);

	//Initialize array of landmark positions
	landmarks[NUM_OF_LANDMARKS];

	int tempPos = 0;
	for (int i = 0; i <= NUM_OF_LANDMARKS; i++)
	{
		landmarks[i] = tempPos;
		tempPos += LANDMARK_RESOLUTION;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////		MAIN LOOP		//////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{    
  char command;
	long mainLoopTime = 0;
	long lastDisplayRefreshTime = 0;

	while (1)
	{
		mainLoopTime = millis();

		if ((mainLoopTime - lastDisplayRefreshTime) > DISPLAY_REFRESH_DELAY) //We refresh the display every 300 ms
		{
			lcd.clear();
                        printToDisplay1("D=" + String(position) + " V=" + String(leftVelocity * 1000));
			printToDisplay2("E=" + String(LeftEncoderPos) + " V=" + String(rightVelocity * 1000));
			lastDisplayRefreshTime = mainLoopTime;

                        Serial.println("Target velocity L " + String(targetLvelocity*1000));
                        Serial.println("Target velocity R " + String(targetRvelocity*1000));
                        Serial.println("velocity L " + String(leftVelocity*1000));
                        Serial.println("velocity R " + String(rightVelocity*1000));
                        Serial.println("");
		}
		
		if (xbee.available()) //This code needs some cleaning
		{
			// If data comes in from XBee, send it out to serial monitor and perfrom command
                        
			command = xbee.read();
			Serial.println("Char received = " + String(command));			
			performCommand(command);
		}
        
        if(PIDenable)
        {
            SetWheelSpeeds();
        }
        else
        {
            GetSpeeds();
        }
		
	}
}


void performCommand(char command)
{
	switch (command){
        Serial.println("LOL");
	case 'W':
		driveForward();
		break;
	case 'S':
		driveBackwards();
		break;
	case 'A':
		turnLeft();
		break;
	case 'D':
		turnRight();
		break;
	case 'Q':
		stopDriving();
		break;
	case 'F':
		driveForwardPID();
		break;
	case 'B':
		driveBackwardsPID();
		break;
        case ' ':
		stopDrivingPID();
		break;
	case '1':
		command1();
  case 'R':
		position = 0;
                PIDenable = false;
                LeftEncoderPos = 0;
                RightEncoderPos = 0;
                lastlefterror = 0;
                lastrighterror = 0;
                lefti = 0;
                righti = 0;
                leftVelocity = 0;
                rightVelocity = 0;
		break;
	}

	return;
}
