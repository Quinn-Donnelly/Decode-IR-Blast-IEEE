/**
 * Blink
 *
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 */
#include "Arduino.h"
#define interruptPin 3
#define systemTime micros()
const unsigned int ONE_DOWN_TIME = 1125;

volatile unsigned long time;        // Stores the systemTime return value
volatile unsigned long downTime;     // Stores the calculated down time from interupts
unsigned int currentState;          // Value for tracking the state in the loop state machine
unsigned int countOfBitsRead;      // Tracks the current number of bits that have been processed in the message

//=========================== Prototypes =======================================

/**
 * This is the interupt call function for the FALLING of the pin.
 * On call it will set the time to the current systemTime function and attach the
 * onUp function to interupt on RISING
 */
void onDown();
/**
 * This is the interupt call function for the RISING of the pin.
 * On call it will set the downTime to be the time since the falling of the pin
 */
void onUp();
/**
 * This is the function that will calculate the downTime of the burst by firing
 * on the CHANGE interupt and checking for HIGH or LOW on the pin and storing
 * the system time in using systemTime
 */
void onChange();

//=========================== Lifecycle Functions ==============================

void setup()
{
  // Attach to start listening for the Falling of the long burst
  attachInterrupt(digitalPinToInterrupt(interruptPin), onChange, CHANGE);
  time = 0;
  currentState = 0;
  countOfBitsRead = 0;
  Serial.begin(9600);
}

void loop()
{
  static bool message[] = {0,0,0};

  /**
   * This is a state machine that will handle parsing the message's waveform
   * @param currentState The state of the machine where each state is described
   *                     below
   */
  switch (currentState)
  {
    // Handles waiting until after the space of 4.5ms following the inital burst
    case 0:
      if(downTime > 3000 && downTime != 0)
      {
        currentState = 1;
        downTime = 0;
      }
      break;

    // Handles the first five logical zeros that are sent in the message
    case 1:
      if(downTime != 0 && downTime < ONE_DOWN_TIME)
      {
        ++countOfBitsRead;
        downTime = 0;
      }

      if(countOfBitsRead >= 5)
      {
        currentState = 2;
      }
      break;

    // Handles Reading the actual message being sent
    case 2:
      if(downTime != 0 && downTime < ONE_DOWN_TIME)
      {
        message[countOfBitsRead-5] = 0;
        downTime = 0;
        ++countOfBitsRead;
      } else if(downTime != 0){
        message[countOfBitsRead-5] = 1;
        downTime = 0;
        ++countOfBitsRead;
      }

      if(countOfBitsRead >= 8)
      {
        currentState = 3;
      }
      break;

    // Handles the ending of the pulse and resets the variables
    case 3:
      if(downTime > 9000)
      {
          currentState = 0;
          countOfBitsRead = 0;
          downTime = 0;
          // Zeroing the message for testing purposes
          Serial.print("Final Message: ");
          for(int i = 0; i < 3; ++i)
          {
            if(message[i])
              Serial.print(1);
            else
              Serial.print(0);


            message[i] = false;
          }
          Serial.println();
      }
      break;
  }
}

//=========================== Function Definitions =============================

void onDown()
{
  time = systemTime;
  downTime = 0;
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  attachInterrupt(digitalPinToInterrupt(interruptPin), onUp,  RISING);
}

void onUp()
{
  downTime = systemTime - time;
  detachInterrupt(digitalPinToInterrupt(interruptPin));
  attachInterrupt(digitalPinToInterrupt(interruptPin), onDown, FALLING);
}

void onChange()
{
  if(digitalRead(interruptPin)) {
    downTime = systemTime - time;
    Serial.println(downTime);
  } else {
    time = systemTime;
  }
}
