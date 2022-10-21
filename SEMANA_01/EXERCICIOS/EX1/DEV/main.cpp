#include <iostream>
#include <string>

// // 1 -  Faça uma função que recebe uma certa medida e ajusta ela percentualmente 
// // entre dois valores mínimo e máximo e retorna esse valor

// // 2 - Faça uma função que simule a leitura de um sensor lendo o 
// // valor do teclado ao final a função retorna este valor

// // 3 - Faça uma função que armazena uma medida inteira qualquer 
// // em um vetor fornecido. Note que como C não possui vetores 
// // nativos da linguagem, lembre-se que você precisa passar o 
// // valor máximo do vetor assim como a última posição preenchida
// // Evite também que, por acidente, um valor seja escrito em 
// // uma área de memória fora do vetor



// // 4 - Faça uma função que recebe um vetor com 4 posições que contém 
// // o valor da distância de um pequeno robô até cada um dos seus 4 lados.
// // A função deve retornar duas informações: A primeira é a direção 
// // de maior distância ("Direita", "Esquerda", "Frente", "Tras") e a 
// // segunda é esta maior distância.




// // 5 - Faça uma função que pergunta ao usuário se ele deseja continuar o mapeamento e 
// // retorna verdadeiro ou falso


// // 6 - A função abaixo (que está incompleta) vai "dirigindo" virtualmente um robô 
// // e através de 4 sensores em cada um dos 4 pontos do robo ("Direita", "Esquerda", 
// // "Frente", "Tras"). 
// //      A cada passo, ele verifica as distâncias aos objetos e vai mapeando o terreno 
// // para uma movimentação futura. 
// //      Ele vai armazenando estas distancias em um vetor fornecido como parâmetro 
// // e retorna a ultima posicao preenchida do vetor.
// //      Esta função deve ir lendo os 4 sensores até que um comando de pare seja enviado 
// //
// //      Para simular os sensores e os comandos de pare, use as funções já construídas 
// // nos ítens anteriores e em um looping contínuo até que um pedido de parada seja 
// // enviado pelo usuário. 
// //
// //      Complete a função com a chamada das funções já criadas

// using namespace std;

int converteSensor(int val, int min, int max) {
    float conta1 = ((val - min) * 100);
    float conta2 = (max - min);

    double resul = conta1 / conta2;

    return resul;
}

int leituraSensor(int dir) {
	//Verifico qual a direção que ele deseja saber para efetuar a pergunta correta
	if (dir == 1) {
		int leituraMedidaSensorFrente = 0;
		printf("Informe a distancia para a frente: "); //Pergunto no conseole a distância(simulação do sensor)
		scanf("%i", &leituraMedidaSensorFrente);
		return leituraMedidaSensorFrente;
	} else if (dir == 2) {
		int leituraMedidaSensorTras = 0;
		printf("Informe a distancia para a tras: "); //Pergunto no conseole a distância(simulação do sensor)
		scanf("%i", &leituraMedidaSensorTras);
		return leituraMedidaSensorTras;
	} else if (dir == 3) {
		int leituraMedidaSensorEsquerda = 0;
		printf("Informe a distância para a esquerda: "); //Pergunto no conseole a distância(simulação do sensor)
		scanf("%i", &leituraMedidaSensorEsquerda);
		return leituraMedidaSensorEsquerda;
	} else if (dir == 4) {
		int leituraMedidaSensorDireita = 0;
		printf("Informe a distancia para a direita: "); //Pergunto no conseole a distância(simulação do sensor)
		scanf("%i", &leituraMedidaSensorDireita);
		return leituraMedidaSensorDireita;
	} else {
		return 0;
	}
}

int armazenaNoVetor(int valor, int *vetor, int posicao, int max) {
	//Esse if têm como intuito fazer com que seja impossível estrapolar os espaços alocados na memória para o vetor em questão
	if ((posicao + 1) < max) {
		//Seto nessa posição o valor passado do "sensor"
		vetor[posicao] = valor;
		//Adiciono mais 1 na posição para essa ser então a nova "ultima posição", que será retornada para a função "Dirige()"
		int ultimaPosicao = posicao + 1;
		return ultimaPosicao;
	} else {
		return posicao;
	}
}

char* direcaoMaiorCaminho(int *vetor, int *dir) {
	int index = 0;
	int pos = vetor[0]; //Até então o primeiro valor é o maior

	for (int i = 1; i < 4; i++) { //Efetuou uma busca pelos quatro valores do vetor (esquerda, direira, frente, trás), e verificar entre eles qual é o maior
		if (vetor[i] > pos) { //Caso seja maior que o até então posicionado na váriavel acima
			pos = vetor[i]; //Seto então esse como o novo "maior valor"
			index = i; //Seto então esse como o index do novo "maior valor"
		}
	}

	*dir = pos; //Defino o ponteiro do paremetro com a maior distância

	//Utilizo esse if para então retornar qual a direção relacionada ao "maior valor"
	//Utilizo o index, pois esse varia de 0 a 4, sendo cada um relativo a uma direção
	if (index == 0) {
		return (char*) "Direita";
	} else if (index == 1) {
		return (char*) "Esquerda";
	} else if (index == 2) {
		return (char*) "Tras";
	} else if (index == 3) {
		return (char*) "Frente";
	} else {
		return (char*) "Nao encontrado";
	}
}

int leComando() {
	int respostaUser = 0;

	//Pergunto no console se deseja continuar
    printf("Voce deseja continuar? (1 - S/0 - N): ");
    scanf("%i", &respostaUser);

    if (respostaUser == 1) { //Se deseja ou não continuar, retorno os valores que irão servir como controle do WHILE da função "Dirige()" 
        return 1;
    };

	return 0;
};

int dirige(int *v,int maxv){
	int maxVetor = maxv;
	int *vetorMov = v;
	int posAtualVetor = 0;
	int dirigindo = 1;

	//Controla quantas iterações irão ocorrer (sequencia de direita, esquerda, frente, trás)
	while(dirigindo){
		//Efetuando medição, conversão e armazenamento para a direita
		int medidaDirei = leituraSensor(4); //Chama a leitura de sensor
		medidaDirei = converteSensor(medidaDirei,0,830); //Converte a leitura
		posAtualVetor = armazenaNoVetor(medidaDirei, vetorMov, posAtualVetor, maxVetor); //Armazena o valor da leitura no vetor geral

		//Efetuando medição, conversão e armazenamento para a esquerda
		int medidaEsquer = leituraSensor(3);
		medidaEsquer = converteSensor(medidaEsquer,0,830);
		posAtualVetor = armazenaNoVetor(medidaEsquer, vetorMov, posAtualVetor, maxVetor);

		//Efetuando medição, conversão e armazenamento para a frente
		int medidaFrente = leituraSensor(2);
		medidaFrente = converteSensor(medidaFrente,0,830);
		posAtualVetor = armazenaNoVetor(medidaFrente, vetorMov, posAtualVetor, maxVetor);

		//Efetuando medição, conversão e armazenamento para a tras
		int medidaTras = leituraSensor(1);
		medidaTras = converteSensor(medidaTras,0,830);
		posAtualVetor = armazenaNoVetor(medidaTras, vetorMov, posAtualVetor, maxVetor);

		dirigindo = leComando();
	}
	return posAtualVetor;
}


// O trecho abaixo irá utilizar as funções acima para ler os sensores e o movimento
// do robô e no final percorrer o vetor e mostrar o movimento a cada direção baseado 
// na maior distância a cada movimento
void percorre(int *v,int tamPercorrido){		
	int *vetorMov = v;
	int maiorDir = 0;
	
	//Faço um for que irá conter cada uma das iterações (esquerda, direita, frente e trás), por isso o i+=4
	for(int i = 0; i< tamPercorrido; i+=4) {

		//Chamo a função de maior direção passando um vetor com uma iteração, assim ele descobre dentro da iteração para qual direção há a maior distância
		char *direcao = direcaoMaiorCaminho(&(vetorMov[i]),&maiorDir);
		printf("Movimentando para %s distancia = %i\n",direcao,maiorDir);
	}
}

int main(int argc, char** argv) {
	int maxVetor = 100;
	int vetorMov[maxVetor*4];
	int posAtualVet = 0;
	
	//Chamo a função respónsavel por fazer o robo se locomover
	posAtualVet = dirige(vetorMov,maxVetor);

	//Chama a função que verificará para cada iteração para qual direção o robo deve andar e quanto deve andar
	percorre(vetorMov,posAtualVet);
	
	return 0;
}