#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
int cadeirasEspera;
int clientes;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition_var = PTHREAD_COND_INITIALIZER;
sem_t esperas;
sem_t barbeiro;


void *thread(void *num){
	int *id = num;
	if(*id == clientes){
		for(;;){
			sem_wait(&barbeiro);
			pthread_mutex_lock(&mutex);
			cadeirasEspera += 1;
			pthread_mutex_unlock(&mutex);
			for(int i=0; i< 10000000; ++i);
			printf("cortou cabelo\n");
			pthread_cond_signal( &condition_var );
		}
	}
	for(;;){
		pthread_mutex_lock(&mutex);
		if(cadeirasEspera>0){
			cadeirasEspera -= 1;
			sem_post(&barbeiro);
			pthread_cond_wait( &condition_var, &mutex);
			printf("thread %d cortou cabelo\n",*id);
			pthread_mutex_unlock(&mutex);
		}else{
			pthread_mutex_unlock(&mutex);
			printf("sem cadeiras para a thread %d\n", *id);
			for(int i=0; i< 100000000; ++i);
		}
	}

}	



int main(){
  int i = 0;
	for(;;){
  	printf("digite quantas cadeiras de espera vai ter:\n");
  	scanf("%d",&cadeirasEspera);
  	printf("quantos clientes vao ter?:\n");
  	scanf("%d", &clientes);
  	if(clientes>cadeirasEspera+1){
  		break;
  	}else{
  		printf("quantidade de clientes menor que numero de cadeiras+1 digite novamente\n");
  	}	
	}  
  pthread_t Athread[clientes+1];
	int nThread[clientes+1];
   for(int i=0;i<=clientes;i++){
  	nThread[i] = i;
  }

    for (i = 0; i <clientes; i++){
        pthread_create(&Athread[i], NULL, thread, &nThread[i]);
    }
    pthread_create(&Athread[clientes], NULL, thread, &nThread[clientes]);
    for (i = 0; i <clientes; i++)
        pthread_join(Athread[i], NULL);//roda as thread ao mesmo tempo

    return (1);
}
