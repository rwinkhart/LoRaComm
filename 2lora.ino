// get address of partner LoRa module
uint8_t getPeerAddress() {
  // clear serial buffer prior to address query
  clearSloraBuffer();

  Slora.println("AT+ADDRESS?");

  while (!Slora.available()) {
    // wait until serial response is received
  }

  if (Slora.readString().indexOf("ADDRESS=2") > 0) {
    return 1;
  } else {
    return 2;
  }
  
}

// send message to partner LoRa module
void sendToPeer(const char* message) {
  Slora.print("AT+SEND=");
  Slora.print(peerAddress);
  Slora.print(",");
  Slora.print(strlen(message));
  Slora.print(",");
  Slora.println(message);

  // clear serial buffer as to not interpret as incoming message
  delay(500); // TODO find a more sophisticated way to delay
  clearSloraBuffer();
}

// return parsed message from incoming LoRa message
String parseMessage(String message) {
  String parsed = message.substring(0, message.lastIndexOf(',', message.lastIndexOf(',') - 1));
  int secondComma = parsed.indexOf(',', message.indexOf(',') + 1);
  parsed = parsed.substring(secondComma+1);


  return parsed;
}

void clearSloraBuffer() {
  while (Slora.available() > 0) {
    Slora.read();
  }
}

void clearSerialBuffer() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}