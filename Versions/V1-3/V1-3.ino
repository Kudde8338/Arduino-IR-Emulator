// Includerar script för att
#include <IRremote.hpp>


// Definitioner
// Definiera pinnar för vart sändaren och vart mottagaren sitter på Arduino
int irSendPin = 2;
int IR_RECEIVE_PIN = 4;

// Definiera de olika knapparna:
// Knapp för att ändra mellan läge receive och send
int MODE_BUTTON = 13;

// Knappar för att välja save slot
int SAVE1_BUTTON = 10;
int SAVE2_BUTTON = 11;
int SAVE3_BUTTON = 12;

// Initiera de tre olika kommandon som kan sparas
// Kommando ett
int addressOne = 0x3000;
int commandOne = 0x4F;
String protocolOne = "NEC";

// Kommando två
int addressTwo = 0x3000;
int commandTwo = 0x4F;
String protocolTwo = "NEC";

// kommando tre
int addressThree = 0x3000;
int commandThree = 0x4F;
String protocolThree = "NEC";

// Definiera vilket läge Arduinon är i; "S" = Send, "R" = Receive
String mode = "S";


void setup() {
  Serial.begin(9600); // Initiera en port för att skicka debugging till

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK); // Startar receiver
  IrSender.begin(irSendPin, ENABLE_LED_FEEDBACK, USE_DEFAULT_FEEDBACK_LED_PIN); // Startar sändare

  // Initiera alla knappar som input
  pinMode(MODE_BUTTON, INPUT); // Lägesändraren
  pinMode(SAVE1_BUTTON, INPUT); // Save1
  pinMode(SAVE2_BUTTON, INPUT); // Save2
  pinMode(SAVE3_BUTTON, INPUT); // Save3
}

// Programmet som körs om och om igen
void loop() {
  if ((digitalRead(MODE_BUTTON) == HIGH) && (mode == "S")) {
    mode = "R";
    delay(200);
  } else if ((digitalRead(MODE_BUTTON) == HIGH) && (mode == "R")) {
    mode = "S";
    delay(200);
  } else if (mode == "S") {
    irSend();
  } else if (mode == "R") {
    irReceive();
  }
}

void irReceive() {
  if (IrReceiver.decode()) {
// Avkommentera för debug:
//    debug(IrReceiver.decodedIRData);'

    if (IrReceiver.decodedIRData.protocol == SONY || IrReceiver.decodedIRData.protocol == NEC) {
      irSave(IrReceiver.decodedIRData);
    }
    IrReceiver.resume();
  }
}

void irSave(IRData data) {
  while ((digitalRead(SAVE1_BUTTON == LOW)) && (digitalRead(SAVE2_BUTTON) == LOW) && (digitalRead(SAVE3_BUTTON) == LOW)) {
    delay(50);
  }
  if (digitalRead(SAVE1_BUTTON) == HIGH) {
    addressOne = data.address;
    commandOne = data.command;
    protocolOne = (data.protocol == SONY) ? "SONY" : "NEC";

  } else if (digitalRead(SAVE2_BUTTON) == HIGH) {
    addressTwo = data.address;
    commandTwo = data.command;
    protocolTwo = (data.protocol == SONY) ? "SONY" : "NEC";

  } else if (digitalRead(SAVE3_BUTTON) == HIGH) {
    addressThree = data.address;
    commandThree = data.command;
    protocolThree = (data.protocol == SONY) ? "SONY" : "NEC";
  }

    IrReceiver.resume();
}

void irSend() {
  if (digitalRead(SAVE1_BUTTON) == HIGH) {
    if (protocolOne == "SONY") {
      IrSender.sendSony(addressOne, commandOne, 3);
    } else if (protocolOne == "NEC") {
      IrSender.sendNEC(addressOne, commandOne, 3);
    }
  } else if (digitalRead(SAVE2_BUTTON) == HIGH) {
    if (protocolTwo == "SONY") {
      IrSender.sendSony(addressTwo, commandTwo, 3);
    } else if (protocolTwo == "NEC") {
      IrSender.sendNEC(addressTwo, commandTwo, 3);
    }
  } else if (digitalRead(SAVE3_BUTTON) == HIGH) {
    if (protocolThree == "SONY") {
      IrSender.sendSony(addressThree, commandThree, 3);
    } else if (protocolThree == "NEC") {
      IrSender.sendNEC(addressThree, commandThree, 3);
    }
  }
}

void debug(IRData data) {
  IrReceiver.printIRResultShort(&Serial); // Skriver komplett mottagen data i en rad
  IrReceiver.printIRSendUsage(&Serial); // Skriver ut instruktion som krävs för att skicka motsvarande signal med IRTransmitter
  Serial.println("Address: ");
  Serial.println(data.address, HEX);
  Serial.println("");
  Serial.println("Command: ");
  Serial.println(data.command, HEX);
  Serial.println("");
  Serial.println("Protocol: ");
  Serial.println(data.protocol);
  Serial.println("");
}
