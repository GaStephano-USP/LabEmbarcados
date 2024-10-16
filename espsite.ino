#include <ESP8266WiFi.h>

// Configurações da rede Wi-Fi
const char* ssid = "Ingrid 2.4GHz";   // Substitua pelo nome da sua rede Wi-Fi
const char* password = "ilikechopin08"; // Substitua pela senha da sua rede

WiFiServer server(80); // Inicia o servidor na porta 80

void setup() {
  Serial.begin(115200);
  
  // Conectar ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");
  
  // Iniciar o servidor
  server.begin();
  Serial.println("Servidor HTTP iniciado");
  Serial.print("IP do ESP8266: ");
  Serial.println(WiFi.localIP()); // Exibe o IP no Serial
}

void loop() {
  // Verifica se um cliente se conectou
  WiFiClient client = server.available();
  if (client) {
    Serial.println("Novo cliente conectado");
    
    // Aguarda até que o cliente envie dados
    //while (client.connected() && !client.available()) {
    //  delay(1);
    //}
    //pq aguardar eu nn sei
    
    // Lê o valor do LM35
    int valorAnalogico = analogRead(A0); // Leitura do pino A0
    float temperaturaC = (valorAnalogico * 3.3 / 1024.0) * 100; // Converte para Celsius

    // Monta a resposta HTML
    String resposta = "HTTP/1.1 200 OK\r\n";
    resposta += "Content-Type: text/html\r\n";
    resposta += "Connection: close\r\n";
    resposta += "\r\n";
    resposta += "<!DOCTYPE HTML>";
    resposta += "<html><body>";
    resposta += "<h1>Temperatura</h1>";
    resposta += "<p>Temperatura: " + String(temperaturaC) + " °C</p>";
    resposta += "</body></html>";

    // Envia a resposta ao cliente
    client.print(resposta);
    
    // Desconecta o cliente
    client.stop();
    Serial.println("Cliente desconectado");
  }
}