/*
 * A first sketch to test the interrupts and Leonardo
 * Remember, if we want to read pin statuses using port registers
 * (https://www.arduino.cc/en/Reference/PortManipulation)
 * we need to check the Leonardo micro pinout 
 * (http://www.pighixxx.net/pinoutspg/boards/page/3/#prettyPhoto[gallery]/5/):
 * - Digital pin 2 on Arduino Leonardo is on PORT D register, position 1;
 * - Digital pin 3 on Arduino Leonardo is on PORT D register, position 0;
 */

volatile byte chA, chB = 0;
const int CHA = 2;
const int CHB = 3;

void setup() 
{
  Serial.begin(9600);
  pinMode( CHA, INPUT);
  pinMode( CHB, INPUT);
  attachInterrupt( digitalPinToInterrupt( CHA ), chA_ISR, CHANGE);
  attachInterrupt( digitalPinToInterrupt( CHB ), chB_ISR, CHANGE);

}


void loop() 
{
  Serial.print(chA);
  Serial.print("\t");
  Serial.println(chB);
}


void chA_ISR()
{
  chA = (PIND & B00000010)>>1;
}

void chB_ISR()
{
  chB = (PIND & B00000001)>>0;
}

