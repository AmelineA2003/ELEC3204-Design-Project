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
const int N = 40;
const int R = 3;
float c_displacement = 0;
float req_disp = 0;

float curr_diff;
float prev_diff;


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
  
  analogWrite(ENB, 255);
}

void loop() {
  
  c_displacement = ((2*pi*R)/N) * getCounter();
  Serial.println(c_displacement);

  if (c_displacement > 2600){
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
    req_disp = 1700;
    Serial.print("  LVL1_Button   ");
  }
  else if (digitalRead(LVL2_BUTTON) == HIGH){
    req_disp = 3300; 
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
