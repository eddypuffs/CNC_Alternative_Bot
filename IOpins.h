//Motor pins info can be found here: http://www.robotshop.com/media/files/pdf/datasheet-1443.pdf
//Black and Red --> Power
//Blue --> Hall Sensor Vcc
//Green --> Hall Sensor Vss
//Yellow --> Encoder A
//White --> Encoder B

#define leftEncoderA    20                         //  Left encoder pin that will trigger interrupt 0
#define leftEncoderB    22						   //  Secondary left encoder pin

#define rightEncoderA   21						    // Right encoder pin that will trigger interrupt 1
#define rightEncoderB   23	     				    // Secondary right encoder pin

#define lcdEn   40		//LCD PINS				    
#define lcdRS   41
#define lcdD4	45						    
#define lcdD5	44
#define lcdD6	43
#define lcdD7	42

#define xbeeRX	53
#define xbeeTX	52

#define landmarkDetection	19
