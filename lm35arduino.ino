  //pino de leitura
#define ADCPIN 0

//variavel da temperatura
float temperatura = 0;

void setup(){
  //utiliza ref interna de 1.1
  analogReference(INTERNAL);

  //Inicializa Serial
  Serial.begin(9600);

}

void loop(){
  //faz leitura do sensor pela funcao
  temperatura = analogAvg(ADCPIN);

  //faz conversao de volts para celcius
  temperatura = temperatura  * 1100 / (1024 * 10);

  //imprime temperatura no serial
  Serial.println(temperatura);

  delay(1000);
}

int analogAvg (int sensorPin){
  int total = 0;
  int amostras = 20;
  for (int n = 0; n < amostras; n++){
    total += analogRead (sensorPin);
    delay(10); //pequeno atraso entre leituras
  }
  return total / amostras;
}