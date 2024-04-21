void calibrate(float req_speed, float* duty_cycle){
  //tunes the pwm signal to maintain the same elevator speed
  float cali_dis[10];
  float elev_speed;
  float test_duty = 0;
  float pwm;
  int t_step = 5;
  int i;
  
  do {
    delay(500);
    
    test_duty += 10;
    pwm = map(0, 100, 0, 255, test_duty);
    
    Serial.println("Testing duty cycle %0.2f %", test_duty);
    analogWrite(PWM_B, pwm);
    
    for (i = 0; i < 10; i++){
      cali_dis[i] = ((2*pi*R)/N) * getCounter();
      delay(t_step);
    }
  
    elev_speed = ( cali_dis[9] - cali_dis[0] ) / (i + 1);
    Serial.println("Elevator speed: %0.2f", elev_speed);
    Serial.println("Error: %0.2f", (req_speed - elev_speed) / req_speed);
    release();
    
  } while ( (req_speed - elev_speed) / req_speed ) > 0.10) 

  do {
    delay(500);
    
    test_duty += 1;
    pwm = map(0, 100, 0, 255, test_duty);

    Serial.println("Testing duty cycle %0.2f %", test_duty);
    analogWrite(PWM_B, pwm);
    
    for (i = 0; i < 10; i++){
      cali_dis[i] = ((2*pi*R)/N) * getCounter();
      delay(t_step);
    }
  
    elev_speed = ( cali_dis[9] - cali_dis[0] ) / (i + 1);
    Serial.println("Elevator speed: %0.2f", elev_speed);
    Serial.println("Error: %0.2f", (req_speed - elev_speed) / req_speed);
    release();
    
  } while ( (req_speed - elev_speed) / req_speed ) > 0.10) 

  *duty_cycle = test_duty;

  Serial.println("The elevator has been calibrated to move at %0.2f with a duty cycle of %0.2f %", req_speed, test_duty);

  return 1;
}
