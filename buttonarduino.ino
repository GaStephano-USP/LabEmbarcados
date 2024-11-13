const int botao = 2;       // Define o pino do botão
const int led = 13;        // Define o pino do LED embutido

void setup() {
  pinMode(botao, INPUT);   // Configura o pino do botão como entrada
  pinMode(led, OUTPUT);    // Configura o pino do LED como saída
}

void loop() {
  int estadoBotao = digitalRead(botao);   // Lê o estado do botão

  if (estadoBotao == HIGH) {              // Se o botão for pressionado
    digitalWrite(led, HIGH);              // Acende o LED
  } else {
    digitalWrite(led, LOW);               // Apaga o LED
  }
}

/**
Como montar:
Coloque o botão na protoboard.
Conecte um dos pinos do botão ao pino digital 2 do Arduino (ou outro pino digital de sua escolha).
Conecte o mesmo pino do botão ao GND do Arduino através do resistor de 10kΩ, para funcionar como um pull-down resistor. Esse resistor garante que o valor lido pelo pino do Arduino seja zero (LOW) quando o botão não estiver pressionado.
Conecte o outro pino do botão ao +5V do Arduino. Quando o botão for pressionado, o pino de entrada (pino 2) lerá HIGH.**/
