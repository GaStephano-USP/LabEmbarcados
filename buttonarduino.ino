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
