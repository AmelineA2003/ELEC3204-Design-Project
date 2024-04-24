#define outputA 2  //CLK pin
#define outputB 3  //DT pin
#define rstbtn  4  //reset button pin

int counter = 0;
const float pi = 3.14; // Pi value 

const int R = 7;  //Radius of the wheel from center to edge
const int N = 40; //no of steps for one rotation
float distance = 0;

int pwm;
int duty_cycle;

bool IN3, IN4;

void setup() {
  // Initialize LCD

  Serial.begin(9600);
  delay(2000);

  pinMode(9,OUTPUT); 
  pinMode(10,OUTPUT); 
  pinMode(11,OUTPUT); 

  digitalWrite(10, LOW); 
  digitalWrite(9, HIGH); 

  pinMode(5,INPUT); 
  pinMode(6,INPUT); 
  pinMode(7,INPUT); 



  // Initialize encoder pins
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  pinMode(rstbtn, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(outputA), readEncoder, FALLING);

}

void loop() {
  
  if (Serial.available())
  {
    String user_input = Serial.readStringUntil("\n"); 
  }
  
  if (digitalRead(5) || user_input == "up"){
    IN3 = LOW;
    IN4 = HIGH;
  }
  else if (digitalRead(6) || user_input == "down"){
    IN3 = HIGH;
    IN4 = LOW;
  }
  else if (digitalRead(7) || user_input == "brake"){
    IN3 = LOW;
    IN4 = LOW; 
  }

  digitalWrite(10, IN3); 
  digitalWrite(9, IN4); 
    
  if (digitalRead(rstbtn) == LOW) {
    resetCounter();
  }

  duty_cycle = 100;
  pwm = map(0, 100, 0, 255, duty_cycle);
  analogWrite(11, pwm);
  delay(5);
}


void readEncoder() {
  int bValue = digitalRead(outputB);
  if (bValue == HIGH) {
    counter++; // Clockwise
  }
  if (bValue == LOW) {
    counter--; // Counterclockwise
  }
}

// Get the counter value, disabling interrupts.
// This make sure readEncoder() doesn't change the value
// while we're reading it.
int getCounter() {
  int result;
  noInterrupts();
  result = counter;
  interrupts();
  return result;
}

void resetCounter() {
  noInterrupts();
  counter = 0;
  interrupts();
}
