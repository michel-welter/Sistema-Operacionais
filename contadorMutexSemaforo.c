
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define max 15
#define prox (i + 1) % 3 

void *thread(void *data);
void incrimenta(int);

int contador = 0;
int contAux = 0;
sem_t thread_sem[3];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread(void *num){
   
    for(;;){
        int *id = num;
        for(int i=0; i< 1000000; ++i){
            for(int j=0;j<150;++j){
                // just for wasting some time
            }
        }  
        if(*id == contAux){    
        	incrimenta(*id);
        }else{
          sem_wait(&thread_sem[*id]);
        }
        for(int i=0; i< 1000000; ++i){
            for(int j=0; j<150;++j){
                // just for wasting some time
            }
        }     
    }
}

void incrimenta(int i){

    pthread_mutex_lock(&mutex);//entra na zona critica tranca mutex
  	
    contador++;
    contAux = prox;
    printf("thread %d = %d\n",i,contador);
    pthread_mutex_unlock(&mutex);// sai da zona critica libera mutex
    sem_post(&thread_sem[prox]);
    if(contador == max){
      printf("final do contador = %d\n",contador);
      exit(EXIT_SUCCESS);
    }
}

int main(){
  int nThread[3] = {0, 1, 2};
  int i = 0;
  pthread_t Athread[3];
  for (i = 0; i < 3; i++)
        sem_init(&thread_sem[i], 0, 0); // inicializa o semáforo com valor 0
    for (i = 0; i < 3; i++){
        pthread_create(&Athread[i], NULL, thread, &nThread[i]);
    }
    for (i = 0; i < 3; i++)
        pthread_join(Athread[i], NULL);//roda as thread ao mesmo tempo
    return (1);
}

