#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
// #include <ESPmDNS.h>
// #include <ESP32Servo.h>
int LED_YELLOW_PIN = 21;
int LED_RED_PIN = 20;
int LED_RED_RESULT = 12;
int LED_BLUE_RESULT = 18;
int LED_WHITE_RESULT = 10;
int buzzerPin = 6;

int pontosJog1 = 0;
int pontosJog2 = 0;

#define I2C_SDA 17 
#define I2C_SCL 16

WebServer server(80);
const char* ssid = "Inteli-COLLEGE";
const char* password = "QazWsx@123";

LiquidCrystal_I2C  lcd_i2cBase(0x27, 16, 2);
//////////////////////////////
class MostradorLCD {
  private:
    LiquidCrystal_I2C *lcd_i2c;
  public:
    MostradorLCD (LiquidCrystal_I2C *lcd){
      lcd_i2c = lcd;     
      lcd_i2c->init(); // initialize the lcd
      lcd_i2c->backlight();

    };
    void mostraL1(char *texto){
      //Serial.printf("L1: %s\n",texto);
      lcd_i2c->setCursor(0, 0); 
      lcd_i2c->printf(texto);
    };
    void mostraL1Dest(char *texto){
     // Serial.printf("L1 Destino: %s\n",texto);
      lcd_i2c->setCursor(0, 0); 
      lcd_i2c->printf("->%s ",texto);
    };
    void mostraL1IP(char *ip){
   //   Serial.printf("IP: %s\n",ip);
      lcd_i2c->setCursor(0, 0); 
      lcd_i2c->printf("IP:%s",ip);
    };
    void mostraL1IP(String ip){
   //   Serial.printf("IP: %s\n",ip);
      lcd_i2c->setCursor(0, 0); 
      lcd_i2c->printf("IP:%s",ip);
    };
    void mostraL2(char *texto){
      lcd_i2c->setCursor(0, 1); 
      lcd_i2c->printf(texto);
    };
    void mostraL2Msg(char *texto){
      lcd_i2c->setCursor(0, 1); 
      lcd_i2c->printf("Msg: %s",texto);
    };
    void resetLCD(){
      lcd_i2c->setCursor(0, 0);
      lcd_i2c->printf("Aguar. Jogadores");
    };
    void clearLCD(){
      lcd_i2c->clear();
    };
    void displayResults(int pontos1, int pontos2) {
      lcd_i2c->setCursor(0, 0); 
      lcd_i2c->printf("Pontos1: %i",pontos1);
      lcd_i2c->setCursor(0, 1); 
      lcd_i2c->printf("Pontos2: %i",pontos2);
    }
};
MostradorLCD *lcd = NULL;

void jogador1wins() {
  tone(buzzerPin, 4000, 200);
  tone(buzzerPin, 3500, 200);
  tone(buzzerPin, 3000, 200);
  tone(buzzerPin, 2500, 200);
  pontosJog1 = pontosJog1 + 1;
  digitalWrite(LED_RED_RESULT, HIGH);
  lcd->clearLCD();
  lcd->mostraL1("Jog. 1 Vencedor");
  delay(2000);
  digitalWrite(LED_RED_RESULT, LOW);
  lcd->clearLCD();
  lcd->displayResults(pontosJog1, pontosJog2);
  delay(4000);
  lcd->clearLCD();

  server.send(200, "text/plain", "Jogador 1 venceu");
}

void jogador2wins() {
  tone(buzzerPin, 4000, 200);
  tone(buzzerPin, 3500, 200);
  tone(buzzerPin, 3000, 200);
  tone(buzzerPin, 2500, 200);
  pontosJog2 = pontosJog2 + 1;
  digitalWrite(LED_BLUE_RESULT, HIGH);
  lcd->clearLCD();
  lcd->mostraL1("Jog. 2 Vencedor");
  delay(2000);
  digitalWrite(LED_BLUE_RESULT, LOW);
  lcd->clearLCD();
  lcd->displayResults(pontosJog1, pontosJog2);
  delay(4000);
  lcd->clearLCD();
  server.send(200, "text/plain", "Jogador 2 venceu");
}

void empate() {
  tone(buzzerPin, 4000, 200);
  tone(buzzerPin, 3500, 200);
  tone(buzzerPin, 3000, 200);
  tone(buzzerPin, 2500, 200);
  digitalWrite(LED_WHITE_RESULT, HIGH);
  lcd->clearLCD();
  lcd->mostraL1("Empate");
  delay(2000);
  digitalWrite(LED_WHITE_RESULT, LOW);
  lcd->clearLCD();
  lcd->displayResults(pontosJog1, pontosJog2);
  delay(4000);
  lcd->clearLCD();
  server.send(200, "text/plain", "Empate");
}

void testConnection() {
  tone(buzzerPin, 2000, 1500);
  lcd->clearLCD();
  lcd->mostraL1("Conexao Testada");
  delay(3000);
  server.send(200, "text/plain", "Testada");
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_YELLOW_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_BLUE_RESULT, OUTPUT);
  pinMode(LED_RED_RESULT, OUTPUT);
  pinMode(LED_WHITE_RESULT, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  WiFi.mode(WIFI_STA);  //Optional
  WiFi.begin(ssid, password);

  Wire.begin(I2C_SDA, I2C_SCL);
  //------------------------//
  lcd = new MostradorLCD(&lcd_i2cBase);

  Serial.println("\nConnecting");
  lcd->mostraL1("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(50);
  }
  lcd->clearLCD();
  digitalWrite(LED_RED_PIN, LOW);
  digitalWrite(LED_YELLOW_PIN, HIGH);
  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
  server.on("/jogador1", jogador1wins);
  server.on("/jogador2", jogador2wins);
  server.on("/empate", empate);
  server.on("/testeConnect", testConnection);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  lcd->mostraL1IP(WiFi.localIP().toString());
  delay(5000);
  lcd->clearLCD();
}
void loop() {
  server.handleClient();
  if (WiFi.status() != WL_CONNECTED) {
    lcd->mostraL1("Conexao perdida");
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");
    delay(1000);
  } else {
    digitalWrite(LED_YELLOW_PIN, HIGH);
    digitalWrite(LED_RED_PIN, LOW);
    lcd->resetLCD();
  }

  delay(100);
}