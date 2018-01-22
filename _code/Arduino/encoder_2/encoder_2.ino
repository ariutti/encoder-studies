/*
 * a sketch by Nicola Ariutti
 * 2018/01/21
 */

const int LED = 13;
const int ENC_A = 2;
const int ENC_B = 3;

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

  // intial print
  Serial.println();
  Serial.println( "Initial print: " );
  Serial.print( bCurrent );
  Serial.print( " : " );
  Serial.println( bCurrent, BIN );
  Serial.println();
}


/* LOOP **********************************/
void loop() 
{
  bCurrent = PIND >> 2;
  bCurrent = convert( bCurrent );
  if( bCurrent != bPrevious ) 
  {
    Serial.print( bCurrent );
    Serial.print( " : " );
    Serial.println( bCurrent, BIN );
    bPrevious = bCurrent;
  }
  
  
}

/* because we are usign pullups 
 * our logis will appear inverted.
 * We use this function to invert
 * the logic
 */
byte convert( byte _b )
{
  byte c = ~_b; // bitwise not
  c = c & B00000011; //bitwise and 
  return c;
}


/* DIGITAL PLOT **************************/
void digitalPlot( int _digitalPinNumber, int _addr )
{
  int value = digitalRead( _digitalPinNumber );
  value *= 1023;
  
  byte lower = value & 0x7F;
  byte addr = _addr & 0x0F; // 0000 1111
  byte upper = (value >> 7) | (addr << 3) | 0x80;
  
  Serial.write( upper );
  Serial.write( lower ); 
  //delay( 1 ); 
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



