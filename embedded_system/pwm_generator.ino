int PWM_OUTPUT = 3; // pino 3

void setup() {
  TCCR1B = TCCR1B & B11111000 | B00000001;// Mudança de frequência para 31,372kHz
  Serial.begin(9600);
  pinMode(PWM_OUTPUT, OUTPUT);
}

void loop() {
  // testando o PWM
  for (int i = 0; i < 120; i++) {
    analogWrite(PWM_OUTPUT, i);
  }

  for (int k=120; k >= 0; k--){
    analogWrite(PWM_OUTPUT, k);
  }  
}
