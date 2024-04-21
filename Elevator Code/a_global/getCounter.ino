int getCounter() {
  int result;
  noInterrupts();
  result = counter;
  interrupts();
  return result;
}
