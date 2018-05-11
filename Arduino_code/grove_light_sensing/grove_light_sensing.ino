// http://wiki.seeedstudio.com/Grove-Infrared_Reflective_Sensor/

#define DOT 2


void setup()
{
  Serial.begin(9600);
  pinMode(DOT, INPUT);
}


void loop()
{
  bool state = digitalRead(DOT);
  Serial.println(state);
  delay(100);
}

