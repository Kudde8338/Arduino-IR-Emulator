#include <IRremote.hpp>

int irSendPin = 2;
int IR_RECEIVE_PIN = 4;

int SAVE_BUTTON = 13;
int SEND_BUTTON = 10;

int address = 0x3000;
int command = 0x4F;
String protocol = "NEC";

void setup() {
  Serial.begin(9600);

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Startar receiver
  IrSender.begin(irSendPin, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN); // Startar Sändare

  pinMode(SAVE_BUTTON, INPUT);
  pinMode(SEND_BUTTON, INPUT);
}

void loop() {
  irReceive();
  irSend();

}

void irReceive() {
  if (digitalRead(SAVE_BUTTON) == HIGH) {
    if (IrReceiver.decode()) {
      IrReceiver.printIRResultShort(&Serial); // Skriver komplett mottagen data i en rad
      IrReceiver.printIRSendUsage(&Serial); // Skriver ut instruktion som krävs för att skicka motsvarande signal med IRTransmitter

      address = IrReceiver.decodedIRData.address;
      command = IrReceiver.decodedIRData.command;

      if (IrReceiver.decodedIRData.protocol == SONY) {
        protocol = "SONY";
      } else if (IrReceiver.decodedIRData.protocol == NEC) {
        protocol = "NEC";
      }

      Serial.println("Address: ");
      Serial.println(address, HEX);

      Serial.println("");
      Serial.println("Command: ");
      Serial.println(command, HEX);

      Serial.println("");
      Serial.println("Protocol: ");
      Serial.println(protocol);

      Serial.println("");

      IrReceiver.resume();
    }
  }
}

void irSend() {
  if (digitalRead(SEND_BUTTON) == HIGH) {
    if (protocol == "SONY") {
      IrSender.sendSony(address, command, 3);
    } else if (protocol == "NEC") {
      IrSender.sendNEC(address, command, 3);
    }
  }
}
