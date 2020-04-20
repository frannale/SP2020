#include <pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

int canthilos=4;
int N=20;
int arreglo[20];
float total=0;
pthread_mutex_t task_queue_lock;


void *multiplica(void *id) {
    
  int i;
  float promedioLocal,cant = 0;
  int threadId = (int) id;
  //recorro y sumo mi parte del arreglo
  for(i = (threadId*(N/canthilos)); i < ((threadId+1)*(N/canthilos)); i++){
   		cant+= arreglo[i];
  }
  promedioLocal = cant / (N/canthilos);
  printf("Mi promedio local %f\n", promedioLocal);
  //almaceno el promedio de lo que junte en el centralizado
  pthread_mutex_lock(&task_queue_lock);
  	total += promedioLocal;
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
 srand ( time(NULL) );
 //srand genera la semilla random
 for (i = 0; i < N; i++){
 	arreglo[i] = rand() % 10;
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

//en total esta la suma de los promedios calculados por los hilos, lo dividimos acorde a la cantidad de hilos que ejecutaron  
total = total / canthilos ;
printf("Tiempo en segundos %f\n", dwalltime() - timetick);
printf("Promedio: %f\n", total );

 return(0);

}