#include <IRremote.h>
const int RECV_PIN = 11;
const int SPEAKER_PIN = 12;
//const int LED_PIN = 4;
IRrecv irrecv(RECV_PIN);
decode_results results;
int state = 0; //off
IRsend irsend;


void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(SPEAKER_PIN,OUTPUT);
  Serial.println(F("Send a code from your remote and we will record it."));
  pinMode(RECV_PIN,OUTPUT);
}

void loop(){
  if (irrecv.decode(&results)){
      Serial.println("New Msg: ");
      Serial.print("0x   ");
      Serial.println(results.value, HEX);
      irrecv.resume();
      tone(SPEAKER_PIN, 200);
      delay(100);
      noTone(SPEAKER_PIN);
  }
//  irsend.sendNEC(0x000000,32); 
//  delay(100);
//  irrecv.enableIRIn();
}

//void transmit()
//{
//  int i = 0;
//  while(i < 3){
//    digitalWrite(IR_PIN, HIGH);
//    delayMicroseconds(11);
//    digitalWrite(IR_PIN, LOW);
//    delayMicroseconds(11);
//    i++;
//  }
//}
