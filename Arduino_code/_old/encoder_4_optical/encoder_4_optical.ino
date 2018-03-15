/*
 * a preliminary sketch 
 * by Nicola Ariutti - 2018/01/22
 * Usign a Bourns 
 * ENA1D-B28-L00128L
 * optical encoder
 */

const int LED = 13;
const int ENC_A = 2;
const int ENC_B = 3;

/*
// unused
uint8_t cw[] = {1,3,2,0}; // clockwise seq
uint8_t ccw[]= {2,3,1,0}; // counter-clockwise seq
uint8_t seq[] = {0, 0, 0, 0}; // where to store the current sequence
uint8_t cnt = 0;
*/

byte bCurrent;
byte bPrevious;

boolean bSendSerialData = false;

int CW = 0;
int CCW = 0;


enum STATE {
  ENC_IDLE,
  ENC_CW,
  ENC_CCW
} state;


/*SETUP **********************************/
void setup() 
{
  Serial.begin( 230400 );
  //Serial.begin( 9600 );


  // LED
  pinMode( LED, OUTPUT );
  digitalWrite( LED, false );


  // set pins as inputs and activate
  // internal pullups. It means that when
  // switches are open, we read HIGH on the pin.
  pinMode( ENC_A, INPUT );
  pinMode( ENC_B, INPUT );


  // bit shift 4 bits in order to read
  // pin 4 and 5 as the least significant
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
  // read the status of pin 2 and 3
  // bit 2 and 3 of PORTD register respectively.
  // Do a bit shift operation to discard TX and RX bit information  
  bCurrent = PIND >> 2;
  // Do an AND to discard information from the other digital pins
  bCurrent = bCurrent & B00000011;
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
        CW ++;
        //Serial.println("CW");
        if( bSendSerialData )
          Serial.write(1);
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
        CCW ++;
        //Serial.println("CCW");
        if( bSendSerialData )
          Serial.write(2);
       }
    }

    //if( bCurrent == 0  ) 
     // Serial.println();
    //Serial.print( state );
    //Serial.print(" - ");
    
    bPrevious = bCurrent;
    //debug();
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
