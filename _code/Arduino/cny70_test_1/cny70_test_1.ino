/*
 * This sketch is meant to work 
 * with the Vishay CNY70
 */

#define LIGHT A0

void setup() 
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() 
{
  int v = analogRead( LIGHT );
  Serial.println( v );
  delay(100);
}
