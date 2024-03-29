




// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  Serial.begin(9600); // Begen listening on port 9600 for serial
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

// the loop function runs over and over again forever
void loop() {

  
   if(Serial.available() > 0) // Read from serial port
    {
      char ReaderFromNode; // Store current character
      ReaderFromNode = (char) Serial.read();
      convertToState(ReaderFromNode); // Convert character to state  
      getPersonTemp(ReaderFromNode); // get person temp
    }
    
   // Serial.println("personTemp:"+(String)random(20,40));
  delay(1000); 
}

void convertToState(char chr) {
  
  if(chr=='w'){
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100); 
  }
  if(chr=='t'){
    digitalWrite(LED_BUILTIN, LOW);
    delay(100); 
  }
}


void getPersonTemp(char chr) {
  
  if(chr=='a'){
    Serial.println("personTemp:"+(String)random(30,40));
  }
  
}
