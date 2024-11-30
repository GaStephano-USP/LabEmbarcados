#include <ESP8266WiFi.h> 
#include <PubSubClient.h> 
#include <ESP8266HTTPClient.h> 

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
  sendPostRequest("");
  Serial.print("fmandou pro banco"); 
                         
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
    //reconnect();
    //COMENTEI PRA TESTAR O ENVIO DE DADOS PRO BANCO
  }
  client.loop();                                                         

   // Verifica se há dados recebidos na porta serial (RX)
  if (Serial.available() > 0) {
    String receivedData = Serial.readStringUntil('\n'); // Lê até o caractere de nova linha
    receivedData.trim(); // Remove espaços extras ou quebras de linha

    Serial.print("Recebido no RX: ");
    Serial.println(receivedData);

    // Publica a mensagem recebida no tópico MQTT
    if (client.publish(mqttTopic, receivedData.c_str())) {
      Serial.println("Mensagem enviada ao MQTT!");
    } else {
      Serial.println("Falha ao enviar mensagem ao MQTT!");
    }
  }
}

void sendPostRequest(String data) {
  Serial.println("To na funcao");
  HTTPClient http;
  http.begin(espClient, "http://192.168.15.8:8000/insertentry");
  http.addHeader("Content-Type", "application/json");

  String jsonData = "{\"temperature\": 0, \"oximetry\": 0, \"bpm\": 0, \"ecg\": [0]}";
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
                