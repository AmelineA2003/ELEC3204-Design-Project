void lvl_calibrate (float* duty_cycle, float* ground_disp, float* first_disp, float* second_disp){
  float displacement;

  release();
  delay(1000);

  Serial.println("Press ground button to begin level calibration");
  
  while (digitalRead(GROUND, HIGH)) {}
  delay(1000);
  
  analogWrite(PWM_B, map(0, 100, 0, 255, *duty_cycle));

  Serial.println("Elevator will move in 2 seconds. Press ground button to set ground level");
  delay(2000);
  up_dir();
  do{
    displacement = ((2*pi*R)/N) * getCounter();
  } while ( digitalRead(GROUND, HIGH) )

  brake();
  
  Serial.println("Elevator will move in 2 seconds. Press lvl1 button to set level 1");
  delay(2000);
  up_dir();
  do{
    displacement = ((2*pi*R)/N) * getCounter();
  } while ( digitalRead(LVL1, HIGH) )

  brake();
  
  Serial.println("Elevator will move in 2 seconds. Press lvl2 button to set level 2");
  delay(2000);
  up_dir();
  do{
    displacement = ((2*pi*R)/N) * getCounter();
  } while ( digitalRead(LVL2, HIGH) )

  Serial.println("Levels are set");
}
