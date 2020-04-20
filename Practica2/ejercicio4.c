#include <pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

int canthilos=4;
int N=20;
int arreglo[20];
int maxTotal = 0;
int minTotal = 1000;
pthread_mutex_t task_queue_min;
pthread_mutex_t task_queue_max;


void *multiplica(void *id) {
  int maxLocal = 0;
  int minLocal = 1000;
  int threadId = (int) id;
  //recorro verificando min y max sobre mi parte del arreglo
  for(int i = (threadId*(N/canthilos)); i < ((threadId+1)*(N/canthilos)); i++){
   	if(arreglo[i] > maxLocal) maxLocal = arreglo[i] ;
    if(arreglo[i] < minLocal) minLocal = arreglo[i] ;
  }
  printf("Mi minimoo local del hilo %d %d\n",threadId, minLocal);
  printf("Mi maximo local del hilo %d %d\n",threadId, maxLocal);
  //ACA SE PODRIA MEJORAR LA EFICIENCIA? PREGUNTANDO POR EL MIN SI ESTA OCUPADO
  
  //verifico maximo local contra global
  pthread_mutex_lock(&task_queue_max);
  if(maxLocal > maxTotal){  maxTotal = maxLocal ;  printf("Hilo %d  actualizo el maxmino con %d\n",threadId, maxLocal);}
  pthread_mutex_unlock(&task_queue_max);

  //verifico minimo local contra global
  pthread_mutex_lock(&task_queue_min);
  	if( minLocal < minTotal ) { minTotal = minLocal ; printf("Hilo %d  actualizo el minimo con %d\n",threadId, minLocal);}
  pthread_mutex_unlock(&task_queue_min);
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
 srand ( time(NULL) );
 //srand genera la semilla random
 for (i = 0; i < N; i++){
 	arreglo[i] = rand() % 1000;
 }

 double timetick;

timetick = dwalltime();
pthread_t threads[canthilos];
pthread_attr_t attr;
int ids[canthilos];

pthread_mutex_init(&task_queue_min, NULL);
pthread_mutex_init(&task_queue_max, NULL);
for (i = 0; i < canthilos; i++)
  pthread_create(&threads[i], NULL, multiplica, (void *) (__intptr_t) i);
 
for (i = 0; i < canthilos; i++)
  pthread_join(threads[i], NULL);
  
printf("Mi minimoo total %d\n", minTotal);
printf("Mi maximo total %d\n", maxTotal);

 return(0);

}