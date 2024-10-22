// get address of partner LoRa module
uint8_t getPeerAddress() {
  // clear serial buffer prior to address query
  clearSerialBuffer();

  Serial.println("AT+ADDRESS?");

  while (!Serial.available()) {
    // wait until serial response is received
  }

  if (Serial.readString().indexOf("ADDRESS=2") > 0) {
    return 1;
  } else {
    return 2;
  }
  
}

// send message to partner LoRa module
void sendToPeer(const char* message) {
  Serial.print("AT+SEND=");
  Serial.print(peerAddress);
  Serial.print(",");
  Serial.print(strlen(message));
  Serial.print(",");
  Serial.println(message);

  // clear serial buffer as to not interpret as incoming message
  delay(500); // TODO find a more sophisticated way to delay
  clearSerialBuffer();
}

void clearSerialBuffer() {
  while (Serial.available() > 0) {
    Serial.read();
  }
}