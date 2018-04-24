/*
 * A first sketch to test the interrupts
 * The Arduino Leonardo has also an interrupt 
 * on digital pin 7
 */

// encoder stuff
#define CHA 3
#define CHB 2
#define PHOTOTRANSISTOR 7

volatile byte chA, chB = 0;
volatile byte bCurrent = 0;
volatile byte bPrevious = 0;
volatile int integrator = 0;

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
  attachInterrupt(digitalPinToInterrupt(CHA), chA_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHB), chB_ISR, CHANGE);
  //attachInterrupt(digitalPinToInterrupt(PHOTOTRANSISTOR), phototransistor_ISR, RISING);
}


// DRAW //////////////////////////////////////////
void loop() 
{
  delay(10);
}




void chA_ISR()
{
  bCurrent = (PIND & B00001100)>>2;
  decode();
}

void chB_ISR()
{
  bCurrent = (PIND & B00001100)>>2;
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
      Serial.println(integrator);
    }
    else if( ( bCurrent == 1 && bPrevious == 0 ) ||
             ( bCurrent == 3 && bPrevious == 1 ) ||
             ( bCurrent == 2 && bPrevious == 3 ) ||
             ( bCurrent == 0 && bPrevious == 2 ) 
           )
    {
      state = ENC_CCW;
      integrator --;
      Serial.println(integrator);
    }
    bPrevious = bCurrent;
  }  
}

