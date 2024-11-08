void setup(){
  // set pin modes
  pinMode(LED, OUTPUT);
  pinMode(SloraRX, INPUT);
  pinMode(SloraTX, OUTPUT);

  // enable serial consoles
  Serial.begin(9600);
  Slora.begin(9600);

  // store address of LoRa peer
  peerAddress = getPeerAddress();

  // define LCD columns and rows (and clear it)
  lcd.begin(16, 2);
  lcd.clear();
}

void loop(){
  // always listen for incoming LoRa messages
  if (Slora.available()) {
    // store received message
    String incoming = parseMessage(Slora.readString());

    // check if message is a reply
    if (incoming.equals("%OK")) {
      // reply received; clear messagBuffer and LCD
      messageBuffer[0] = '\0';
      lcd.clear();
      cursor = 1;
    } else {
      // not a reply
      // flash notification LED
      digitalWrite(LED, 1);
      delay(1000);
      digitalWrite(LED, 0);
      // echo message over hardware serial for GUI
      Serial.println(incoming);
      // send reply to peer
      sendToPeer("%OK\0");
    }

    // clear Slora buffer to avoid overloading Slora serial
    clearSloraBuffer();
  }

  // always listen for requests from GUI (to forward over Slora)
  if (Serial.available()) {
    sendToPeer(Serial.readString().c_str());
    clearSerialBuffer(); // avoid over-loading serial interface
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
