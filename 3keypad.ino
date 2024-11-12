void backspace() {
  uint8_t cursorX;
  uint8_t cursorY;

  cursor--;
  cursorX = cursor;
  if (cursorX > 16) {
    cursorY = 1;
    cursorX = cursorX - 16;
  } else {
    cursorY = 0;
  }

  // set cursor to previous position (subtract one since cursor is numbered from 1)
  lcd.setCursor(cursorX - 1, cursorY);
  // print a space
  lcd.print(' ');
  // set cursor back to the newly blank space
  lcd.setCursor(cursorX - 1, cursorY);
}

void determineMultipress(char key) {
  switch (key) {
    case 'A':  // A B C | 1
      updateMultipress(1);
      break;
    case 'D':  // D E F | 2
      updateMultipress(2);
      break;
    case 'G':  // G H I | 3
      updateMultipress(3);
      break;
    case 'a':  // alphabetical keyboard
      numeric = false;
      break;
    case 'J':  // J K L | 4
      updateMultipress(5);
      break;
    case 'M':  // M N O | 5
      updateMultipress(6);
      break;
    case 'P':  // P Q R | 6
      updateMultipress(7);
      break;
    case 'n':  // numeric keyboard
      numeric = true;
      break;
    case 'S':  // S T U | 7
      updateMultipress(9);
      break;
    case 'V':  // V W X | 8
      updateMultipress(10);
      break;
    case 'Y':  // Y Z , | 9
      updateMultipress(11);
      break;
    case '^':  // capital shift
      capitalShift = true;
      break;
    case ':':  // : ) ( | '
      updateMultipress(13);
      break;
    case '.':  // . ! ? | 0
      updateMultipress(14);
      break;
    case '>':  // send message
      sendToPeer(messageBuffer);
      // do not clear messageBuffer or LCD until a reply is received
      break;
    case ' ':  // confirm t9 character | space (if last character was confirmed) | backspace (if capital shifted)
      if (multipress[1] == 0) {
        // if last character was confirmed, send a space
        updateMultipress(16);
      }
      typeKey = true;
      break;
  }
}

void updateMultipress(uint8_t keyNumber) {
  uint8_t pressCount;

  if (multipress[0] == keyNumber) {
    // if this key was pressed previously, increment pressCount
    pressCount = multipress[1] + 1;

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
    case 13:
      if (numeric) {
        return '\'';
      }
      trio[0] = ':';
      trio[1] = ')';
      trio[2] = '(';
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
      trio[0] = ' ';  // rest of trio does not need to be defined (once multipress exceeds 1 for a space, it is printed and multipress is reset)
  }

  return trio[pressCount - 1];
}

// block until a key is pressed
void blockUntilKey() {
  key = NO_KEY;
  while (key == NO_KEY) {
    key = keypad.getKey();
  }
}