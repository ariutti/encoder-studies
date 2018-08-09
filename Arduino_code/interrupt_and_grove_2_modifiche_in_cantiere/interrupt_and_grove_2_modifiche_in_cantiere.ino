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
 * - Digital pin 7 on Arduino Leonardo is on PORT E register, position 6;
 * 
 * # CONNECTIONS
 * 
 * Below a description of all the connections:
 * - Connect CHA fork to digital pin 2 of Arduino Leonardo;
 * - Connect CHB fork to digital pin 3 of Arduino Leonardo;
 * - Connect signal from GROVE Infrared Reflective sensor to digital pin 7 of Arduino Leonardo;
 * 
 * 2018-08-09 - MODS
 * Because we have a large black horizontal line on the PVC tape as reset,
 * and because of the fact that a code like this:
 * 
 * [...]
 * attachInterrupt(digitalPinToInterrupt(RESET), reset_ISR, CHANGE);
 * [...]
 * void reset_ISR()
 * {
 *   integrator = 0;
 *   if( bSendSerial ) { Serial.println("r;"); }
 * }
 * 
 * would have trigger the reset twice (one in getting inside the black line,
 * the other getting outside it), I have made some modification in order for the 
 * reset to happen only on one of these two margin.
 * 
 * In order to do this I was forced to read read the status of the pin (register PINE)
 * and behaving differently if we are moving CW or CCW.
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

volatile bool bReset = false;
bool bSendSerial = false;




volatile enum STATE 
{
  ENC_IDLE,
  ENC_CW,
  ENC_CCW
} status;




// SETUP /////////////////////////////////////////
void setup() 
{
  Serial.begin(9600);
  pinMode(CHA, INPUT);
  pinMode(CHB, INPUT);
  pinMode(RESET, INPUT);
  attachInterrupt(digitalPinToInterrupt(CHA), chA_ISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(CHB), chB_ISR, CHANGE);
  // When a black dot is in sight, the signal from the GROVE sensor
  // will go up (while onboard LED will go OFF). When no more black
  // in front of the sensor, signal will return LOW (on board LED will light up)
  //attachInterrupt(digitalPinToInterrupt(RESET), reset_ISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(RESET), reset_ISR, CHANGE);
  status = ENC_IDLE;

}


// DRAW //////////////////////////////////////////
void loop() 
{
  // get serial information
  getSerialData();  
  
  if(integrator != prevIntegrator)
  {
    if(bSendSerial) 
    { 
      Serial.print(integrator);
      Serial.println(";"); 
    }
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
//   
//  switch( status ) {
//    case ENC_CW:
//      //Serial.println("CW");
//      break;
//    case ENC_CCW:
//      //Serial.println("CCW");
//      break;
//    default:
//      break;
//  }
  
  // Digital pin 7 on Arduino Leonardo is on PORT E register, position 6;
  bReset = (PINE >> 6) & B00000001;

  
  if(bReset && status==ENC_CW)
  {
    //Serial.println(" == CW RESET LINE ===================================== ");
    integrator = 0;
    if( bSendSerial ) { Serial.println("r;"); }
  }
  else if(!bReset && status==ENC_CCW)
  {
    // Serial.println(" == CCW RESET LINE ==================================== ");
    integrator = 0;
    if( bSendSerial ) { Serial.println("r;"); }
  }
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
      status = ENC_CW;
      integrator ++;
      //Serial.println(integrator);
    }
    else if( ( bCurrent == 1 && bPrevious == 0 ) ||
             ( bCurrent == 3 && bPrevious == 1 ) ||
             ( bCurrent == 2 && bPrevious == 3 ) ||
             ( bCurrent == 0 && bPrevious == 2 ) 
           )
    {
      status = ENC_CCW;
      integrator --;
      //Serial.println(integrator);
    }
    bPrevious = bCurrent;
  }  
}


// SERIAL UTILITY /////////////////////////////////////////////////
void getSerialData()
{
  if(Serial.available()) 
  {
    char user_input = Serial.read(); 
      
    if (user_input =='1')  {
       bSendSerial = true;
    }
    else if(user_input =='0')  {
      bSendSerial = false;
    }
  }
}
