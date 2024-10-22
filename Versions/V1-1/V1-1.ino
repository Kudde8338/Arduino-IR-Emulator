

// Imports
#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>


// Define constants
#define IR_RECEIVE_PIN 8
#define STATUS_LED 4
#define BUTTON_RECEIVE 5
#define BUTTON_1 7


// save_slot_1 = 

IRsend irsend;

bool receivemode = 0;

struct storedIRDataStruct {
    IRData receivedIRData;
    // extensions for sendRaw
    uint8_t rawCode[RAW_BUFFER_LENGTH]; // The durations if raw
    uint8_t rawCodeLength; // The length of the code
} sStoredIRData;

// sStoredIRData.receivedIRData = IrReceiver.decodedIRData;

void setup() {
  pinMode(STATUS_LED, OUTPUT);
  pinMode(BUTTON_RECEIVE, INPUT);

  Serial.begin(115200); // Baud rate for serial communication
  while (!Serial) {}; // Wait for serial to become available
  Serial.println("Ready!");

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver.
  IrSender.begin();
}

void loop() {

  // put your main code here, to run repeatedly:
  if (digitalRead(BUTTON_1) == HIGH) {
    Serial.println("Send buttom pressed!");
    sendReceived(sStoredIRData);
  }

  if (!receivemode && (digitalRead(BUTTON_RECEIVE) == HIGH)) {
    receivemode = 1;
    Serial.println("Receive mode!");
    digitalWrite(STATUS_LED, HIGH);
    IrReceiver.resume();

  }

  if (receivemode && (IrReceiver.decode())) {
    storeReceived(sStoredIRData.receivedIRData, IrReceiver.decodedIRData);
    receivemode = 0;
    delay(1000);
  }

}

void storeReceived(IRData receivedData, IRData decodedData) {
//  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Start the receiver.
  Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); // Print "old" raw data
  IrReceiver.printIRResultShort(&Serial); // Print complete received data in one line
  IrReceiver.printIRSendUsage(&Serial);   // Print the statement required to send this data
  
  receivedData = decodedData;
}

void sendReceived(storedIRDataStruct storedData) {
  irsend.sendRaw(storedData.rawCode, storedData.rawCodeLength, 38);
  Serial.println("Sent!");
  delay(1000);
}
