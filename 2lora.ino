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

void encrypt(String input, byte *outArray) {
  byte in[33] = { 0 };
  input.getBytes(in, input.length() + 1);
  // process the input in 8-byte blocks
  for (int i = 0; i < 32; i += 8) {
    des.tripleEncrypt(outArray + i, in + i, desKey);
  }
}

void decrypt(byte *inArray, byte *outArray) {
  // process the input in 8-byte blocks
  for (int i = 0; i < 32; i += 8) {
    des.tripleDecrypt(outArray + i, inArray + i, desKey);
  }
}

void hexStringToByteArray(const String &hexString, byte *byteArray) {
  int len = hexString.length();
  int byteIndex = 0;

  for (int i = 0; i < len; i += 2) {
    if (byteIndex >= 32) {
      break;  // prevent buffer overflow
    }
    String byteString = hexString.substring(i, i + 2);
    byteArray[byteIndex++] = (byte)strtol(byteString.c_str(), NULL, 16);
  }
}

String byteArrayToHexString(const byte *byteArray) {
  String hexString;
  for (int i = 0; i < 32; i++) {
    if (byteArray[i] < 16) hexString += "0";  // Add leading zero for single digit hex values
    hexString += String(byteArray[i], HEX);
  }
  return hexString;
}

String byteArrayToString(const byte *byteArray) {
  String result = "";
  for (int i = 0; i < 32; i++) {
    result += (char)byteArray[i];
  }
  return result;
}

// encrypt and send message to LoRa peer
void sendToPeer(const char *message) {
  // ensure message is not blank
  if (message[0] == '\0') {
    return;
  }

  // encrypt message prior to send
  byte messageEncBytes[33] = { 0 };
  encrypt(String(message), messageEncBytes);
  String messageEncString = byteArrayToHexString(messageEncBytes);

  Slora.print("AT+SEND=");
  Slora.print(peerAddress);
  Slora.print(",");
  Slora.print(messageEncString.length());
  Slora.print(",");
  Slora.println(messageEncString);

  // clear slora buffer as to not interpret as incoming message
  while (Slora.available() < 1) {
    delay(25);
  }
  clearSloraBuffer();
}

// return parsed and decrypted incoming message from LoRa peer
String parseMessage(String message) {
  String parsed = message.substring(0, message.lastIndexOf(',', message.lastIndexOf(',') - 1));
  int secondComma = parsed.indexOf(',', message.indexOf(',') + 1);
  parsed = parsed.substring(secondComma + 1);

  // decrypt message prior to return
  byte parsedBytes[32] = { 0 };
  hexStringToByteArray(parsed, parsedBytes);
  byte decBytes[32] = { 0 };
  decrypt(parsedBytes, decBytes);
  String parsedFinal = byteArrayToString(decBytes);

  return parsedFinal;
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