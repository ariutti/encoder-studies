/*
 * Custom light sensor
 */

#define LIGHTSENSOR A0

void setup() 
{
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

void loop() 
{
  int val = analogRead(LIGHTSENSOR);
  Serial.println(val);
}
