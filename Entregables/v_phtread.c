#include <pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

int canthilos=4;
int N=32;
double *A,*B,*C,*D;
pthread_mutex_t task_queue_lock;

// HILO QUE CALCULA EL PROMEDIO Y MAXIMOS,MINIMOS
void *promedioYLimites(void *id) {
//
//HACER CON UN ARREGLO PARA QUE CADA HILO GUARDE SU PARTE EN LA POSICION DE SU ID, PARA EL MAXIMO MINIMO Y TOTALES

  int i,j;
  float totalLocalA = 0;
  float totalLocalB = 0;
  float totalLocalC = 0;
  int maxLocalA = 0;
  int minLocalA = 1000;
  int maxLocalB = 0;
  int minLocalB = 1000;
  int maxLocalC = 0;
  int minLocalC = 1000;

  int threadId = (int) (__intptr_t) id;
  //recorro y sumo mi parte del arreglo
  int inicio = threadId * (N/canthilos);
  int fin = inicio + (N/canthilos);

  for(i = inicio ; i < fin ; i++){
    for(j = inicio; j < fin; j++){
   		totalLocalA+= A[i*N+j];
        if(A[i*N+j] > maxLocalA) maxLocalA = A[i*N+j] ;
        if(A[i*N+j] < minLocalA) minLocalA = A[i*N+j] ;
        totalLocalB+= B[i*N+j];
        if(B[i*N+j] > maxLocalB) maxLocalB = B[i*N+j] ;
        if(B[i*N+j] < minLocalB) minLocalB = B[i*N+j] ;
        totalLocalC+= C[i*N+j];
        if(C[i*N+j] > maxLocalC) maxLocalC = C[i*N+j] ;
        if(C[i*N+j] < minLocalC) minLocalC = C[i*N+j] ;
    }
    }

  printf("Mi promedio local %f\n", totalLocalA/ (N/canthilos));
  printf("Mi minimoo local del hilo A,B,C son %d %d\n",threadId, minLocalA);
  printf("Mi maximo local del hilo %d %d\n",threadId, maxLocalA);
  //almaceno el promedio de lo que junte en el centralizado
  pthread_mutex_lock(&task_queue_lock);
  	total += totalLocalA/ (N/canthilos;
    if(maxLocal > maxTotal){  maxTotal = maxLocal ;  printf("Hilo %d  actualizo el maxmino con %d\n",threadId, maxLocal);}
    if( minLocal < minTotal ) { minTotal = minLocal ; printf("Hilo %d  actualizo el minimo con %d\n",threadId, minLocal);}
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
 
    int i,j;
    float totalA=0;
    float totalB=0;
    float totalC=0;
    int maxA = 0;
    int maxB = 0;
    int maxC = 0;
    int minA = 100;
    int minB = 100;
    int minC = 100;
    //Aloca memoria para las matrices
    A=(double*)malloc(sizeof(double)*N*N);
    B=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
    D=(double*)malloc(sizeof(double)*N*N);
    
    srand ( time(NULL) );
    //srand genera la semilla random y cargo las matrices
      for(i = 0 ; i < N ; i++){
        for (j = 0; j < N; j++){
            A[i*N+j] = rand() % 100;
            B[i*N+j] = rand() % 100;
            C[i*N+j] = rand() % 100;
        }
      }

 double timetick;

timetick = dwalltime();
pthread_t threads[canthilos];
pthread_attr_t attr;
int ids[canthilos];

pthread_mutex_init(&task_queue_lock, NULL);

for (i = 0; i < canthilos; i++)
  pthread_create(&threads[i], NULL, promedioYLimites, (void *) (__intptr_t) i);  
 
for (i = 0; i < canthilos; i++)
  pthread_join(threads[i], NULL);


printf("Tiempo en segundos %f\n", dwalltime() - timetick);
printf("Promedio A: %f\n", totalA/(N*N) );
printf("Promedio B: %f\n", totalB/(N*N) );
printf("Promedio C: %f\n", totalC/(N*N) );
printf("Mi minimoo total A %d\n", minA);
printf("Mi maximo total A %d\n", maxA);
printf("Mi minimoo total B %d\n", minB);
printf("Mi maximo total b %d\n", maxB);
printf("Mi minimoo total C %d\n", minC);
printf("Mi maximo total C %d\n", maxC);


// EN ESTA ALTURA YA TENEMOS EL PROMEDIO Y LOS LIMITES, SOLO QUEDA MULTIPLICARLOS POR LAS MATRICES

 return(0);

}