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
const byte rowPins[ROWS] = {13, 12, 11, 10}; //connect to the row pinouts of the keypad
const byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad

// create keypad object
const Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

// declare variable to store latest keypress
char key;

// declare variables for tracking t9 multipresses
bool numeric; // tracks whether to use numeric keyboard mode
bool typeKey; // tracks whether the current keypress should be visually typed
char t9key;
uint8_t multipress[2] = {17, 0}; // {key #, press count}

//****************END KEYPAD****************//

//****************START LCD****************//
// define LCD pins
LiquidCrystal lcd(14, 15, 16, 17, 18, 19);

// declare variable tracking cursor position
uint8_t cursor = 1;
//****************END LCD****************//

void updateMultipress(uint8_t keyNumber) {
  uint8_t pressCount;

  if (multipress[0] == keyNumber) {
    // if this key was pressed previously, increment pressCount
    pressCount = multipress[1]+1;

    // if pressCount exceeds 3, reset it to 1
    if (pressCount > 3) {
      pressCount = 1;
    }

  } else {
    // if this key was not pressed previously, reset pressCount
    pressCount = 1;
  }
  // update multipress
  multipress[0] = keyNumber;
  multipress[1] = pressCount;
}

char calcT9(uint8_t keyNum, uint8_t pressCount, bool numeric) {
  char trio[3];

  // translate key # to letters
  switch (keyNum) {
  case 1:
    if (numeric) {
      return '1';
    }
    trio[0] = 'A';
    trio[1] = 'B';
    trio[2] = 'C';
    break;
  case 2:
    if (numeric) {
      return '2';
    }
    trio[0] = 'D';
    trio[1] = 'E';
    trio[2] = 'F';
    break;
  case 3:
    if (numeric) {
      return '3';
    }
    trio[0] = 'G';
    trio[1] = 'H';
    trio[2] = 'I';
    break;
  case 5:
    if (numeric) {
      return '4';
    }
    trio[0] = 'J';
    trio[1] = 'K';
    trio[2] = 'L';
    break;
  case 6:
    if (numeric) {
      return '5';
    }
    trio[0] = 'M';
    trio[1] = 'N';
    trio[2] = 'O';
    break;
  case 7:
    if (numeric) {
      return '6';
    }
    trio[0] = 'P';
    trio[1] = 'Q';
    trio[2] = 'R';
    break;
  case 9:
    if (numeric) {
      return '7';
    }
    trio[0] = 'S';
    trio[1] = 'T';
    trio[2] = 'U';
    break;
  case 10:
    if (numeric) {
      return '8';
    }
    trio[0] = 'V';
    trio[1] = 'W';
    trio[2] = 'X';
    break;
  case 11:
    if (numeric) {
      return '9';
    }
    trio[0] = 'Y';
    trio[1] = 'Z';
    trio[2] = ',';
    break;
  case 14:
    if (numeric) {
      return '0';
    }
    trio[0] = '.';
    trio[1] = '!';
    trio[2] = '?';
    break;
  }
    
  return trio[pressCount-1];
}

void setup(){
  // enable serial console
   Serial.begin(9600);

  // define LCD columns and rows (and clear it)
  lcd.begin(16, 2);
  lcd.clear();
}
  
void loop(){
  // always listen for keypresses
  key = keypad.getKey();

  // actions to perform when a key is pressed
  if (key) {
    typeKey = false;

    switch (key) {
    case 'A': // A, B, C | 1
      updateMultipress(1);
      break;
    case 'D': // D, E, F | 2
      updateMultipress(2);
      break;
    case 'G': // G, H, I | 3
      updateMultipress(3);
      break;
    case 'a': // alphabetical keyboard
      numeric = false;
      break;
    case 'J': // J, K, L | 4
      updateMultipress(5);
      break;
    case 'M': // M, N, O | 5
      updateMultipress(6);
      break;
    case 'P': // P, Q, R | 6
      updateMultipress(7);
      break;
    case 'n': // numeric keyboard
      numeric = true;
      break;
    case 'S': // S, T, U | 7
      updateMultipress(9);
      break;
    case 'V': // V, W, X | 8
      updateMultipress(10);
      break;
    case 'Y': // Y, Z, "," | 9
      updateMultipress(11);
      break;
    case '^': // capital shift
      break;
    case '@': // check received
      break;
    case '.': // ., !, ? | 0
      updateMultipress(14);
      break;
    case '>': // send message
      lcd.clear(); // clear screen and reset cursor position
      break;
    case ' ': // confirm t9 character | space (if last character was confirmed) | backspace (if capital shifted)
      typeKey = true;
      break;                                                                              
    }

    if (typeKey) {
      t9key = calcT9(multipress[0], multipress[1], numeric);
      multipress[1] = 0;

      // print t9key at cursor location
      if (cursor == 17) {
        lcd.setCursor(0, 1);
      }
      lcd.print(t9key);
      cursor++;
    }

  }
}
