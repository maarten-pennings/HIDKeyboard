// rp2040try.ino - keyboard test for RP2040 
// I have a board with an LED on LED_BUILTIN and a button on pin 23
// NOTE if the RP2040 has micropython, bring it in USB mode first 
// (press BOOT button, then either power up by plugging uSB or press RESET).
// then flash (Tools>Upload method UF2). It is then always in USB mode, but
// select Port> COMx, see https://www.upesy.com/blogs/tutorials/install-raspberry-pi-pico-on-arduino-ide-software#
#include "Keyboard.h"

#define BUT_KEY 23
int     but_last;

void setup() {
  Serial.begin(115200); while(!Serial) delay(100);
  Serial.printf("\nrp2040try: press KEY to send ALT-R to PC over USB\n\n");
  // Init the LED.
  pinMode(LED_BUILTIN, OUTPUT );
  digitalWrite(LED_BUILTIN, LOW );
  // Init the button.
  pinMode(BUT_KEY, INPUT);
  but_last = 0;
  // Init HID
  Keyboard.begin();
}


void loop() {
  int but = digitalRead(BUT_KEY);
  if( but!=but_last ) {
    digitalWrite(LED_BUILTIN, but );
    but_last=but;
    if( but==1 ) {
      Serial.printf("press ... ");
      Keyboard.press(KEY_LEFT_ALT); // sends 04 00, 00 00 00 00 00 00
      delay(10);                    // must have some delay
      Keyboard.press('r');          // sends 04 00, 15 00 00 00 00 00
      delay(10);
    } else {
      Keyboard.releaseAll();
      Serial.printf("released\n");
    }
  }
}

// I added
// Serial.printf("rep= %02x %02x :",_keyReport.modifiers,_keyReport.reserved); for(int i =0;i<6; i++) Serial.printf(" %02x",_keyReport.keys[i]);Serial.printf("\n");
// to line 95 of
// C:\Users\maarten\AppData\Local\Arduino15\packages\rp2040\hardware\rp2040\3.2.0\libraries\HID_Keyboard\src\HID_Keyboard.cpp
