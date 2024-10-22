#include <Keypad.h>
#include <LiquidCrystal.h>

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
char messageBuffer[33];

uint8_t peerAddress;
const uint8_t LED = 4;
//****************END LORA****************//