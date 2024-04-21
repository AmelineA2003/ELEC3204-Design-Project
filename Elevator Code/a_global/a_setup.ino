void setup() {
  // Initialize LCD

  Serial.begin(9600);
  delay(2000);

  // initialise motor controller pins
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(PWM_B,OUTPUT);
  
  // initialise button pins
  pinMode(GROUND,INPUT);
  pinMode(LVL1,INPUT);
  pinMode(LVL2,INPUT);
  pinMode(E-Brake,INPUT);

  // Initialize encoder pins
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  pinMode(rstbtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(outputA), readEncoder, FALLING);
  attachInterrupt(digitalPinToInterrupt(E-Brake), brake, FALLING);

  //initialise the direction of the elevator
  digitalWrite(10, LOW);
  digitalWrite(9, HIGH);
    
  speed_calibrate(20, &duty_cycle);

  lvl_calibrate(&duty_cycle, &ground_disp, &first_disp, &second_disp);
}
