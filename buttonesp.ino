const int pinoBotao = A0;  // Pino analógico onde o botão está conectado
const int limiar = 512;    // Limiar para considerar o botão pressionado (ajuste conforme o circuito)

void setup() {
  Serial.begin(115200);  // Inicializa a comunicação serial a 115200 bps
}

void loop() {
  int leitura = analogRead(pinoBotao);  // Lê o valor analógico do pino
  Serial.println(leitura);              // Imprime o valor lido para monitoramento

  // Verifica se o valor lido está abaixo do limiar
  if (leitura < limiar) {
    Serial.println("Botão pressionado!");
  } else {
    Serial.println("Botão solto.");
  }

  delay(500);  // Pequeno atraso para evitar leituras muito rápidas
}