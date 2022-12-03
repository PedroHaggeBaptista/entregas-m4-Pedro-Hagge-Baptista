#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Definição de pinagens
int LED_RED_RESULT = 12;
int LED_BLUE_RESULT = 18;
int LED_WHITE_RESULT = 10;
int buzzerPin = 6;

//Váriaveos de controle de pontos
int pontosJog1 = 0;
int pontosJog2 = 0;

//Definição das portas para o LCD
#define I2C_SDA 17 
#define I2C_SCL 16

//Instanciação do WebServer e definição das chaves do Rede Wi-Fi
WebServer server(80);
const char* ssid = "Inteli-COLLEGE";
const char* password = "QazWsx@123";


//Instanciação do LCD
LiquidCrystal_I2C  lcd_i2cBase(0x27, 16, 2);

//Definição de classe para controle do LCD (possui métodos para facilitar a escrita no LCD)
class MostradorLCD {
  private:
    LiquidCrystal_I2C *lcd_i2c;
  public:
    //Para cada método, basta passar o texto que deseja exibir no LCD e cada um exibe de uma forma diferente
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

    //Método para voltar o LCD para a tela inicial (Aguardando jogadores)
    void resetLCD(){
      lcd_i2c->setCursor(0, 0);
      lcd_i2c->printf("Aguar. Jogadores");
    };

    //Método para limpar o LCD
    void clearLCD(){
      lcd_i2c->clear();
    };

    //Método para exibir o placar no LCD
    void displayResults(int pontos1, int pontos2) {
      lcd_i2c->setCursor(0, 0); 
      lcd_i2c->printf("Pontos1: %i",pontos1);
      lcd_i2c->setCursor(0, 1); 
      lcd_i2c->printf("Pontos2: %i",pontos2);
    }
};
MostradorLCD *lcd = NULL;


//Função chamada quando a rota /jogador1 é chamada(quando jogador 1 venceu)
void jogador1wins() {
  //Toca som para alertar
  tone(buzzerPin, 4000, 200);
  tone(buzzerPin, 3500, 200);
  tone(buzzerPin, 3000, 200);
  tone(buzzerPin, 2500, 200);

  //Incrementa o placar do jogador 1
  pontosJog1 = pontosJog1 + 1;
  //Liga o LED vermelho para indicar que o jogador 1 venceu
  digitalWrite(LED_RED_RESULT, HIGH);

  //Exibe o vencedor no LCD
  lcd->clearLCD();
  lcd->mostraL1("Jog. 1 Vencedor");
  delay(2000);
  digitalWrite(LED_RED_RESULT, LOW);

  //Exibe o placar no LCD
  lcd->clearLCD();
  lcd->displayResults(pontosJog1, pontosJog2);
  delay(4000);
  lcd->clearLCD();

  server.send(200, "text/plain", "Jogador 1 venceu");
}

//Função chamada quando a rota /jogador2 é chamada(quando jogador 2 venceu)
void jogador2wins() {
  //Toca som para alertar
  tone(buzzerPin, 4000, 200);
  tone(buzzerPin, 3500, 200);
  tone(buzzerPin, 3000, 200);
  tone(buzzerPin, 2500, 200);

  //Incrementa o placar do jogador 2
  pontosJog2 = pontosJog2 + 1;

  //Liga o LED azul para indicar que o jogador 2 venceu
  digitalWrite(LED_BLUE_RESULT, HIGH);

  //Exibe o vencedor no LCD
  lcd->clearLCD();
  lcd->mostraL1("Jog. 2 Vencedor");
  delay(2000);
  digitalWrite(LED_BLUE_RESULT, LOW);

  //Exibe o placar no LCD
  lcd->clearLCD();
  lcd->displayResults(pontosJog1, pontosJog2);
  delay(4000);
  lcd->clearLCD();
  server.send(200, "text/plain", "Jogador 2 venceu");
}

//Função chamada quando a rota /empate é chamada(quando deu empate)
void empate() {
  //Toca som para alertar
  tone(buzzerPin, 4000, 200);
  tone(buzzerPin, 3500, 200);
  tone(buzzerPin, 3000, 200);
  tone(buzzerPin, 2500, 200);

  //Liga o LED branco para indicar o empate
  digitalWrite(LED_WHITE_RESULT, HIGH);

  //Exibe o empate no LCD
  lcd->clearLCD();
  lcd->mostraL1("Empate");
  delay(2000);
  digitalWrite(LED_WHITE_RESULT, LOW);

  //Exibe o placar no LCD
  lcd->clearLCD();
  lcd->displayResults(pontosJog1, pontosJog2);
  delay(4000);
  lcd->clearLCD();
  server.send(200, "text/plain", "Empate");
}

//Função chamada quando a rota /testConnect é chamada(testar a conectividade)
void testConnection() {
  //Toca som para alertar
  tone(buzzerPin, 2000, 1500);

  //Exibe a mensagem no LCD(para teste)
  lcd->clearLCD();
  lcd->mostraL1("Conexao Testada");
  delay(3000);
  server.send(200, "text/plain", "Testada");
}

//Quando a rota chamada não foi definida na função SETUP
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

  //Inicializa as entradas e saídas
  pinMode(LED_BLUE_RESULT, OUTPUT);
  pinMode(LED_RED_RESULT, OUTPUT);
  pinMode(LED_WHITE_RESULT, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  //faz o Setup do WiFi
  WiFi.mode(WIFI_STA);  //Optional
  WiFi.begin(ssid, password);

  //Faz o setup do LCD
  Wire.begin(I2C_SDA, I2C_SCL);
  //------------------------//
  lcd = new MostradorLCD(&lcd_i2cBase);

  //Tenta conexao com o WiFi
  Serial.println("\nConectando ao WiFi");
  //Exibe a tentativa de conexao no LCD
  lcd->mostraL1("Conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(50);
  }
  lcd->clearLCD();

  //Mostra o IP recebido pelo ESP no Serial
  Serial.println("\nConectado com a rede WiFi");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  //Define as rotas do servidor e quais funções cada uma irá chamar
  server.on("/jogador1", jogador1wins);
  server.on("/jogador2", jogador2wins);
  server.on("/empate", empate);
  server.on("/testeConnect", testConnection);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();

  //Exibe o IP recebido pelo ESP no LCD
  lcd->mostraL1IP(WiFi.localIP().toString());
  delay(5000);
  lcd->clearLCD();
}


void loop() {
  //Fica checando a todo momento se a conexão com o WiFi caiu ou não
  server.handleClient();
  if (WiFi.status() != WL_CONNECTED) {
    //Caso a conexao tenha caido ele informa no LCD para que algo seja feito
    lcd->mostraL1("Conexao perdida");
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");
    delay(1000);
  } else {
    //Caso a conexao tenha sido reestabelecida ele informa no LCD, voltando o mesmo para o status de Aguardando Conexão
    lcd->resetLCD();
  }

  delay(100);
}