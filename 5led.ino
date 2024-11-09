void blinkLED(uint8_t count, uint16_t interval) {
  digitalWrite(LED, 0);
  for(int i = 0; i < count; i++) {
    digitalWrite(LED, 1);
    delay(interval);
    digitalWrite(LED, 0);
    delay(interval);
  }
}