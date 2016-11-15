
int LED_stat = 0;
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  pinMode(4, OUTPUT);
  digitalWrite(13, LOW);
  digitalWrite(4, LOW);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  if(Serial.available()){
    while(Serial.available()){
      Serial.read();
    }
    LED_stat = ~LED_stat;
    digitalWrite(13, LED_stat);
    digitalWrite(4, LED_stat);
  }
}
