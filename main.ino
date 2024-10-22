void setup(){
  // enable serial console
   Serial.begin(9600);

  // set LED pin mode
  pinMode(LED, OUTPUT);

  // store address of LoRa peer
  //peerAddress = getPeerAddress();

  // define LCD columns and rows (and clear it)
  lcd.begin(16, 2);
  lcd.clear();
}

void loop(){
  // always listen for incoming messages
  if (Serial.available()) {
    clearSerialBuffer();
    digitalWrite(LED, 1);
    delay(1000);
    digitalWrite(LED, 0);
  }

  // always listen for keypresses
  key = keypad.getKey();

  // actions to perform when a key is pressed
  if (key) {
    typeKey = false;

    determineMultipress(key);

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

      // print t9key at cursor location and add to message buffer; increment cursor position tracker (only if cursor is not already at max position)
      if (cursor <= 32) {
        if (cursor == 17) {
          lcd.setCursor(0, 1);
        }
        lcd.print(t9key);
        messageBuffer[cursor-1] = t9key;
        messageBuffer[cursor] = '\0';
        cursor++;
      }
    }

  }
}
