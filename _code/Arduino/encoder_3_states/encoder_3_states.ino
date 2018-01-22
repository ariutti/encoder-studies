/*
 * a sketch by Nicola Ariutti
 * 2018/01/21
 * Hardware:
 * Connect the rotary encoder chA to digital pin 2;
 * Connect the rotary encoder chB to digital pin 3;
 * This sketch is ment to be used w/ the
 * 'Processing_encoder_fb' sketch.
 */

const int LED = 13;
const int ENC_A = 2;
const int ENC_B = 3;

uint8_t cw[] = {1,3,2,0}; // clockwise seq
uint8_t ccw[]= {2,3,1,0}; // counter-clockwise seq
uint8_t seq[] = {0, 0, 0, 0}; // where to store the current sequence
uint8_t cnt = 0;
byte bCurrent;
byte bPrevious;

boolean bSendToPlotter;


/*SETUP **********************************/
void setup() 
{
  Serial.begin( 9600 );
  pinMode( LED, OUTPUT );
  digitalWrite( LED, false );

  // set pins as inputs and activate
  // internal pullups. It means that when
  // switches are open, we read HIGH on the pin.
  pinMode( ENC_A, INPUT_PULLUP );
  pinMode( ENC_B, INPUT_PULLUP );

  bCurrent = PIND >> 2;
  bCurrent= convert( bCurrent );
  bPrevious = bCurrent;

  /*
  //intial print
  Serial.println();
  Serial.println( "Initial print: " );
  Serial.print( bCurrent );
  Serial.print( " : " );
  Serial.println( bCurrent, BIN );
  Serial.println();
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
  bCurrent = convert( bCurrent );
  
  if( bCurrent != bPrevious ) 
  {
    //debug();
    bPrevious = bCurrent;

    switch(cnt)
    {
      
      case 0:
        if(bCurrent == 1)
        {
          // starting a CW sequence
          seq[ cnt ] = bCurrent;
          cnt++;
          break;
        } 
        else if( bCurrent == 2) 
        {
          //starting a CCW sequence
          seq[ cnt ] = bCurrent;
          cnt++;
          break;
        }
        // this happens when cnt=0 and bCurrent=0
        //Serial.println("case cnt=0: do nothing");
        break;

        
      case 1:
        if(bCurrent == 3)
        {
          // continuing both the CW and CCW seq
          seq[ cnt ] = bCurrent;
          cnt++;
          break;
        }
        //Serial.println("case 1: invalid entry");
        //resetSeq();
        break;

        
      case 2:
        if(bCurrent == 2)
        {
          // continuing a CW sequence
          seq[ cnt ] = bCurrent;
          cnt++;
          break;
        } 
        else if( bCurrent == 1) 
        {
          // continuing a CCW sequence
          seq[ cnt ] = bCurrent;
          cnt++;
          break;
        }
        //Serial.println("case 2: invalid entry");
        //resetSeq();
        break;

        
      case 3:
        if(bCurrent == 0)
        {
          // finishing both the CW and CCW seq
          seq[ cnt ] = bCurrent;
          checkCw();
          checkCcw();
          resetSeq();
          break;
        }
        //Serial.println("case 3: invalid entry");
        //resetSeq();
        break;

        
      default:
        //do nothing
        break;
    }
  } 
  else
  {
    // no new movements on the encoder
  }
}



/* UTILS *********************************/
/* 
 * Because we are usign pullups 
 * our logic will be inverted.
 * We use this function to invert the logic
 */
byte convert( byte _b )
{
  byte c = ~_b; // bitwise not
  c = c & B00000011; //bitwise and 
  return c;
}

bool checkCw()
{
  for(int i=0; i<4; i++) {
    if( seq[i] != cw[i] ) {
      return false;
    }
  }
  //Serial.println("\tCW");
  Serial.write(1);
  return true;
}

bool checkCcw()
{
  for(int i=0; i<4; i++) {
    if( seq[i] != ccw[i] ) {
      return false;
    }
  }
  //Serial.println("\tCCW");
  Serial.write(2);
  return true;
}

void resetSeq()
{
  //Serial.print("reset seq: ");
  for(int i=0; i<4; i++) {
    seq[i]= 0;
  }
  cnt = 0;
  //debug();
}


/* SERIAL EVENT **************************/
void serialEvent()
{
  byte b = Serial.read();
  if (b == 'o' || b == 'O')
    bSendToPlotter = true;
  else if (b == 'c' || b == 'C')
    bSendToPlotter = false;
  //else if (b == 'r')
    // do something like a reset
}

void debug()
{
   Serial.print("cnt: ");
   Serial.print( cnt );
   Serial.print(" [");
   Serial.print( bCurrent );
   Serial.print( " : " );
   Serial.print( bCurrent, BIN );
   Serial.println("]");
}


