#include <IRLibRecvPCI.h> 
const int SPK = 8;

IRrecvPCI myReceiver(2); //pin number for the receiver


void setup() {
Serial.begin(9600);
while (!Serial); //delay for Leonardo
myReceiver.enableIRIn(); // Start the receiver
Serial.println(F("Ready to receive IR signals"));
}

void loop() {
  if (myReceiver.getResults()) {
    tuner();
    Serial.println("HIT!");
    Serial.print(F("\n#define RAW_DATA_LEN "));
    Serial.println(recvGlobal.recvLength,DEC);
    Serial.print(F("uint16_t rawData[RAW_DATA_LEN]={\n\t"));
    for(bufIndex_t i=1;i<recvGlobal.recvLength;i++) {
      Serial.print(recvGlobal.recvBuffer[i],DEC);
      Serial.print(F(", "));
      if( (i % 8)==0) Serial.print(F("\n\t"));
    }
    Serial.println(F("1000};"));//Add arbitrary trailing space
    myReceiver.enableIRIn();      //Restart receiver
  }
}

void tuner(){
  tone(SPK, 800);
  delay(1000);
  noTone(SPK);
}
