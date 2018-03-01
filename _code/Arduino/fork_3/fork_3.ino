/*
 * A sketch by Nicola Ariutti - 2018/03/01
 * Here I'm usign a custom prototype made 
 * of 2 fork light switch SHARP GP1A51HRJ00F.
 * 
 * The two fork are mounted on a codewheel and 
 * properly spaced apart in order to create the
 * quadrature pattern.
 */


const int ENC_CHA = 2;
const int ENC_CHB = 3;
uint8_t shift = 2;

byte bCurrent;
byte bPrevious;

boolean bSendSerialData = false;

int integrator = 0;

enum STATE 
{
  ENC_IDLE,
  ENC_CW,
  ENC_CCW
} state;


// Reset
const int LED = 13;
// a threshold for the signal going from LOW to HIGH
uint8_t hysteresis_H = 240;
// a threshold for the signal going from HIGH to LOW
uint8_t hysteresis_L = 210;
// filter coefficients
const float A = 0.3;
float B;
float value = 0.0;

/*SETUP **********************************/
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

  B = 1.0 - A;
}


/* LOOP **********************************/
void loop() 
{
  // read the status of the two pins 
  // Do a bit shift operation to discard TX and RX bit information  
  // Do an AND to discard information from the other digital pins
  bCurrent = (PIND >> shift) & B00000011;


  // I've tempoarily commented out this section
  // in order to debug the black reset dot!
  /*
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
  */
  

  // filter stuff for black reset spot 
  value = A*analogRead(A0) + B*value;
  if(value > hysteresis_H)
    digitalWrite(LED, HIGH);
  else if(value < hysteresis_L)
    digitalWrite(LED, LOW);
    
  Serial.println(value);
  delay(10);  
}


/* SERIAL EVENT **************************/
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
