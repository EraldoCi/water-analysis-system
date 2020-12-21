int PWM_OUTPUT = 3; // pino 3
int AnalogicDigitalConverter = 0;// pino A0 Analogic to Digital Converter

float poststep = 0.0078;
int speedRate[7] = {100, 20, 50, 100, 200, 250, 300};// taxa de velocidade de scan (mV/s)
int const count = 6;
long range[count];
int number = 0;
float voltage = 0;

void setup() {
  TCCR1B = TCCR1B & B11111000 | B00000001;// Mudança de frequência para 31,372kHz
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(PWM_OUTPUT, OUTPUT);
  pinMode(AnalogicDigitalConverter, INPUT);
}

void loop() {
  // testando o PWM
  for (int i = 0; i < 255; i++) {
    analogWrite(PWM_OUTPUT, i);
  }

  for (int k=255; k >= 0; k--){
    analogWrite(PWM_OUTPUT, k);
  }

  // Testando o código do professor:
//  for(int i = 0; i< count; i++){
//    range[i] = (1000000L/(speedRate[i] * 128L));  
//  }
//
//  for(int i = 0; i< count; i++){
//    number = 0;
//    while(number <= 1){
//      // Começo do scan      
//      for(int value=0; value <= 255; value++){
//        analogWrite(PWM_OUTPUT, value);
////        delay(range[i]);
//        voltage = analogRead(AnalogicDigitalConverter); 
//      }
//
//      //Scan reverso
//      for(int value=255; value >= 0; value--){
//        analogWrite(PWM_OUTPUT, value);
////        delay(range[i]);
//        voltage = analogRead(AnalogicDigitalConverter); 
//      }
//      number+=1;
//    } 
//  }
  
}
