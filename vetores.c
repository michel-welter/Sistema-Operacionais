#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <math.h>
int s;
int N = 10000;
int iteracoes;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_barrier_t barrier;
static pthread_barrier_t barrier1;
double novo[10000];
double velho[10000];

void *thread(void *num){
	int *id = num;
	int t = (int)*id;
	int n = N - 2; //quantidade de poscies que vao ser alteradas
	int d = 3; //número de threads
	int fatia = ceil(n/d); //numero inteiro    
	int inicio = t*fatia + 1;
	int fim = inicio + fatia -1;
	if(fim > (N-2))
		fim = N - 2;//no caso em que a última fatia seja menor que as demais
	for(int i=0;i<N;i++){
		if(i == N-1){
			velho[i] = 1;
			novo[i] = 1;
		}else{
			velho[i] = 0;
			novo[i] = 0;
		}
	}
	for(int j=0;j<iteracoes;j++){
		s = pthread_barrier_wait(&barrier);
		for(int i=1;i<=N-2;i++){
			novo[i] = (velho[i - 1] + velho[i + 1]) / 2;
		}
		s = pthread_barrier_wait(&barrier1);
		double aux[N];
		if(pthread_mutex_trylock(&mutex)==0){
			for(int i=0;i<N;i++){
				aux[i] = novo[i];
				novo[i] = velho[i];
				velho[i] = aux[i];
			}
			pthread_mutex_unlock(&mutex);	
		}
	}

}		
int main(){
  int i = 0;
  int numThreads;
  printf("digite quantas threads o programa usara:\n");
  scanf("%d",&numThreads);
  printf("digite quantas iteracoes vc quer:\n");
  scanf("%d", &iteracoes);
  int nThread[numThreads];
  for(int i=0;i<numThreads;i++){
  	nThread[i] = i;
  }
  pthread_t Athread[numThreads];
  		s = pthread_barrier_init(&barrier, NULL, numThreads);
  		s = pthread_barrier_init(&barrier1, NULL, numThreads);
    for (i = 0; i < numThreads; i++){
        pthread_create(&Athread[i], NULL, thread, &nThread[i]);
    }
    for (i = 0; i < numThreads; i++)
        pthread_join(Athread[i], NULL);//roda as thread ao mesmo tempo
    for(int i=0;i<N;i++){
			printf("{%lf}\n", velho[i]);
		}
    return (1);
}
