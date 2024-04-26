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
const int N = 48; //our encoder has 48 counts per revolution
const int R = 3;
float c_displacement = 0;
float req_disp = 0;

float curr_diff;
float prev_diff;
float prev_displacement;

float c_velocity;
float prev_velocity;

int time_step = 5;
int calc_counter = 0;
int Kp_position = 1;  // Proportional gain for position
int Kp_speed = 10;  // Proportional gain for speed

void setup() {

  
  // Initialize LCD
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
  
  analogWrite(ENB, 200);
}

void loop() {

  
  c_displacement = ((2*pi*R)/N) * getCounter();

  c_velocity = (c_displacement - prev_displacement) / time_step;

  float  heighterror = req_disp - c_displacement;
  float desiredSpeed = Kp_position * heighterror;

  float speedError = desiredSpeed - c_velocity;
  float pwmSignal = Kp_speed * speedError;
  pwmSignal = constrain(pwmSignal, 151, 255);
  analogWrite(ENB, pwmSignal);

  String words = "Displacement: " + String(c_displacement, 2) + ", Velocity: " + String(c_velocity, 2) + ", PWM Signal: " + String(pwmSignal);

  Serial.println(words);
  
//  Serial.print("Displacement: %0.2f, Velocity: %0.2f, PWM Signal: %0.2f", c_displacement, c_velocity, pwmSignal);

  if (c_displacement > 3100){
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
    Serial.print("  GND_Button    ");
  }
  else if (digitalRead(LVL1_BUTTON) == HIGH){
    req_disp = 1600;
    Serial.print("  LVL1_Button   ");
  }
  else if (digitalRead(LVL2_BUTTON) == HIGH){
    req_disp = 2750; 
    Serial.print("  LVL2_Button   ");
  }
  else{
    //do nothing
  }

  
  if (abs(c_displacement - req_disp) < 15){
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
  prev_velocity = c_velocity;

//  calc_count++;
  
  delay(time_step);
}


void override(){
  Serial.println("override triggered: manual mode");
  while(1)
  {
    c_displacement = ((2*pi*R)/N) * getCounter();
    Serial.println(c_displacement);
    
    if (digitalRead(LVL2_BUTTON))
    {
      digitalWrite(10, LOW); 
      digitalWrite(9, HIGH); 
    }
    else if (digitalRead(LVL1_BUTTON))
    {
      digitalWrite(10, HIGH); 
      digitalWrite(9, LOW); 
    }
    else if (digitalRead(GND_BUTTON))
    {
      digitalWrite(10, LOW); 
      digitalWrite(9, LOW); 
    }
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
}

void resetCounter() {
  noInterrupts();
  counter = 0;
  interrupts();
}
