/*
* a sketch by Nicola Ariutt
* 2018/01/21
* This sketch will be useful as a feedback
* for a rotary encoder movement exposed by 
* and Arduino sketch.
*/


import processing.serial.*;
Serial s_port;
boolean bSerialListen;
PFont f;
Encoder e;


// SETUP ////////////////////////////////////////
void setup()
{
  size( 600, 600 );
  frameRate(60);
  smooth();
  
  f = createFont("Courier", 14);
  textFont(f);

  //println(Serial.list());
  s_port = new Serial(this, Serial.list()[3], 230400);
  bSerialListen = false;
  println(width*0.5);
  e = new Encoder( 128, width*0.5*0.75 ); //nSteps, radius
}

// DRAW /////////////////////////////////////////
void draw()
{
  background(0);
  
  if( bSerialListen)
    text("press 'c' to close serial communication", 10, 10);
  else
    text("press 'o' to open serial communication", 10, 10);
  
  e.display();
}

// SERIAL EVENT /////////////////////////////////
void serialEvent(Serial s)
{
  int b = s.read();
  if(bSerialListen) {
    if( b == 1 ) {
      e.select( +1 );
    }
    else if( b == 2 ){
      e.select( -1 );
    } 
  }
}


// KEYBOARD /////////////////////////////////////
void keyPressed()
{
  if (key == 'o' || key == 'O')
  {
    println("open");
    s_port.write('o');
    bSerialListen = true;
  }
  else if( key == 'c' || key == 'C')
  {
    println("close");
    s_port.write('c');
    bSerialListen = false;
  }
}