/*
 * a sketch by Nicola Ariutti
 * 2018/01/21
 * To be used w/ Processing Oscilloscope
 * src: https://github.com/Limulo/Processing-oscilloscope
 */

const int LED = 13;
const int ENC_A = 2;
const int ENC_B = 3;

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
}


/* LOOP **********************************/
void loop() 
{
  if( bSendToPlotter )
  {
    digitalPlot( ENC_A, 0 );
    digitalPlot( ENC_B, 1 );
  }
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



