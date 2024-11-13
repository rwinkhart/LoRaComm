void setup() {
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

void loop() {
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

      // forward reply to GUI
      Serial.println("%OK");
    } else {
      // not a reply
      if (!incoming.equals(incomingPrev)) {
        // flash notification LED
        blinkLED(1, 1000);
        // send reply to peer
        sendToPeer("%OK\0");
        // echo message over hardware serial for GUI
        Serial.println(incoming);
        // print message to screen
        if (!docked) {
          writeToLCD(incoming.c_str(), true);
          writeToLCD(messageBuffer, false);
        }
        // store message
        incomingPrev = incoming;
      } else {
        // send reply to peer (even if duplicate)
        sendToPeer("%OK\0");
      }
    }

    // clear Slora buffer to avoid overloading Slora serial
    clearSloraBuffer();
  }

  // always listen for requests from GUI (to forward over Slora)
  if (Serial.available()) {
    String incoming = Serial.readString();
    if (incoming.equals("%DOCK")) {
      // toggle docked mode
      docked = !docked;
      if (docked == true) {
        blinkLED(2, 100);
      } else {
        blinkLED(3, 100);
      }
    } else {
      sendToPeer(incoming.c_str());
    }
    clearSerialBuffer();  // avoid over-loading serial interface
  }

  // always listen for keypresses
  key = keypad.getKey();

  // actions to perform when a key is pressed
  if (key) {
    typeKey = false;

    determineMultipress(key);

    if (typeKey) {
      MPkey = calcMP(numeric);
      multipress[1] = 0;  // reset press count

      if (capitalShift) {
        // disable capitalShift for next iteration
        capitalShift = false;

        if (MPkey == ' ') {
          // print a backspace and set the cursor to the appropriate position
          backspace();
          return;
        } else if (isAlpha(MPkey)) {
          // capitalize MPkey if necessary
          MPkey = toUpperCase(MPkey);
        }
      }

      // print MPkey at cursor location and add to message buffer; increment cursor position tracker (only if cursor is not already at max position)
      if (cursor <= 32) {
        if (cursor == 17) {
          lcd.setCursor(0, 1);
        }
        lcd.print(MPkey);
        messageBuffer[cursor - 1] = MPkey;
        messageBuffer[cursor] = '\0';
        cursor++;
      }
    }
  }
}
