#include <WiFi.h>
#include <string>
#include <ArduinoJson.h>
#include <HTTPClient.h>
#define pinLED1 12  //GP2 ESP-01
#define patID "123"
#define Message "Teste de Mensagem";
//WiFi
const char* SSID = "Inteli-COLLEGE";                // SSID / nome da rede WiFi que deseja se conectar
const char* PASSWORD = "QazWsx@123";  // Senha da rede WiFi que deseja se conectar
//MQTT Server
//Declaração das Funções
void mantemConexoes();  //Garante que as conexoes com WiFi e MQTT Broker se mantenham ativas
void conectaWiFi();     //Faz conexão com WiFi

char converter(char ch, int chave) {
  if (!isalpha(ch)) return ch;
  char offset = isupper(ch) ? 'A' : 'a';
  return (char)((((ch + chave) - offset) % 26) + offset);
}
String criptografar(String entrada, int chave) {
  String saida = "";
  size_t len = entrada.length();
  for (size_t i = 0; i < len; i++)
    saida += converter(entrada[i], chave);
  return saida;
}

StaticJsonDocument<200> doc;
String data;

void sendMessage() {
  HTTPClient http;

  doc["nomeAluno"] = "Pedro Hagge Baptista";
  doc["turma"] = "1";
  doc["grupo"] = "2";
  doc["mensagem"] = Message;

  String mensagem1 = Message

  String teste = criptografar(mensagem1, 12);

  doc["mensagemCripto"] = teste;

  serializeJson(doc, data);
  Serial.println(data);
  http.begin("https://ur524n-3000.preview.csb.app/teobaldo");
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(data);
  if(httpResponseCode>0){
    String response = http.getString();  //Get the response to the request
    Serial.println(httpResponseCode);   //Print return code
    Serial.println(response);           //Print request answer
  } else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
  http.end();
}
void setup() {
  pinMode(pinLED1, OUTPUT);
  Serial.begin(115200);
  tone(pinLED1, 2000, 1000);
  conectaWiFi();
  sendMessage();
}
void loop() {
  mantemConexoes();
}
void mantemConexoes() {
  conectaWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}
void conectaWiFi() {
  if (WiFi.status() == WL_CONNECTED) {
     return;
  }
  Serial.print("Conectando-se na rede: ");
  Serial.print(SSID);
  Serial.println("  Aguarde!");
  WiFi.begin(SSID, PASSWORD); // Conecta na rede WI-FI
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
      Serial.print(".");
  }
  Serial.println();
  Serial.print("Conectado com sucesso, na rede: ");
  Serial.print(SSID);
  Serial.print("  IP obtido: ");
  Serial.println(WiFi.localIP());
}