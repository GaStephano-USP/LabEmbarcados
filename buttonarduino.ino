/*NEED TO TEST THE CODE*/
/*BUTTON FOR ARDUINO*/

const int pinoBotao = A0;  // Pino analógico onde o botão está conectado
const int limiar = 500;    // Limiar para considerar o botão pressionado

void setup() {
  Serial.begin(9600);      // Inicializa a comunicação serial para monitorar o valor
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