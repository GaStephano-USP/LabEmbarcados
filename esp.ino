#include <ESP8266WiFi.h> 
#include <PubSubClient.h> 
#include <ESP8266HTTPClient.h> 
#include <Wire.h> //Biblioteca para comunicação I2C
#include "MAX30100_PulseOximeter.h" //Biblioteca para uso do sensor de frequência cardíaca e oxímetro MAX30100 

#define REPORTING_PERIOD_MS     1000 // Intervalo entre a medição de cada amostra

uint32_t tsLastReport = 0; // Tempo da última amostra dectada

PulseOximeter pox; //Declaração objeto pox

const char* ssid = "";                                
const char* password = "";                                    
const char* mqtt_server = "broker.mqtt-dashboard.com";                  
const char* mqttTopic = "DotHealth/alarme";

WiFiClient espClient;                                                   
PubSubClient client(espClient);                                         
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup() {                                                          
  pinMode(BUILTIN_LED, OUTPUT);                                         
  Serial.begin(9600);                                                 
  setup_wifi();                                                         
  client.setServer(mqtt_server, 1883);                                  
  client.setCallback(callback);
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
}



void setup_wifi() {                                                     

  delay(10);                                                            
  Serial.println();                                                     
  Serial.print("Conectando com ");                                       
  Serial.println(ssid);                                                 
             
  WiFi.begin(ssid, password);                                           
 
  while (WiFi.status() != WL_CONNECTED) {                               
    delay(500);                                                         
    Serial.print(".");                                                  
  }
  randomSeed(micros());
  Serial.println();                                                     
  Serial.println("WiFi conectado");                                     
  Serial.println("Endereço de IP: ");                                   
  Serial.println(WiFi.localIP());                            
}

void callback(char* topic, byte* payload, unsigned int length) {        
  Serial.print("Mensagem recebida [");                                  
  Serial.print(topic);                                                  
  Serial.print("] ");                                                   
  for (int i = 0; i < length; i++) {                                    
    Serial.print((char)payload[i]);                                     
  }
  Serial.println();                                                     

  if ((char)payload[0] == '1') {                                        
    digitalWrite(BUILTIN_LED, LOW);                                     
  } else {                                                              
    digitalWrite(BUILTIN_LED, HIGH);                                    
  }

}

void reconnect() {                                                       
  while (!client.connected()) {                                          
    Serial.print("Aguardando conexão MQTT...");                          
    if (client.connect("ESP8266Client")) {                               
      Serial.println("conectado");                                       
      client.publish("DotHealth/heartbeat", "hello world");    
      client.subscribe("DotHealth/Led");                         
    } else {                                                             
      Serial.print("falhou, rc=");                                       
      Serial.print(client.state());                                      
      Serial.println(" tente novamente em 5s");                          
      delay(5000);                                                       
    }
  }
}

void loop() { 

  // Reconexão MQTT, se necessário
  if (!client.connected()) {                                             
    reconnect();
    //COMENTEI PRA TESTAR O ENVIO DE DADOS PRO BANCO
  }                                                     

   // Verifica se há dados recebidos na porta serial (RX)
  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n'); // Lê até o caractere de nova linha
    receivedData.trim(); // Remove espaços extras ou quebras de linha
    Serial.print("Recebido no RX: ");
    Serial.println(receivedData);

    if(receivedData == "queda"){
      // Publica a mensagem recebida no tópico MQTT
      if (client.publish(mqttTopic, "1")) {
        Serial.println("Mensagem enviada ao MQTT!");
      } else {
        Serial.println("Falha ao enviar mensagem ao MQTT!");
      }
      sendPostFall(receivedData);
    }

    else if(receivedData == "botao"){
      if (client.publish(mqttTopic, "1")) {
        Serial.println("Mensagem enviada ao MQTT!");
      } else {
        Serial.println("Falha ao enviar mensagem ao MQTT!");
      }
      sendPostFall(receivedData);
    }

    else{
      sendPostRequest(receivedData);
    }
  }
}

void sendPostRequest(String data) {
  HTTPClient http;
  http.begin(espClient, "http://10.144.0.120:8000/insertentry");// ****checar endereço IP****
  http.addHeader("Content-Type", "application/json");

  float temperatura = data.toFloat();
  pox.update();
  /* float int int */
  char jsonData[100];
  sprintf(jsonData, "{\"temperature\": %f, \"oximetry\": 0, \"bpm\": 0}", temperatura);
  Serial.println(jsonData);
  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    Serial.println("Resposta do servidor:");
    Serial.println(http.getString());
  } else {
    Serial.print("Erro na requisição HTTP POST: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}
                
void sendPostFall(String data) {
  HTTPClient http;
  http.begin(espClient, "http://10.144.0.120:8000/insertentryfall");// ****checar endereço IP****
  http.addHeader("Content-Type", "application/json");

  char jsonData[100];
  sprintf(jsonData, "{}");

  int httpResponseCode = http.POST(jsonData);

  if (httpResponseCode > 0) {
    Serial.println("Resposta do servidor:");
    Serial.println(http.getString());
  } else {
    Serial.print("Erro na requisição HTTP POST: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}