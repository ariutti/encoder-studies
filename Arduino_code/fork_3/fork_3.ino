/*
 * A sketch by Nicola Ariutti - 2018/03/01
 * Here I'm usign a custom prototype made 
 * of 2 fork light switch SHARP GP1A51HRJ00F.
 * 
 * The two fork are mounted on a codewheel and 
 * properly spaced apart in order to create the
 * quadrature pattern.
 */

// ENCODER STUFF ******************************/
const int ENC_CHA = 2;
const int ENC_CHB = 3;
uint8_t shift = 2;

byte bCurrent;
byte bPrevious;

int integrator = 0;

enum STATE 
{
  ENC_IDLE,
  ENC_CW,
  ENC_CCW
} state;

// Black dot reset stuff **********************/
// a threshold for the signal going from LOW to HIGH
const uint8_t TH_HI = 240;
// a threshold for the signal going from HIGH to LOW
const uint8_t TH_LO = 210;
// filter coefficients
float B, A = 0.3;
float value = 0.0;
boolean below = false;

// Serial Comm ********************************/
boolean bSendSerialData = false;

// Other stuff ********************************/
const int LED = 13;
const int DELAY_TIME = 5;

// TODO: map encoder on interrupts!!

// SETUP ///////////////////////////////////////
void setup() 
{
  Serial.begin( 9600 );
  
  // turn off this annoying LED
  pinMode( LED, OUTPUT );
  digitalWrite( LED, LOW );

  // set pins as inputs
  pinMode( ENC_CHA, INPUT );
  pinMode( ENC_CHB, INPUT );

  bCurrent = (PIND >> shift) & B00000011;
  bPrevious = bCurrent;

  // filter stuff
  B = 1.0 - A;
}

// LOOP ////////////////////////////////////////
void loop() 
{
  // read the status of the two pins 
  // Do a bit shift operation to discard TX and RX bit information  
  // Do an AND to discard information from the other digital pins
  bCurrent = (PIND >> shift) & B00000011;

  // encoder stuff
  if( bCurrent != bPrevious )
  {
    if( ( bCurrent == 1 && bPrevious == 0 ) ||
        ( bCurrent == 3 && bPrevious == 1 ) ||
        ( bCurrent == 2 && bPrevious == 3 ) ||
        ( bCurrent == 0 && bPrevious == 2 ) 
      )
    {
      state = ENC_CW;
      if(bCurrent == 0 && bPrevious == 2)
      {
        integrator ++;
        Serial.print(" CW: ");
        Serial.println(integrator);
        //if( bSendSerialData )
        //  Serial.write(1);
      }
    }
    else if( ( bCurrent == 2 && bPrevious == 0 ) ||
             ( bCurrent == 3 && bPrevious == 2 ) ||
             ( bCurrent == 1 && bPrevious == 3 ) ||
             ( bCurrent == 0 && bPrevious == 1 ) 
           )
    {
      state = ENC_CCW;
      if(bCurrent == 0 && bPrevious == 1)
      {
        integrator --;
        Serial.print("CCW: ");
        Serial.println(integrator);
        //if( bSendSerialData )
        //  Serial.write(2);
       }
    }
    bPrevious = bCurrent;
  }  

  
  // Black dot stuff **************************/
  // filter stuff for black reset spot
  value = A*analogRead(A0) + B*value;
  if(!below && value > TH_HI)
  {
    below = true;
    digitalWrite(LED, HIGH);
    Serial.println(1);
    
  }
  else if(below && value < TH_LO)
  {
    below = false;
    digitalWrite(LED, LOW);
    Serial.println(0);
  }
    
  delay(DELAY_TIME);  
}

// SERIAL EVENT ////////////////////////////////
void serialEvent()
{
  byte b = Serial.read();
  if (b == 'o' || b == 'O')
    bSendSerialData = true;
  else if (b == 'c' || b == 'C')
    bSendSerialData = false;
  //else if (b == 'r')
    // do something like a reset
}
