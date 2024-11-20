#include <SoftwareSerial.h>
// Defina os pinos RX e TX do Arduino para a comunicação com o ESP8266
SoftwareSerial espSerial(10, 11); // RX, TX

const int botao = 2;       // Define o pino do botão

void setup() {
  pinMode(botao, INPUT);   // Configura o pino do botão como entrada
  Serial.begin(9600);        // Comunicação com o computador
  espSerial.begin(9600);     // Comunicação com o ESP8266
  Serial.println("Sistema inicializado.");
}

void loop() {
  int estadoBotao = digitalRead(botao);   // Lê o estado do botão
  if (estadoBotao == HIGH) {              // Se o botão for pressionado
    String mensagem = "Botão de Pânico Acionado";
    espSerial.println(mensagem); // Envia mensagem ao ESP8266
    Serial.println("Mensagem enviada ao ESP8266.");
  }
  delay(1000); // Aguarda 1 segundo para evitar sobrecarga
}
