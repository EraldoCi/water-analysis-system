
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h> // não é uma biliteca oficial

#define WIFI_SSID "nome_do_wifi"
#define WIFI_PASSWORD "senha"

#define FIREBASE_HOST "nome_do_projeto-numero_de_identificacao.firebaseio.com"// 
#define FIREBASE_AUTH "chave_de_autenticaçao"//Chave de autenticação do projeto no firebase
#define VOLTAGE "voltage" // node do banco de dados de tempo real 
#define CURRENT "current" // Valores das correntes medidas

const int sensorPin = A0;
float voltageValue = 0; // Valor da tensão medido

void setup() {
  Serial.begin(9600);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("connecting");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  // Conectando com o Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //ESP.deepSleep(50e6, WAKE_RF_DEFAULT);// modo sleep por 5 segundos
}

void startMeasurement(){
  double potCurrent = 0.0; // corrente do potentiostato
  String eventType = Firebase.getString("type");
  
  voltageValue = analogRead(sensorPin);
  float voltage = (voltageValue*3.3)/1023; 
  
  // adiciona-se 1.7 para correção do valor
  potCurrent = (-voltage + 1.7); // Conversão de tensão para corrente em 10^-5

  Serial.print("Valor da corrente medida: ");
  Serial.println(potCurrent);
  
  Serial.print("Valor da tensão medida: ");
  Serial.println(voltage);
  
  Serial.println("Enviando para o firebase");
  Firebase.push(VOLTAGE, voltage);// Enviando para o Firabase
  Firebase.push(CURRENT, potCurrent);// Enviando para o Firabase

  if(Firebase.failed()){
    Serial.print("Erro no envio das informações para o firebase:");
    Serial.println(Firebase.error());
    return;
  }
  
  delay(1000); //Tempo default para aquisição 
}

void loop() {
   if(Firebase.failed()){
    Serial.print("Erro no envio das informações para o firebase:");
    Serial.println(Firebase.error());
    return;
  }
  
  String start = Firebase.getString("measurement");
  start.toLowerCase();
  
  Serial.println(start); 
  if(start == "true"){
    Serial.println("Começando a medir nível de sal na água");
    startMeasurement();
  } else {
    Serial.print("Medição não inicializada");
  }
}
