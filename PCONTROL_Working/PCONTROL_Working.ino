#define OUTPUTA 2
#define OUTPUTB 3

#define OVERRIDE_BUTTON 4
#define GND_BUTTON 5
#define LVL1_BUTTON 6
#define LVL2_BUTTON 7

#define IN3 9
#define IN4 10
#define ENB 11


int counter = 0;
const float pi = 3.14;
const int N = 12; //our encoder has 48 counts per revolution
const int R = 3;

float c_displacement = 0, req_disp = 0, prev_displacement;

float c_time, prev_time, t_delta, c_rpm = 0;


int time_step = 5;
long rpm;

long prev_counter = 0, c_counter = 0;

unsigned long prevMicro = 0, currentMicro;

const float targetRPM = 47;  // This should be set according to your system requirements
const float Kp = 9;  // This value needs to be tuned for your system

// Current speed and control output
// float currentSpeed;
float controlOutput;


void setup() {

  prevMicro = micros();
  req_disp = 0;
  
  Serial.begin(9600);
  delay(2000);

  pinMode(OUTPUTA, INPUT);
  pinMode(OUTPUTB, INPUT);
  

  pinMode(IN3,OUTPUT); 
  pinMode(IN4,OUTPUT); 
  pinMode(ENB,OUTPUT); 

  digitalWrite(IN4, LOW); 
  digitalWrite(IN3, LOW); 

  //IN
  pinMode(GND_BUTTON,INPUT); 
  pinMode(LVL1_BUTTON,INPUT); 
  pinMode(LVL2_BUTTON,INPUT); 
  pinMode(OVERRIDE_BUTTON,INPUT);

  attachInterrupt(digitalPinToInterrupt(OUTPUTA), readEncoder, FALLING);

}

void loop() {


  c_displacement = ((2*pi*R)/N) * getCounter();
  RPM();
//
  float rpmError = targetRPM - abs(rpm);
//
//  // Calculate control output (adjust motor PWM)
  controlOutput = abs(Kp * rpmError);

  int PWM = 180 + controlOutput;
  PWM = constrain(PWM, 180, 255);
  analogWrite(ENB, abs(int(PWM)));

//  analogWrite(ENB, 180);

  String words = "Displacement: " + String(c_displacement, 2) + ", RPM: " + String(abs(rpm)) + ", PWM: " + String(PWM);
//  String words = "Displacement: " + String(c_displacement, 2) + ", RPM: " + String(abs(rpm));


  Serial.println(words);
  
//  Serial.print("Displacement: %0.2f, Velocity: %0.2f, PWM Signal: %0.2f", c_displacement, c_velocity, pwmSignal);

  if (c_displacement > 13300){
    digitalWrite(IN4, LOW); 
    digitalWrite(IN3, LOW); 
  }

  if (c_displacement < -100){
    digitalWrite(IN4, LOW); 
    digitalWrite(IN3, LOW); 
  }

  if (digitalRead(OVERRIDE_BUTTON) == HIGH){
    delay(1000);
    override();
  }

  if (digitalRead(GND_BUTTON) == HIGH){
    req_disp = 0;
    Serial.println("  GND_Button    ");
  }
  else if (digitalRead(LVL1_BUTTON) == HIGH){
    req_disp = 6000;
    Serial.println("  LVL1_Button   ");
  }
  else if (digitalRead(LVL2_BUTTON) == HIGH){
    req_disp = 10000; 
    Serial.println("  LVL2_Button   ");
  }
  else{
    //do nothing
  }

  
  if (abs(c_displacement - req_disp) < 100){
    digitalWrite(IN4, LOW); 
    digitalWrite(IN3, LOW); 
  }
  else if (c_displacement < req_disp){
    digitalWrite(IN4, LOW); 
    digitalWrite(IN3, HIGH); 
  }
  else if (c_displacement > req_disp){
    digitalWrite(IN4, HIGH); 
    digitalWrite(IN3, LOW);     
  }

  prev_displacement = c_displacement;
//  prev_velocity = c_velocity;

//  calc_count++;
  delay(time_step);
}


void override(){
  Serial.println("override triggered: manual mode");
  while(1)
  {

    c_rpm = RPM();
    
    c_displacement = ((2*pi*R)/N) * getCounter();
//    c_velocity = (c_displacement - prev_displacement) / time_step;
    // Serial.println(c_displacement, c_velocity);
    
//    String words = "Manual | Displacement: " + String(c_displacement, 2) + ", RPM: " + String(c_rpm, 2);
//    Serial.println(words);

    if (digitalRead(LVL2_BUTTON))
    {
      digitalWrite(10, LOW); 
      digitalWrite(9, HIGH); 
      Serial.println("UP");
    }
    else if (digitalRead(LVL1_BUTTON))
    {
      digitalWrite(10, HIGH); 
      digitalWrite(9, LOW); 
      Serial.println("DOWN");
    }
    else if (digitalRead(GND_BUTTON))
    {
      digitalWrite(10, LOW); 
      digitalWrite(9, LOW);
      Serial.println("Brake"); 
    }
    prev_displacement = c_displacement;
//    prev_velocity = c_velocity;
  }

}

int getCounter() {
  int result;
  noInterrupts();
  result = counter;
  interrupts();
  return result;
}

void readEncoder() {
  int bValue = digitalRead(OUTPUTB);
  if (bValue == HIGH) {
    counter++; // Clockwise
  }
  if (bValue == LOW) {
    counter--; // Counterclockwise
  }
//  Serial.println(counter);

  // if (counter % 48*4 == 0)
  // {
  //   // Serial.println(counter);
  //   c_time = micros();
  //   t_delta = (c_time - prev_time)/4;
  //   prev_time = c_time;
  // }
}
void resetCounter() {
  noInterrupts();
  counter = 0;
  interrupts();
}

long RPM() {
//  long rpm;
  currentMicro = millis();

//  String times = "current: " + String(currentMicro, 4) + "prev: " + String(prevMicro, 4);
//  Serial.println(times);

  if (currentMicro - prevMicro >= 1000){
    long c_counter = getCounter();

    int t_delta = currentMicro - prevMicro;

    rpm = (c_counter - prev_counter) * 60/ (t_delta);

//    String diff = String(t_delta) + "   " + String(rpm);
//    Serial.println(diff);
//    String diff = String(c_counter) + "   " + String(prev_counter);
//    Serial.println(rpm);

    prevMicro = currentMicro;
    prev_counter = c_counter;
    return rpm;
  }
//  prev_counter = c_counter;
//  return rpm;
}
