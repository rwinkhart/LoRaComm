// write a message to the LCD and keep it present until a key is pressed
void writeToLCD(const char *message, bool block) {
  lcd.clear();
  cursor = 1;
  for (int i = 0; message[i] != '\0'; i++) {
    if (cursor == 17) {
      lcd.setCursor(0, 1);
    }
    lcd.print(message[i]);
    cursor++;
  }

  if (block && !docked) {
    blockUntilKey();
  }
}