
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

}

void loop() {
  
  digitalWrite(11, 255);
  
  if (digitalRead(5)){
    Serial.println("Ground"); 
      digitalWrite(10, LOW); 
    digitalWrite(9, HIGH); 
  }
  else if (digitalRead(6)){
    Serial.println("Level 1"); 
    digitalWrite(10, HIGH); 
    digitalWrite(9, LOW); 
  }
  else if (digitalRead(7)){
    Serial.println("Level 2"); 
    digitalWrite(10, LOW); 
    digitalWrite(9, LOW); 
  }

}
