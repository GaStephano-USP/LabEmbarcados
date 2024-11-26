#include <ESP8266WiFi.h> 
#include <PubSubClient.h> 



const char* ssid = "Nome da Rede do Igor aq";                                
const char* password = "senha da rede do Igor aq";                                    
const char* mqtt_server = "broker.mqtt-dashboard.com";                  
const char* mqttTopic = "DotHealth/alarme"

WiFiClient espClient;                                                   
PubSubClient client(espClient);                                         
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup() {                                                          
  pinMode(BUILTIN_LED, OUTPUT);                                         
  Serial.begin(115200);                                                 
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