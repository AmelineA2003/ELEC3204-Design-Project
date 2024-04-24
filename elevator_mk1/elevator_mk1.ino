#define outputA 2  //CLK pin
#define outputB 3  //DT pin
#define rstbtn  4  //reset button pin

int counter = 0;
const float pi = 3.14; // Pi value 

const int R = 1;  //Radius of the wheel from center to edge
const int N = 40; //no of steps for one rotation
float c_distance = 0;
float required_displacement = 0;

int pwm;
int duty_cycle;

int K;

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
  c_distance = ((2*pi*R)/N) * getCounter();
  Serial.println(c_distance);
  
  if (digitalRead(5)){
    Serial.println("Ground"); 
    required_displacement = 0; 
  }
  else if (digitalRead(6)){
    Serial.println("Level 1"); 
    required_displacement = 15; 
  }
  else if (digitalRead(7)){
    Serial.println("Level 2"); 
    required_displacement = 30;
  }

  if ( (c_distance - required_displacement) > 0){
    digitalWrite(10, HIGH);
    digitalWrite(9, LOW);
  }
  else {
    digitalWrite(10, LOW);
    digitalWrite(9, HIGH);
  }

  while ( c_distance != required_displacement){
    duty_cycle = 100;
    pwm = map(0, 100, 0, 255, duty_cycle);
    analogWrite(11, pwm);
    delay(5);
  }


  
  if (digitalRead(rstbtn) == LOW) {
    resetCounter();
  }
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
