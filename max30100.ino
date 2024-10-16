#include <Wire.h> //Biblioteca para comunicação I2C
#include "MAX30100_PulseOximeter.h" //Biblioteca para uso do sensor de frequência cardíaca e oxímetro MAX30100 

#define REPORTING_PERIOD_MS     1000 // Intervalo entre a medição de cada amostra

uint32_t tsLastReport = 0; // Tempo da última amostra dectada

PulseOximeter pox; //Declaração objeto pox

void onBeatDetected() { //Função executada quando um pulso é detectado
  Serial.println("Batimento detectado");
}

void setup() {
  Serial.begin(9600); //Inicialização da comunicação serial
  Serial.print("Iniciando funcionamento do sensor ");

  if (!pox.begin())  //Testa a comunicação com o sensor
  {
    Serial.println("Falhou ! "); //Se a comunicação não foi realizada corretamente exibe a mensagem "Falhou"
    for (;;);
  } else
  {
    Serial.println("Sucesso ! "); //Se a comunicação foi realizada com sucesso exibe a mensagem "Sucesso !"
  }
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA); // Define que o sensor use 7,6 mA para o led
  pox.setOnBeatDetectedCallback(onBeatDetected); // Registra todas as vezes em que um batimento for detectado
}

void loop() {
  pox.update(); //Atualiza a leitura do sensor

  if (millis() - tsLastReport > REPORTING_PERIOD_MS) { //Imprime a leitura em intervalos de 1000ms
    Serial.print("Taxa de Batimento : ");
    Serial.print(pox.getHeartRate()); // Faz a leitura de batimento cardíaco
    Serial.print("bpm / Saturacao sangue(SP02) : ");
    Serial.print(pox.getSpO2()); //Realiza a leitura da saturação do sangue
    Serial.println("%");

    tsLastReport = millis();
  }
}