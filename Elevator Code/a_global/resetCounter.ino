void resetCounter() {
  noInterrupts();
  counter = 0;
  interrupts();
}
