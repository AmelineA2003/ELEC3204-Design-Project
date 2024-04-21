void readEncoder() {
  int bValue = digitalRead(outputB);
  if (bValue == HIGH) {
    counter++; // Clockwise
  }
  if (bValue == LOW) {
    counter--; // Counterclockwise
  }
}
