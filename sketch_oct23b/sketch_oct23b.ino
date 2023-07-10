#include <IRremote.h> // Include the IRremote library

int RECV_PIN = 11;
IRrecv irrecv(RECV_PIN);
// This will store our IR received codes
decode_results results; 
IRsend irsend; // Enables IR transmit on pin 3

/* Storage variables for the received IR code: */
unsigned int irLen = 0;
unsigned int irBuffer[RAWBUF]; // Storage for raw (UNKNOWN) codes
int codeType; // The type of code (NEC, SONY, UNKNOWN, etc.)
unsigned int codeValue; 
boolean codeReceived = false; 
const int buttonPin = 12; // PULL UP

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); 

  pinMode(buttonPin, INPUT_PULLUP);
}

void loop() {
  if (irrecv.decode(&results)) 
  {
    updateIRReceive(); // Sort out the received code
    codeReceived = true; // Enables code transmit on button press
    irrecv.resume(); // re-enable receive
  }
  
 // If the button is pressed, and we've received a code...
  if ((digitalRead(buttonPin) == LOW) && (codeReceived == true))
  {
    sendCode(); // Sends out our code. (See bottom of sketch).
    irrecv.enableIRIn(); // Re-enable receiver
    // We only want to be able to send the code once. Re-set
    // codeReceived to false:
    codeReceived = false;
}

// ========================== UPDATE IR Rx =======================

void updateIRReceive()
{
  codeType = results.decode_type;
  irLen = results.rawlen;
  if (codeType == UNKNOWN)
  {
    irLen--;
    // We need to convert from ticks to microseconds
    for (int i = 1; i <= irLen; i++) 
    {
      if (i % 2) 
      {
        // Mark
        irBuffer[i-1] = results.rawbuf[i]*USECPERTICK - MARK_EXCESS;
        Serial.print(" m");
      } 
      else 
      {
        // Space
        irBuffer[i-1] = results.rawbuf[i]*USECPERTICK + MARK_EXCESS;
        Serial.print(" s");
      }
      Serial.print(irBuffer[i-1], DEC);
    }
    Serial.println();
  }
  else 
  {
    if (codeType == NEC) 
    {
      Serial.print("Received NEC: ");
      if (results.value == REPEAT) 
      {
        // Don't record a NEC repeat value as that's useless.
        Serial.println("repeat; ignoring.");
        return;
      }
    } 
    else if (codeType == SONY) 
    {
      Serial.print("Received SONY: ");
    } 
    else if (codeType == RC5) 
    {
      Serial.print("Received RC5: ");
    } 
    else if (codeType == RC6) 
    {
      Serial.print("Received RC6: ");
    } 
    else 
    {
      Serial.print("Unexpected codeType ");
      Serial.print(codeType, DEC);
      Serial.println("");
    }
    Serial.println(results.value, HEX);
    codeValue = results.value;
    irLen = results.bits;
  }
}

// ========================= TRANSMIT CODE ============================

void sendCode()
{
  if (codeType == NEC) 
  {
    irsend.sendNEC(codeValue, irLen);
    Serial.print("Sent NEC ");
    Serial.println(codeValue, HEX);
  } 
  else if (codeType == SONY)
  {
    irsend.sendSony(codeValue, irLen);
    Serial.print("Sent Sony ");
    Serial.println(codeValue, HEX);
  } 
  else if (codeType == RC5 || codeType == RC6) 
  {
    // Put the toggle bit into the code to send
    codeValue = codeValue & ~(1 << (irLen - 1));
    if (codeType == RC5) 
    {
      Serial.print("Sent RC5 ");
      Serial.println(codeValue, HEX);
      irsend.sendRC5(codeValue, irLen);
    } 
    else 
    {
      //irsend.sendRC6(codeValue, irLen);
      Serial.print("Sent RC6 ");
      Serial.println(codeValue, HEX);
    }
  } 
  else if (codeType == UNKNOWN /* i.e. raw */) 
  {
    // Assume 38 KHz
    irsend.sendRaw(irBuffer, irLen, 38);
    Serial.println("Sent raw");
  }
}
