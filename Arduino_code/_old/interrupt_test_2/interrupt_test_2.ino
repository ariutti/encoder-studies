/*
 * Interrupts and Leonardo!
 * Remember, if we want to read pin statuses using port registers
 * (https://www.arduino.cc/en/Reference/PortManipulation)
 * we need to check the Leonardo 32u4 microcontroller pinout 
 * (http://www.pighixxx.net/pinoutspg/boards/page/3/#prettyPhoto[gallery]/5/):
 * 
 * - Digital pin 2 on Arduino Leonardo is on PORT D register, position 1;
 * - Digital pin 3 on Arduino Leonardo is on PORT D register, position 0;
 * 
 */

// encoder stuff
#define CHA 2
#define CHB 3
//#define PHOTOTRANSISTOR 7


volatile byte chA, chB = 0;
volatile byte bCurrent = 0;
volatile byte bPrevious = 0;
volatile int integrator = 0;
volatile int prevIntegrator = 0;

volatile enum STATE 
{
  ENC_IDLE,
  ENC_CW,
  ENC_CCW
} state;

// SETUP /////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  pinMode(CHA, INPUT);
  pinMode(CHB, INPUT);
  attachInterrupt(digitalPinToInterrupt(CHA), chA_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHB), chB_ISR, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(PHOTOTRANSISTOR), phototransistor_ISR, RISING);
}


// DRAW //////////////////////////////////////////
void loop() 
{
  if(integrator != prevIntegrator)
  {
    Serial.print(integrator);
    Serial.println(";");
    prevIntegrator = integrator;
  }
  delay(10);
}


// OTHER STUFF ///////////////////////////////////
void chA_ISR()
{
  bCurrent = (PIND & B00000011)>>0;
  decode();
}

void chB_ISR()
{
  bCurrent = (PIND & B00000011)>>0;
  decode();
}


void phototransistor_ISR()
{
  Serial.println("ph");  
}



void decode() 
{
  //Serial.println(bCurrent);
  
  // encoder stuff
  if( bCurrent != bPrevious )
  {
    if( ( bCurrent == 2 && bPrevious == 0 ) ||
        ( bCurrent == 3 && bPrevious == 2 ) ||
        ( bCurrent == 1 && bPrevious == 3 ) ||
        ( bCurrent == 0 && bPrevious == 1 )  
      )
    {
      state = ENC_CW;
      integrator ++;
      //Serial.println(integrator);
    }
    else if( ( bCurrent == 1 && bPrevious == 0 ) ||
             ( bCurrent == 3 && bPrevious == 1 ) ||
             ( bCurrent == 2 && bPrevious == 3 ) ||
             ( bCurrent == 0 && bPrevious == 2 ) 
           )
    {
      state = ENC_CCW;
      integrator --;
      //Serial.println(integrator);
    }
    bPrevious = bCurrent;
  }  
}

