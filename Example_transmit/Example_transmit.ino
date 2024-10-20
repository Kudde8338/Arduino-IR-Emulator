#include <Arduino.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>
// #define IR_SEND_PIN 7
// #define IR_TRANSMIT_PIN 7

IRsend irsend;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.begin(115200);
    while (!Serial)
        ; // Wait for Serial to become available. Is optimized away for some cores.

    // Just to know which program is running on my Arduino
    Serial.println(F("START " __FILE__ " from " __DATE__ "\r\nUsing library version " VERSION_IRREMOTE));
    Serial.print(F("Send IR signals at pin "));
    Serial.println(IR_SEND_PIN);

    /*
     * The IR library setup. That's all!
     */
    IrSender.begin(); // Start with IR_SEND_PIN -which is defined in PinDefinitionsAndMore.h- as send pin and enable feedback LED at default feedback LED pin
    disableLEDFeedback(); // Disable feedback LED at default feedback LED pin
}

void loop() {
  // put your main code here, to run repeatedly:
  irsend.sendSony(0x1, 0x15, 2, 12);
  Serial.print("SENT ");
  delay(5000);


}
