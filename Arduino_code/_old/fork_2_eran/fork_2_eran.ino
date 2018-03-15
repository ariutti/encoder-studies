/*
 * This sketch is inspired by the work of eran.io
 * http://eran.io/rotary-encoder-based-cooking-timer/
 * Here's code: https://github.com/pavius/rotary-encoder-timer
 */


// connect the encoder channelA to digital pin 4
// connect the encoder channelB to digital pin 5
#define CHA 2
#define CHB 3
#define NO_TRANSITION_EVENT (1)

// channel shift register
uint8_t statusChA = 0x00;
uint8_t statusChB = 0x00;

uint8_t searchPatternA = 0x00;
uint8_t searchPatternB = 0x00;

typedef enum {
  STATE_IDLE,
  STATE_DETECTING_CW,
  STATE_DETECTING_CCW,
} STATE;
STATE state = STATE_IDLE;

uint8_t transitionEvent = NO_TRANSITION_EVENT;
uint8_t transitionEventIndex = 0;

//
int8_t changeCounts = 0;

const uint8_t cwTransitions[]  = {8, 0, 15, 7};
const uint8_t ccwTransitions[] = {0, 8, 7, 15};


// SETUP ///////////////////////////////////////////////////////////////
void setup()
{
  Serial.begin(9600);
  // turn off the annoying LED
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
}

// LOOP ////////////////////////////////////////////////////////////////
void loop() 
{
  statusChA = ((statusChA<<1) | digitalRead(CHA)) & 0x07;
  statusChB = ((statusChB<<1) | digitalRead(CHB)) & 0x07;

  // debounce CH A
  if( statusChA == searchPatternA )
  {
    // change search pattern
    searchPatternA = (~searchPatternA) & 0x07;
    // append 0 to resutl
    transitionEvent = statusChA; // 0 or 7
  }

  // debounce CH B
  if( statusChB == searchPatternB )
  {
    // change search pattern
    searchPatternB = (~searchPatternB) & 0x07;
    // append 1 to result
    transitionEvent = (statusChB | (1<<3)); // 8 or 15
  }
 
  // Check pattern (if something has happend
  // the transition event is now 7, 15, 0 or 8.
  if( transitionEvent != NO_TRANSITION_EVENT )
  {
    if( state == STATE_IDLE )
    {
      // first value of a CW transition?
      if(transitionEvent == cwTransitions[0])
      {
        // yes! so we enter the corresponding state
        state = STATE_DETECTING_CW;
        // prepare for the next transition;
        transitionEventIndex++;
      }
      // first value of a CCW transition?
      else if(transitionEvent == ccwTransitions[0])
      {
        // yes! so we enter the corresponding state
        state = STATE_DETECTING_CCW;
        // prepare for the next transition;
        transitionEventIndex++;
      }
    }
    else if( state == STATE_DETECTING_CW )
    {
      if(transitionEvent == cwTransitions[transitionEventIndex])
      {
        // increment
        transitionEventIndex++;
        // have we finished?
        if(transitionEventIndex == 4)
        {
          // increment count
          changeCounts++;
          //back to idle
          state = STATE_IDLE;
          transitionEventIndex = 0;
        }
      }
      else
      {
        // this transition is incorrect, go back to idle
        state = STATE_IDLE;
        transitionEventIndex = 0;
      }
    }
    else if( state == STATE_DETECTING_CCW )
    {
      if(transitionEvent == ccwTransitions[transitionEventIndex])
      {
        // increment
        transitionEventIndex++;
        // have we finished?
        if(transitionEventIndex == 4)
        {
          // decrement count
          changeCounts--;
          //back to idle
          state = STATE_IDLE;
          transitionEventIndex = 0;
        }
      }
      else
      {
        // this transition is incorrect, go back to idle
        state = STATE_IDLE;
        transitionEventIndex = 0;
      } 
    }
    // we have already processed the transition occured so
    // we must zero out the transition.
    transitionEvent = NO_TRANSITION_EVENT;
  }
  
  Serial.println(changeCounts);
  
  delay(1); 
}
