
/*importa as bibliotecas a serem usadas e define o numero de threads*/
#include <stdio.h> 
#include <stdlib.h> // permite trablhar com strings
#include <pthread.h> //permite trabalhar com threads

#define num_threads 100

/*aloca as threads,as contas, e o mutex*/
pthread_t threads[num_threads];
//transfs serve para identificacao do numero de cada thread
int c1=100,c2=100, transfs[num_threads];

//semaforo
pthread_mutex_t mutex;

/*as funcoes a definem o comportamento das thread baseado em seu numero*/

//metodo que a thread impar ira executar
void *threadPar(void *arg)//threads pares
{
	int* i = (int*)arg;	//salva o numero da thread
	int valor = (rand()%50)+1; // escolhe um valor aleatório para a transferencia
	printf("tranferencia %d c1 => c2, R$ %d\n",*i,valor);
	pthread_mutex_lock(&mutex);//bloqueia o mutex
	if(valor <= c1){ // verifica se possui saldo suficiente para a tranferencia
		c1 -= valor; //decresce valor da conta que esta mandando
		c2 += valor; // acrescenta valor na conta que esta recebendo
	}
	pthread_mutex_unlock(&mutex); // libera o mutex
}
//metodo que a thread impar ira executar
void *threadImpar(void *arg)//threads impares
{
	int* i = (int*)arg;//salva o numero da thread
	int valor = num_threads%(*i+1);// escolhe um valor randomico para a transferencia
	printf("tranferencia %d c2 => c1, R$ %d\n",*i,valor);
	pthread_mutex_lock(&mutex);//bloqueia o mutex
	if(valor <= c2){// verifica se ha saldo o suficiente para a tranferencia
		c2 -= valor;//decresce valor da conta que esta mandando
		c1 += valor; // acrescenta valor na conta que esta recebendo
	}
	pthread_mutex_unlock(&mutex);// libera o mutex
}

//funcao principal
int main()
{
	int i;
	pthread_mutex_init(&mutex,NULL);// inicializa o mutex
	
	// inicializa todas as threads
	for(i=0;i<num_threads;i++){ 
		transfs[i] = i;//salva o numero da thread
		
		if(i%2 == 0){//calcula se a thread é par
			//chamada da funcao thread par
			pthread_create(&threads[i], NULL, &threadPar , (void *) &transfs[i]);//inicializa threads pares informando seu numero
		}
		else{
			//chamada da funcao thread impar
			pthread_create(&threads[i], NULL, &threadImpar , (void *) &transfs[i]);//inicializa threads impares informando seu numero
		}
	}
	//encerra todas as threads
	for(i=0;i<num_threads;i++){
		pthread_join(threads[i],NULL); // junta as thread e as mata uma a uma
		printf("fim transacao %d\n", i);
	}
	//encerra o mutex
	pthread_mutex_destroy(&mutex);
	printf("saldo final c1: %d\nsaldo final c2: %d\n",c1,c2);

	return 0;
}
