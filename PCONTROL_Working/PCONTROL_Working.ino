#define OUTPUTA 2
#define OUTPUTB 3

#define OVERRIDE_BUTTON 4
#define GND_BUTTON 5
#define LVL1_BUTTON 6
#define LVL2_BUTTON 7

#define IN3 9
#define IN4 10
#define ENB 11

//************** CONSTANT VARIABLES **************

const int time_step = 5;  // delay for the main loop
const int N = 12; //our encoder has 48 counts per revolution
const int R = 3;
const float pi = 3.14;

//  DYNAMICS SYSTEM CONTROL VARIABLES
const float targetRPM = 47;  // Sets the reference RPM, i.e. the RPM 
const float Kp = 9;          // This value needs to be tuned for your system

//***********************************************

//************** DYNAMIC VARIABLES ***************

float c_displacement = 0, req_disp = 0, prev_displacement;    // Elevator displacement reference variables

//  ENCODER WHEEL VARIABLES
int counter = 0;  
long prev_counter = 0, c_counter = 0;

// TIME REFERENCE VARIABLES
unsigned long prevMicro = 0, currentMicro;

// MOTOR CONTROL VARIABLES  
float controlOutput;
int PWM;

//  RPM variables
float c_rpm = 0; 
long rpm;         //  Variable that hold the current RPM value
float rpmError;   //  Variable that hold the value of the error function 

//*************************************************

void setup() {

  prevMicro = micros();   // Intialises the time reference,
  req_disp = 0;           // Sets the origin point of the elevator
  
  Serial.begin(9600);     // Initialises Serial monitor
  delay(2000);

  pinMode(OUTPUTA, INPUT);  //  intialises encoder signal pins as inputs
  pinMode(OUTPUTB, INPUT);  // We intend to receive a square wave signal from the encoder wheel
  
  //  initialises motor controller signal pins  as outputs
  pinMode(IN3,OUTPUT);      //  IN3 and IN4 control the direction of the motor
  pinMode(IN4,OUTPUT); 
  pinMode(ENB,OUTPUT);      //  outputs the PWM signal

  //IN
  pinMode(GND_BUTTON,INPUT);        //  initialises the control buttons as inputs
  pinMode(LVL1_BUTTON,INPUT); 
  pinMode(LVL2_BUTTON,INPUT); 
  pinMode(OVERRIDE_BUTTON,INPUT);

  
  digitalWrite(IN4, LOW);   // Sets IN4 and IN3 to low so that the elevator is in a 
  digitalWrite(IN3, LOW);   // brake state when the code begins

  //  Initialises an interrupt that triggers when one of the encoder input pins receives a falling edge
  attachInterrupt(digitalPinToInterrupt(OUTPUTA), readEncoder, FALLING);   
  //  When the readEncoder function triggers, the readEncoder function determines the direction the motor is turning
  //  and changes the encoder count, the encoder count can then be converted to 

}

void loop() {

  c_displacement = ((2*pi*R)/N) * getCounter();

  RPM();
  rpmError = targetRPM - abs(rpm);

// Calculate control output (adjust motor PWM)
  controlOutput = abs(Kp * rpmError);

  PWM = 180 + controlOutput;
  PWM = constrain(PWM, 180, 255);
  analogWrite(ENB, abs(int(PWM)));

  // prints elevator status to serial monitor
  String words = "Displacement: " + String(c_displacement, 2) + ", RPM: " + String(abs(rpm)) + ", PWM: " + String(PWM);
  Serial.println(words);
  
  if (c_displacement > 13300)   // As a safety precaution, the elevator will stop moving if it exceeds a certain height displacement
  {                             // so the elevator won't break
    digitalWrite(IN4, LOW); 
    digitalWrite(IN3, LOW); 
  }

  if (c_displacement < -100)    // As a safety precaution, the elevator will stop moving if it goes too far below the origin
  {
    digitalWrite(IN4, LOW); 
    digitalWrite(IN3, LOW); 
  }

  if (digitalRead(OVERRIDE_BUTTON) == HIGH) // As a safety precaution, gives the user manual control of the elevator by switching   
  {                                         // the elevator into override mode
    delay(1000);
    override();
  }

  // This if-else statement defines the levels of the elevator and changes the required displacement of the elevator system in response
  //  to the desired level. The desired level is indicated to the system by pressing the GND, LVL1, and LVL2 buttons
  
  if (digitalRead(GND_BUTTON) == HIGH)
  {
    req_disp = 0;
    Serial.println("  GND_Button    ");
  }
  else if (digitalRead(LVL1_BUTTON) == HIGH)
  {
    req_disp = 6000;
    Serial.println("  LVL1_Button   ");
  }
  else if (digitalRead(LVL2_BUTTON) == HIGH)
  {
    req_disp = 10000; 
    Serial.println("  LVL2_Button   ");
  }
  else
  {
    //do nothing
  }

  //  This if-else statement defines how the system will respond to a given required displacement and changes the direction of the motor 
  //  movement accordingly
  
  if (abs(c_displacement - req_disp) < 100)   // Will stop the elevator when the current displacement is within a range of the required displacement
  {                                           // this is done so that 
    digitalWrite(IN4, LOW); 
    digitalWrite(IN3, LOW); 
  }
  else if (c_displacement < req_disp)
  {
    digitalWrite(IN4, LOW); 
    digitalWrite(IN3, HIGH); 
  }
  else if (c_displacement > req_disp)
  {
    digitalWrite(IN4, HIGH); 
    digitalWrite(IN3, LOW);     
  }

  prev_displacement = c_displacement;
  delay(time_step);
}


void override()
{
  Serial.println("override triggered: manual mode");
  while(1)
  {
    c_rpm = RPM();
    c_displacement = ((2*pi*R)/N) * getCounter();

    //prints elevator status
    String words = "Manual | Displacement: " + String(c_displacement, 2) + ", RPM: " + String(c_rpm, 2);
    Serial.println(words);

    // button response
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
  }
}

int getCounter() 
{
  int result;
  noInterrupts();
  result = counter;
  interrupts();
  return result;
}

void readEncoder() 
{
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

long RPM()
{
  currentMicro = millis();

  if (currentMicro - prevMicro >= 1000){
    long c_counter = getCounter();

    int t_delta = currentMicro - prevMicro;

    rpm = (c_counter - prev_counter) * 60/ (t_delta);

    prevMicro = currentMicro;
    prev_counter = c_counter;
    return rpm;
  }

}
