#include <Keypad.h>
#include <LiquidCrystal.h>

// OVERALL TODO
// Add visual cursor
// Send/receive LoRa messages
// Allow sending text received over serial (from computer if beginning with \x1f)

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

void backspace() {
  uint8_t cursorX;
  uint8_t cursorY;

  cursor--;
  cursorX = cursor;
  if (cursorX > 16) {
     cursorY = 1;
     cursorX = cursorX-16;
  } else {
    cursorY = 0;
  }

  // set cursor to previous position (subtract one since cursor is numbered from 1)
  lcd.setCursor(cursorX-1, cursorY);
  // print a space
  lcd.print(' ');
  // set cursor back to the newly blank space
  lcd.setCursor(cursorX-1, cursorY);
}

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
    trio[0] = 'a';
    trio[1] = 'b';
    trio[2] = 'c';
    break;
  case 2:
    if (numeric) {
      return '2';
    }
    trio[0] = 'd';
    trio[1] = 'e';
    trio[2] = 'f';
    break;
  case 3:
    if (numeric) {
      return '3';
    }
    trio[0] = 'g';
    trio[1] = 'h';
    trio[2] = 'i';
    break;
  case 5:
    if (numeric) {
      return '4';
    }
    trio[0] = 'j';
    trio[1] = 'k';
    trio[2] = 'l';
    break;
  case 6:
    if (numeric) {
      return '5';
    }
    trio[0] = 'm';
    trio[1] = 'n';
    trio[2] = 'o';
    break;
  case 7:
    if (numeric) {
      return '6';
    }
    trio[0] = 'p';
    trio[1] = 'q';
    trio[2] = 'r';
    break;
  case 9:
    if (numeric) {
      return '7';
    }
    trio[0] = 's';
    trio[1] = 't';
    trio[2] = 'u';
    break;
  case 10:
    if (numeric) {
      return '8';
    }
    trio[0] = 'v';
    trio[1] = 'w';
    trio[2] = 'x';
    break;
  case 11:
    if (numeric) {
      return '9';
    }
    trio[0] = 'y';
    trio[1] = 'z';
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
  case 16:
    trio[0] = ' '; // rest of trio does not need to be defined (once multipress exceeds 1 for a space, it is printed and multipress is reset)
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
      capitalShift = true;
      break;
    case '@': // check received
      break;
    case '.': // ., !, ? | 0
      updateMultipress(14);
      break;
    case '>': // send message
      lcd.clear(); // clear screen and reset cursor position
      cursor = 1;
      break;
    case ' ': // confirm t9 character | space (if last character was confirmed) | backspace (if capital shifted)
      if (multipress[1] == 0) {
        // if last character was confirmed, send a space
        updateMultipress(16);
      }
      typeKey = true;
      break;
    }

    if (typeKey) {
      t9key = calcT9(multipress[0], multipress[1], numeric);
      multipress[1] = 0;

      if (capitalShift) {
        // disable capitalShift for next iteration
        capitalShift = false;

        if (t9key == ' ') {
          // print a backspace and set the cursor to the appropriate position
          backspace();
          return;
        } else if (isAlpha(t9key)) {
          // capitalize t9key if necessary
          t9key = toUpperCase(t9key);
        }
      }

      // print t9key at cursor location and increment cursor position tracker (only if cursor is not already at max position)
      if (cursor <= 32) {
        if (cursor == 17) {
          lcd.setCursor(0, 1);
        }
        lcd.print(t9key);
        cursor++;
      }
    }

  }
}
