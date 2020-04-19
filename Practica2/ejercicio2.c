#include <pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

int canthilos=4;
int numabuscar=6;
int N=16;
int arreglo[16];
int apariciones=0;
pthread_mutex_t task_queue_lock;


void *multiplica(void *id) {
  int i;
  int cant=0;
  int threadId = (int) id;
  printf("arranco hilo en %d \n", (threadId*(N/canthilos)));
  for(i = (threadId*(N/canthilos)); i < ((threadId+1)*(N/canthilos)); i++){
   	if(arreglo[i]==numabuscar){
   		printf("encontre\n");
   		cant++;
   	}
   	printf("%d\n", arreglo[i]);
  }
  pthread_mutex_lock(&task_queue_lock);
  	apariciones += cant;
  pthread_mutex_unlock(&task_queue_lock);
}

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc,char*argv[]){
 


 int i;

 for (i = 0; i < N; i++){
 	arreglo[i] = rand() % 10;
 	printf("num: %d \n", arreglo[i]);
 }

 double timetick;


timetick = dwalltime();
pthread_t threads[canthilos];
pthread_attr_t attr;
int ids[canthilos];

pthread_mutex_init(&task_queue_lock, NULL);

for (i = 0; i < canthilos; i++)
  pthread_create(&threads[i], NULL, multiplica, (void *) (__intptr_t) i);
 
for (i = 0; i < canthilos; i++)
  pthread_join(threads[i], NULL);


 printf("Buscar apariciones en un arreglo de dimension %d. Tiempo en segundos %f\n",N, dwalltime() - timetick);
 printf("Apariciones: %d\n", apariciones);

 return(0);

}