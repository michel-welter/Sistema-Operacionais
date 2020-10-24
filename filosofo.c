#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

#define N 5 // numero de thread
#define pensando 0
#define com_fome 1
#define comendo 2
#define left (i + N - 1) % N //pega garfo da esquerda
#define right (i + 1) % N    //pega garfo da direita

void *thread(void *data);
void pega_garfo(int);
void libera_garfo(int);
void teste(int);

int nfilosofo[N] = {0, 1, 2, 3, 4};
int situacao[N] = {0, 0, 0, 0, 0}; // situacao dos filosofos
sem_t phil_sem[N];              // semaforo de cada filosofo
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; //inicializa o mutex

void *thread(void *num){
   
    for(;;){
        int *id = num;
        for(int i=0; i< 1000000; ++i){
            for(int j=0;j<150;++j){
                // just for wasting some time
            }
            
            
        }   
            
        pega_garfo(*id);
        for(int i=0; i< 1000000; ++i){
            for(int j=0; j<150;++j){
                // just for wasting some time
            }
        }     
        libera_garfo(*id);
    }
}

void pega_garfo(int i){

    pthread_mutex_lock(&mutex);//entra na zona critica tranca mutex
    situacao[i] = com_fome; //atualiza situacao
    printf("Filosofo %d ta com fome.\n", i);
    teste(i);
    pthread_mutex_unlock(&mutex);// sai da zona critica libera mutex
    sem_wait(&phil_sem[i]); //diminui o semaforo do filosofo
   
}

void libera_garfo(int i){

    pthread_mutex_lock(&mutex);//entra em zona critica tranca mutex
    situacao[i] = pensando; //atualiza situacao
    printf("Filosofo %d liberou os garfos %d e %d.\n", i, left, right);
    printf("Filosofo %d esta pensando\n", i);
    teste(left);
    teste(right);
    pthread_mutex_unlock(&mutex);//sai do zona critica libera mutex
}

void teste(int i){

    if (situacao[i] == com_fome && situacao[left] != comendo && situacao[right] != comendo){//testa se ele consegue comer

        situacao[i] = comendo; //atualiza situacao
        printf("Filosofo %d esta com os garfos %d e %d.\n", i, left, right);
        printf("Filosofo %d esta comendo.\n", i);
        sem_post(&phil_sem[i]);
    }
}

int main(void){

    int i = 0;
    pthread_t philosopher[N];
    for (i = 0; i < N; i++)
        sem_init(&phil_sem[i], 0, 0); // inicializa o semáforo com valor 0
    for (i = 0; i < N; i++){

        pthread_create(&philosopher[i], NULL, thread, &nfilosofo[i]);
        printf("Filosofo %d esta pensando.\n", i);
    }
    for (i = 0; i < N; i++)
        pthread_join(philosopher[i], NULL);//roda as thread ao mesmo tempo
    return (1);
}