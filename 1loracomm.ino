#include <DES.h>
#include <Keypad.h>
#include <LiquidCrystal.h>
#include <SoftwareSerial.h>

//****************START USER SETTINGS****************//
// configure 3DES
DES des;
byte desKey[] = { // PLACEHOLDER; PLEASE CHANGE ME!!!
              0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e, // key A
              0x92, 0x2f, 0xb5, 0x1F, 0xc7, 0x1f, 0x43, 0x6e, // key B
              0x3b, 0x38, 0x98, 0x37, 0x15, 0x20, 0xf7, 0x5e, // key C
            };
//****************END USER SETTINGS****************//

//****************START KEYPAD****************//
// define keypad matrix
const byte ROWS = 4; // four rows
const byte COLS = 4; // four columns
const char keys[ROWS][COLS] = {
  {'A','D','G','a'},
  {'J','M','P','n'},
  {'S','V','Y','^'},
  {'@','.','>',' '}
};

// define keypad pins
const byte rowPins[ROWS] = {13, 12, 11, 10}; // connect to the row pinouts of the keypad
const byte colPins[COLS] = {9, 8, 7, 6}; // connect to the column pinouts of the keypad

// create keypad object
const Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// declare variable to store latest keypress
char key;

// declare variables for tracking t9 multipresses
bool capitalShift; // tracks whether the shift key was pressed
bool numeric; // tracks whether to use numeric keyboard mode
bool typeKey; // tracks whether the current keypress should be visually typed
char t9key;
uint8_t multipress[2] = {17, 0}; // {key #, press count}

//****************END KEYPAD****************//

//****************START LCD****************//
// define LCD pins
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);

// declare variable tracking cursor position
uint8_t cursor = 1; // set to 1 for easy non-zero based position tracking
//****************END LCD****************//

//****************START LORA****************//
// declare message buffer
unsigned char messageBuffer[33];

// configure software serial
const byte SloraRX = 2;
const byte SloraTX = 3;
SoftwareSerial Slora = SoftwareSerial(SloraRX, SloraTX);

// declare misc. variables/constants
uint8_t peerAddress;
const uint8_t LED = 4;
//****************END LORA****************//
