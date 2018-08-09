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
 * # RESET 
 * 
 * For the reset we are using a Grove Infrared Reflective sensor
 * link: http://wiki.seeedstudio.com/Grove-Infrared_Reflective_Sensor/
 * 
 * # CONNECTIONS
 * 
 * Below a description of all the connections:
 * - Connect CHA fork to digital pin 2 of Arduino Leonardo;
 * - Connect CHB fork to digital pin 3 of Arduino Leonardo;
 * - Connect signal from GROVE Infrared Reflective sensor to digital pin 7 of Arduino Leonardo;
 */

// encoder stuff
#define CHA 2
#define CHB 3
#define RESET 7


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
  pinMode(RESET, INPUT);
  attachInterrupt(digitalPinToInterrupt(CHA), chA_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHB), chB_ISR, CHANGE);
  // when a black dot is in sight, the signal from the GROVE sensor
  // will fall down. So we are using the FALLING edge.
  attachInterrupt(digitalPinToInterrupt(RESET), reset_ISR, FALLING);
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
  delay(5);
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


void reset_ISR()
{
  //Serial.println("ph");  
  integrator = 0;
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

