#define LED 4                   // Led pin set
#define PULSE_TIME 560          // Delay for the zero and one HIGH (Microseconds)
#define ZERO_LOW_DELAY 560      // Delay for the zeros LOW (Microseconds)
#define ONE_LOW_DELAY 1690      // Delay for the ones LOW (Microseconds)
#define BEGIN_PULSE_HIGH 9000   // Pulse Burst in the start of the message (Mircroseconds)
#define BEGIN_PULSE_LOW 4500    // Pulse Space before the output of the message (Mircroseconds)


//================================ Prototypes ===================================
/*
 * Outputs a logical one to the LED according the the delay times and pulse times
 * in the defines at the top of this function
 */
void outputOnePulse();
/*
 * Outputs a logical zero to the LED according the the delay times and pulse times
 * in the defines at the top of this function
 */
void outputZeroPulse();
/*
 * Outputs the ending pulse, a pulse of HIGH for the pulse time 
 */
 void outputEndPulse();
/*
 * Begins the message with the long HIGH and long LOW followed by the five logical
 * zeros
 */
void beginMessage();

//============================== Lifecycle Functions =============================

void setup() 
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.begin(9600);
}

void loop()
{
  // Message to be output
  const bool message[] = {1,0 ,0};

  Serial.println("Begining Message Transmittion");
  beginMessage();
  for(int i = 0; i < 3; ++i)
  {
    (message[i]) ? outputOnePulse() : outputZeroPulse();
  }
  outputEndPulse();
  
  Serial.println("Message has been sent");
  delay(5000);
}

//============================= Function Definitions ==============================
void outputZeroPulse()
{    
  digitalWrite(LED, HIGH);
  delayMicroseconds(PULSE_TIME);
  digitalWrite(LED, LOW);
  delayMicroseconds(ZERO_LOW_DELAY);
}

void outputOnePulse()
{
  digitalWrite(LED, HIGH);
  delayMicroseconds(PULSE_TIME);
  digitalWrite(LED, LOW);
  delayMicroseconds(ONE_LOW_DELAY);
}

void outputEndPulse()
{
  digitalWrite(LED, HIGH);
  delayMicroseconds(PULSE_TIME);
  digitalWrite(LED, LOW);
}

void beginMessage()
{
  // Ouput the long high and the long low for the 
  // beggining of the pulse
  digitalWrite(LED, HIGH);
  delayMicroseconds(BEGIN_PULSE_HIGH);
  digitalWrite(LED, LOW);
  delayMicroseconds(BEGIN_PULSE_LOW);

  // Ouput the five zero pulses
  for(int i = 0; i < 5; ++i)
  {
    outputZeroPulse();
  }
}
