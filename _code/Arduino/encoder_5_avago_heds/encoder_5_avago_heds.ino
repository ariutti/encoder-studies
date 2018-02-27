/*
 * A sketch by Nicola Ariutti - 2018/02/21
 * Going to test an Avago HEDS-9000#T00
 * Two channel Optical Incremental Encoder Module.
 * 
 * To be used with the "processing_osc_A_B" sketch
 * in order to figure out how the quadrature works.
 */

const int LED = 13;
const int ENC_A = 2;
const int ENC_B = 3;

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


/*SETUP **********************************/
void setup() 
{
  Serial.begin( 9600 );
  //Serial.begin( 9600 );


  // turn off this annoying LED
  pinMode( LED, OUTPUT );
  digitalWrite( LED, LOW );


  // set pins as inputs
  pinMode( ENC_A, INPUT );
  pinMode( ENC_B, INPUT );


  // bit shift 4 bits in order to read
  // pin 2 and 3 as the least significant
  bCurrent = PIND >> 2;
  bCurrent = bCurrent & B00000011;
  bPrevious = bCurrent;

  /*
  //intial print
  Serial.println();
  Serial.println( "Initial print: " );
  debug();
  */
}


/* LOOP **********************************/
void loop() 
{
  // read the status of the two pins
  // (bit 2 and 3 of PORTD register respectively).
  // Do a bit shift operation to discard TX and RX bit information  
  bCurrent = PIND >> 2;
  // Do an AND to discard information from the other digital pins
  bCurrent = bCurrent & B00000011;

  /*
  if( bCurrent != bPrevious )
  {
    if( ( bCurrent == 1 && bPrevious == 0 ) ||
    ( bCurrent == 3 && bPrevious == 1 ) ||
    ( bCurrent == 2 && bPrevious == 3 ) ||
    ( bCurrent == 0 && bPrevious == 2 ) )
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
    ( bCurrent == 0 && bPrevious == 1 ) )
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

    //if( bCurrent == 0  ) 
     // Serial.println();
    //Serial.print( state );
    //Serial.print(" - ");
    
    bPrevious = bCurrent;
    //debug();
  }
  */
  
  uint8_t a = digitalRead(2);
  uint8_t b = digitalRead(3);

  if( bSendSerialData )
  {
    Serial.write(128);
    Serial.write( a );
    Serial.write(129);
    Serial.write( b );
    //Serial.print(a);
    //Serial.print(" - ");
    //Serial.println(b);
  }
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


/* DEBUG *********************************/
void debug()
{
   //Serial.print(" [");
   Serial.println( bCurrent );
   //Serial.print( " : " );
   //Serial.print( bCurrent, BIN );
   //Serial.println("]");
}
