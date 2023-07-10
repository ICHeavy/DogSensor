#include <IRLibDecodeBase.h>  //We need both the coding and
#include <IRLibSendBase.h>    // sending base classes
#include <IRLib_P01_NEC.h>    //Lowest numbered protocol 1st
#include <IRLib_HashRaw.h>    //We need this for IRsendRaw
#include <IRLibCombo.h>       // After all protocols, include this
#include <IRLibRecv.h>

IRdecode myDecoder;
IRsend mySender;
IRsend sender;

IRrecv myReceiver(2); //pin number for the receiver

const int LED = 9;
uint8_t codeProtocol;  // The type of code
uint32_t codeValue;    // The data bits if type is not raw
uint32_t constValue = 0x811C9DC5;
uint8_t codeBits;      // The length of the code in bits

//These flags keep track of whether we received the first code 
//and if we have have received a new different code from a previous one.
bool Rx, Tx; 


void setup() {
  codeProtocol=UNKNOWN; 
  codeValue=0;
  pinMode(LED, OUTPUT);
  pinMode(11,OUTPUT);
  Serial.begin(9600);
  delay(2000);
  while(!Serial);//delay for Leonardo
  Serial.println(F("Send a code from your remote and we will record it."));
  Serial.println(F("Type any character and press enter. We will send the recorded code."));
  Serial.println(F("Type 'r' special repeat sequence."));
  myReceiver.enableIRIn(); // Start the receiver
}


void sendCode(void) {
//  codeValue = (uint32_t) & (recvGlobal.decodeBuffer[1]);
  codeBits = recvGlobal.decodeLength - 1;
  Serial.println(F("Sent raw"));
  mySender.send(codeProtocol,codeValue,codeBits);
}


void LEDguy(void){
  digitalWrite(LED,HIGH);
  delay(250);
  digitalWrite(LED,LOW);
//  tone(11, 500);
//  delay(300);
//  noTone(11);

}
void storeCode(void) {
  codeProtocol = myDecoder.protocolNum;
  myDecoder.dumpResults(false);
  codeValue = myDecoder.value;
}


void loop(){ 
    sendCode();
    myReceiver.enableIRIn(); // Re-enable receiver
    delay(400);
    
    if (myReceiver.getResults()){
      myDecoder.decode();
      if (myDecoder.value == constValue){
        myReceiver.disableIRIn();
        LEDguy();
        delay(100);
        myReceiver.enableIRIn(); // Re-enable receiver
        delay(400);
        myDecoder.value = 0;
        }
    }
  }
