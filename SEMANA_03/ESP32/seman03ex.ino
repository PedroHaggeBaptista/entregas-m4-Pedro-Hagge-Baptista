#define pinoSensor 1
#define pinoLed0 18
#define pinoLed1 17
#define pinoLed2 16
#define pinoLed3 15
#define buzzer 13
#define btn 12
#define btn2 11

int vetor[20];
int posAtualVetor = 0;

void setup() {
  //Defino todos os pinos que estão plugados, e suas funções
  pinMode(pinoLed0, OUTPUT);
  pinMode(pinoLed1, OUTPUT);
  pinMode(pinoLed2, OUTPUT);
  pinMode(pinoLed3, OUTPUT); 
  pinMode(buzzer, OUTPUT);
  pinMode(btn, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);

  // Inicialização do ESP32 
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
}

//Função para fazer a conversão de decimal para binário
void conversion(int num, int *vetorBinarios) {
  //Se o número passado for 0 ou 1 (estavam com problema) 
  //ele já define os quatro algorismos manualmente
  if (num == 0) {
    vetorBinarios[0] = 0;
    vetorBinarios[1] = 0;
    vetorBinarios[2] = 0;
    vetorBinarios[3] = 0;
  }
  //Instancição de um vetor para guardar o valor em binário, algorismo por algorismo
  int numBinario[4];
  int i=0;

  //Efetuo um for para gerar cada algorismo do binário
  for (i; i <= 3; i++){
    numBinario[i] = num % 2;
    num = num / 2;
  }

  //Passo cada valor do array que atualmente possui os algorismos do binário para 
  //o array que eu possuo com ponteiro nos argumentos da função
  for (int i = 0; i<=3; i++) {
    vetorBinarios[i] = numBinario[i];
  }
}

//Define qual será a frequência para o buzzer
void defineFrequency(int val) {
  int valFreq = 0;

  //Se a frequencia for zero já coloco uma frequência padrão
  if (val == 0) {
    valFreq = 176.220;
  } else {
    //Caso não seja zero, para gerar uma frequência única eu multiplico o número do LDR
    //por 100, obtendo assim uma freq para cada número 
    valFreq = val * 100;
  }
  //Faço o buzzer tocar com a freq
  tone(buzzer, valFreq);
}

//Armazena os decimais no vetor para tocar depois
int armazenaNoVetor(int valor, int *vetor, int posicao, int maxVetor) {
    Serial.println("Teste");
    //Esse if têm como intuito fazer com que seja impossível estrapolar os espaços alocados na memória para o vetor em questão
    if ((posicao + 1) < maxVetor) {
        //Seto nessa posição o valor passado do "sensor"
        vetor[posicao] = valor;
        //Adiciono mais 1 na posição para essa ser então a nova "ultima posição", que será retornada para a função "Dirige()"
        int ultimaPosicao = posicao + 1;
    //Retorno a última posição para que possa haver continuidade caso essa função seja chamada novamente
        return ultimaPosicao;
    } else {
    //Como não há mais espaços no vetor retorno a própria posição para travar a adição com essa função
        return posicao;
    }
}

//Função para ligar os leds
void turnOnLeds(int valLed1, int valLed2, int valLed3, int valLed4) {
  //Seto todos os leds para off
  digitalWrite(pinoLed0, LOW);
  digitalWrite(pinoLed1, LOW);
  digitalWrite(pinoLed2, LOW);
  digitalWrite(pinoLed3, LOW);
  
  //Se o primeiro algorismo do binário for 1 eu ligo o primeiro led
  if (valLed1 == 1) {
    digitalWrite(pinoLed0, HIGH);
  }

  //Se o segundo algorismo do binário for 1 eu ligo o segundo led
  if (valLed2 == 1) {
    digitalWrite(pinoLed1, HIGH);
  }

  //Se o terceiro algorismo do binário for 1 eu ligo o terceiro led
  if (valLed3 == 1) {
    digitalWrite(pinoLed2, HIGH);
  }

  //Se o quarto algorismo do binário for 1 eu ligo o quarto led
  if (valLed4 == 1) {
    digitalWrite(pinoLed3, HIGH);
  }
}

void loop() {

  //Defino o max de numeros que podem ficar salvos para serem tocados depois
  int maxVetor = 20;

  //Valor da frequência atual
  int valFreq = 0;

  //Vetor que receberá os algorismos do binário
  int vetorBinarios[4] = {0, 0, 0, 0};

  int vetorBinariosRepeat[4] = {0, 0, 0, 0};

  //Faço a leitura do sensor LDR (luz) e ajusto essa leitura para se localizar entre 0 e 15
  int valorLDR = 15 - round(analogRead(pinoSensor) / 273.4375);

  //Chamo a função de conversão
  conversion(valorLDR, vetorBinarios);

  //Chamo a função de definir a frequência
  defineFrequency(valorLDR);

  //Função que liga os leds de acordo com os algorismos do binário
  turnOnLeds(vetorBinarios[0], vetorBinarios[1], vetorBinarios[2], vetorBinarios[3]);

  //Caso o primeiro botão seja apertado
  if (digitalRead(btn) == 0) {
    while (digitalRead(btn) == 0) {}
    //Armazeno o decimal atual no vetor quando o botão é clicado
    posAtualVetor = armazenaNoVetor(valorLDR, vetor, posAtualVetor, maxVetor);
  }

  //Caso o segundo botão seja pressionado
  if(digitalRead(btn2) == 0){
    while (digitalRead(btn2) == 0) {}
    Serial.println("Apertou");
    //Paro o buzzer que está tocando na frêquencia do valor atual
    noTone(buzzer);
    //Faço um for que passe por todos os valores do vetor de valoresArmazenados
    for(int i = 0; i <= (maxVetor - 1); i++) {
      //Para que não haja tanta demora (como o array é incialmente definido com zeros)
      //ele ignora os valores 0 que estão no array
      if (vetor[i] == 0 ){
        continue;
      }
      //Para o resto eu mando ele definir a frequência do valor específico do array
      //e então toca-lo por 500ms, após ele passa ao próximo valor do array.
      conversion(vetor[i], vetorBinariosRepeat);

      defineFrequency(vetor[i]);

      turnOnLeds(vetorBinariosRepeat[0], vetorBinariosRepeat[1], vetorBinariosRepeat[2], vetorBinariosRepeat[3]);
      delay(1000);
      
      vetorBinariosRepeat[0] = 0;
      vetorBinariosRepeat[1] = 0;
      vetorBinariosRepeat[2] = 0;
      vetorBinariosRepeat[3] = 0;
    }
  }
  delay(50);
}
