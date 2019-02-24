// Keyboard (sending USB HID reports) for Arduino Pro Micro
// Maarten Pennings 2018 oct 17


// Make sure that in Sketch > Include Library > Manage Libraries...
// You have installed 'HID-Project' by NicoHood (I used version 2.5.0)
#include "HID-Project.h"
#if USB_VERSION>0x200
#error USBCore.h file is patched (?)
#endif

// This sketch has an abstract notion of button indices (numbers, ids).
// Technically, these indices are used as index in BUT_PINS[] and reports[]. 
// The diagram below (left) shows the keyboard this sketch supports. 
// It has 10 buttons, each represented by a box. The top line in the box
// shows the button index (BUT0..BUT9). 
//
//   +------+------+------+              
//   | BUT0 | BUT1 | BUT2 |             +---------+
//   | PIN1 | PIN0 | PIN2 |            1|TX    RAW| 
//   | VOL- | MUTE | VOL+ |            0|RX    GND|
//   +------+------+------+             |GND   RST|
//   | BUT3 | BUT4 | BUT5 |             |GND   VCC|
//   | PIN3 | PIN4 | PIN5 |            2|SDA    A3|21
//   | PREV | PLAY | NEXT |            3|SCL    A2|20
//   +------+------+------+            4|A6     A1|19
//   | BUT6 | BUT7 | BUT8 |            5|       A0|18
//   | PIN6 | PIN7 | PIN8 |            6|A7   SCLK|15
//   | F22  | F23  | F24  |            7|     MISO|14
//   +------+------+------+            8|A8   MOSI|16
//          | BUT9 |                   9|A9    A10|10
//          | PIN9 |                    +---------+
//          |alt-F4|
//          +------+
//
// Each button is a physical switch, on one end connected to an arduino pin
// and on the other side connected to GND. A button press should close the
// button, e.g. connect the pin to ground.
//
// The array BUT_PINS[] needs to be configured to your liking, telling this
// sketch which button is connected to which pin. It is very likely you keep 
// the array as is, unless the number of buttons needs to change. But the 
// assignment is arbitrary. The diagram above lists the pin numbers this 
// sketch assigns to each button in the middle row (PIN0..PIN9). In case you 
// wonder why button with index 0 uses pin 1 and the button with index 1 
// uses pin 0, look at the order of the pin-out on the Pro Micro shown on 
// the right hand side (where 0 and 1 are swapped).
//
// Each button sends a so-called HID report. A HID report is a message from
// a keyboard (the Pro Micro in our case) to the PC, telling the PC that e.g.
// Ctrl and C were pressed together. The array reports[] needs to be
// configured to your liking, assigning reports to button indices. This
// is probably _the_ thing you want to change. See below near reports[]
// more details of what to fill out.


// BUT - button driver ---------------------------------------------------

// Configure BUT_PINS[]: list the pin numbers the buttons are connected to.
// All pins will be configured with internal pull-ups, so wire them such 
// that a press connects them to GND. 
// Number of pins is as many as you like and the CPU supports.
int BUT_PINS[]={1,0,2,3,4,5,6,7,8,9};

#define BUT_COUNT (int)(sizeof(BUT_PINS)/sizeof(int))
#define BUT(i)  (1<<(i))
#define BUT_ALL ((1<<(BUT_COUNT+1))-1)

int but_state_prev;
int but_state_cur;

void but_init(void) {
  for( int i=0; i<BUT_COUNT; i++ )
    pinMode(BUT_PINS[i],INPUT_PULLUP); 
  but_scan();
  but_scan();
}

void but_scan(void) {
  but_state_prev= but_state_cur;
  but_state_cur= 0;
  for( int i=0; i<BUT_COUNT; i++ )
    if( digitalRead(BUT_PINS[i])==0 ) but_state_cur |= 1<<i;
}

int but_changed( int mask ) {
  return (but_state_prev ^ but_state_cur) & mask;
}

int but_isdown( int mask ) {
  return but_state_cur & mask;
}

int but_isup( int mask ) {
  return ~but_state_cur & mask;
}

int but_wentdown( int mask ) {
  return but_changed(mask) & but_isdown(mask);
}

int but_wentup( int mask ) {
  return but_changed(mask) & but_isup(mask);
}


// REPORT - the reports with key codes to send -----------------------------

typedef enum report_src_e {
    REPORT_SRC_KBD, // The source of the code is 'keyboard', see ImprovedKeylayouts.h for codes
    REPORT_SRC_CON, // The source of the code is 'consumer', see ConsumerAPI.h for codes
} report_src_t;

typedef struct report_s {
  report_src_t src;
  int          code[5];
} report_t;

#define KBD1(code0)                         { REPORT_SRC_KBD, {code0,0,0,0,0} }
#define KBD2(code0,code1)                   { REPORT_SRC_KBD, {code0,code1,0,0,0} }
#define KBD3(code0,code1,code2)             { REPORT_SRC_KBD, {code0,code1,code2,0,0} }
#define KBD4(code0,code1,code2,code3)       { REPORT_SRC_KBD, {code0,code1,code2,code3,0} }
#define KBD5(code0,code1,code2,code3,code4) { REPORT_SRC_KBD, {code0,code1,code2,code3,code4} }
#define CON1(code0)                         { REPORT_SRC_CON, {code0,0,0,0,0} }

// Configure reports[]: for each key listed in BUT_PINS, define the
// so-called HID report that should be send to the PC. This sketch can
// send two kinds of HID reports, either a "keyboard" report
// or a "consumer" report. Keyboard reports contain normal keys (like,
// A, 3, F4, Return, Shift), consumer reports have special keys (think 
// remote control keys like volume up or play/pause).
// Note that keyboard reports can contain up to 5 keys, this is typically 
// needed when sending something like CTRL-ALT-SHIFT-C. For a full list of
// keyboard keys respectively consumer keys, see 
//   Arduino\libraries\HID\src\HID-APIs\ImprovedKeylayouts.h
//   Arduino\libraries\HID\src\HID-APIs\ConsumerAPI.h
// Some examples: KEY_A, KEY_0, KEY_F1, KEY_ENTER, KEY_COMMA, KEYPAD_ADD, KEY_RIGHT_ARROW, KEY_LEFT_ALT, KEY_COPY, KEY_PASTE
report_t reports[BUT_COUNT] = {
  CON1(MEDIA_VOLUME_DOWN),
  CON1(MEDIA_VOLUME_MUTE),
  CON1(MEDIA_VOLUME_UP),
  CON1(MEDIA_PREV),
  CON1(MEDIA_PLAY_PAUSE),
  CON1(MEDIA_NEXT),
  KBD1(KEY_F22),
  KBD1(KEY_F23),
  KBD1(KEY_F24),
  KBD2(KEY_LEFT_ALT,KEY_F4),
};

// LED - driver for the LEDs -----------------------------------------------

// Unfortunately, the ProMicro does not have independent LEDs.
// It has a power LED which can not be controlled, and it has LEDs connected
// to the serial TX and RX lines. 
// The Pro Micro library offers two macros to use these LEDS
// Note: RXLED is pin 17, but TX LED does not have a Arduino pin.
// Note: any activity on Serial port (print respectively read) will alter these LEDs.
// If you have another board, or soldered LEDs yourself to pins, change the LED macros.

#define LED_A_ON()  TXLED1
#define LED_A_OFF() TXLED0

#define LED_B_ON()  RXLED0
#define LED_B_OFF() RXLED1


// MAIN - setup and loop ---------------------------------------------------

void setup() {
  but_init();

  Consumer.begin();
  Keyboard.begin();

  // Flicker LED to show booted
  LED_B_OFF();
  for( int i=0; i<10; i++) { 
    LED_A_ON(); delay(100); LED_A_OFF(); delay(100);
  }
}

void loop() {
  // Take snapshot of the button states
  but_scan();

  // Switch LED on when there was a change in any of the button states
  if( but_changed(BUT_ALL) ) LED_A_ON();
  
  // Loop over each button, send press or release reports
  // (for each report send, blink TXLED)
  for( int bix=0; bix<BUT_COUNT; bix++ ) {
    if( reports[bix].src==REPORT_SRC_KBD ) {
      if( but_wentdown(BUT(bix)) ) {
        for(int cix=0; cix<5 && reports[bix].code[cix]!=0; cix++ ) 
          Keyboard.press( (KeyboardKeycode) reports[bix].code[cix] );
      }
      if( but_wentup(BUT(bix)) ) {
        for(int cix=0; cix<5 && reports[bix].code[cix]!=0; cix++ ) 
          Keyboard.release( (KeyboardKeycode) reports[bix].code[cix] );
      }
    } else if( reports[bix].src==REPORT_SRC_CON ) {
      if( but_wentdown(BUT(bix)) ) {
        Consumer.press( (ConsumerKeycode) reports[bix].code[0] );
      }
      if( but_wentup(BUT(bix)) ) {
        Consumer.release( (ConsumerKeycode) reports[bix].code[0] );
      }
    }
  }

  // Switch LED off (it might be off still)
  LED_A_OFF();

  // Simple debounce
  delay(50);
}

