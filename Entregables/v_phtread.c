#define _XOPEN_SOURCE 600 /* Or higher */
#include <pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

//cantidad de hilos
int canthilos;
// tamanio de la matriz
int N;
// maximo numero generado en cada posicion
int nRandom = 50;
// matrices
double *A,*B,*AB,*C,*D;
// cada arreglo en su ultima posicion tendra una posicion exlusiva para calcular su contenido(avg,max,min)
double *maxA,*maxB,*maxC,*minA,*minB,*minC,*totalA,*totalB,*totalC;
double escalar;
// creacion de barreras
int pthread_barrier_init(pthread_barrier_t *restrict firstBarrier,
const pthread_barrierattr_t *restrict attr,
unsigned cantHilos);
int pthread_barrier_init(pthread_barrier_t *restrict secondBarrier,
const pthread_barrierattr_t *restrict attr,
unsigned cantHilos);


// HILO QUE CALCULA EL PROMEDIO Y MAXIMOS,MINIMOS
void *computarExpresionEscalar(void * ptr) {
    int i,j,k;
    int * p,threadId;
    int localMax = 0;
    int localMin = nRandom;
    int localTot = 0;
    p = (int *) ptr;
    threadId = * p;
    int inicio = threadId * (N/canthilos);
    int fin = inicio + (N/canthilos);
    //RECORRO A   
    for( i = inicio ; i < fin ; i++){
      int disp = i * N;
      for( j = 0; j < N; j++){
        int pos = disp + j;
        localTot += A[pos];        
        if(A[pos] > localMax) localMax = A[pos] ;
        if(A[pos] < localMin) localMin = A[pos] ;
      }
    }
    // GUARDO EN EL ARREGLO GLOBAL MIS VALORES, Y VUELVO A INCIALIZAR VARIABLES PARA B
    totalA[threadId] = localTot;
    maxA[threadId] = localMax;
    minA[threadId] = localMin;
    localMax = 0;
    localMin = nRandom;
    localTot = 0;
    //RECORRO B
    for( i = inicio ; i < fin ; i++){
      int disp = i * N;
      for( j = 0; j < N; j++){
        int pos = disp + j;
        localTot += B[pos];
        if(B[pos] > localMax) localMax = B[pos] ;
        if(B[pos] < localMin) localMin = B[pos] ;
      }
    }
    // GUARDO EN EL ARREGLO GLOBAL MIS VALORES, Y VUELVO A INCIALIZAR VARIABLES PARA C
    totalB[threadId] = localTot;
    maxB[threadId] = localMax;
    minB[threadId] = localMin;
    localMax = 0;
    localMin = nRandom;
    localTot = 0;
    //RECORRO C
    for( i = inicio ; i < fin ; i++){
      int disp = i * N;
      for( j = 0; j < N; j++){
        int pos = disp + j;
        localTot += C[pos];
        if(C[pos] > localMax) localMax = C[pos] ;
        if(C[pos] < localMin) localMin = C[pos] ;
      }
    }
    totalC[threadId] = localTot;
    maxC[threadId] = localMax;
    minC[threadId] = localMin;
    // A.B
    for( i=inicio;i<fin;i++){
        int dispFila = i * N;
        for(j=0;j<N;j++){
            int dispColumna = j * N;
            for(k=0;k<N;k++){
                AB[dispFila + j]= A[dispFila + k] * B[dispColumna + k];
            }
        }
    }   
    // D = AB.C
    for(i=inicio;i<fin;i++){
        int dispFila = i * N;
        for(j=0;j<N;j++){
          int dispColumna = j * N;
            for(k=0;k<N;k++){
                D[dispFila+j]= AB[dispFila+k] * C[dispColumna + k];
            }
        }
    }

    int pthread_barrier_wait(pthread_barrier_t *firstBarrier);

    // LUEGO QUE LA BARRERA ASEGURO QUE TODOS TERMINARON,EL HILO 0, RECORRE LO GENERADO POR LO HILOS Y CALCULA LOS AVG,MIN,MAX
    if( threadId == 0){
      int pthread_barrier_destroy(pthread_barrier_t * firstBarrier);
      for(i = 0; i < canthilos; i++){
        totalA[canthilos] += totalA[i];
        totalB[canthilos] += totalB[i];
        totalC[canthilos] += totalC[i];
        if( minA[canthilos] > minA[i] ) minA[canthilos] = minA[i];
        if( minB[canthilos] > minB[i] ) minB[canthilos] = minB[i];
        if( minC[canthilos] > minC[i] ) minC[canthilos] = minC[i];
        if( maxA[canthilos] < maxA[i] ) maxA[canthilos] = maxA[i];
        if( maxB[canthilos] < maxB[i] ) maxB[canthilos] = maxB[i];
        if( maxC[canthilos] < maxC[i] ) maxC[canthilos] = maxC[i];
        }
        
        escalar = (maxA[canthilos] * maxB[canthilos] * maxC[canthilos] - minA[canthilos] * minB[canthilos] * minC[canthilos]) / totalA[canthilos]/(N*N) * totalB[canthilos]/(N*N) *totalC[canthilos]/(N*N);
    }
    //BARRERA PARA ESPERAR AL QUE SE SACRIFICA POR EL TEAM Y CALCULA EL ESCALAR PARA QUE LOS DEMAS HILOS LO USEN

    int pthread_barrier_wait(pthread_barrier_t *secondBarrier);

    // MULTIPLICO POR EL ESCALAR
    for(i=inicio;i<fin;i++){
        int dispFila = i * N;
        for(j=0;j<N;j++){
            D[dispFila+j]=D[dispFila+j] * escalar;

        }
    }
    // FINALIZA LA EJECUCION DEL HILO
    pthread_exit(0);

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
    N = atoi(argv[1]);
    canthilos = atoi(argv[2]);
    //Aloca memoria para las matrices
    A=(double*)malloc(sizeof(double)*N*N);
    B=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
    D=(double*)malloc(sizeof(double)*N*N);
    AB=(double*)malloc(sizeof(double)*N*N);
    // Aloco los arreglos
    totalA = (double*)malloc(sizeof(double)*canthilos +1 );
    totalB = (double*)malloc(sizeof(double)*canthilos +1 );
    totalC = (double*)malloc(sizeof(double)*canthilos +1 );
    maxA = (double*)malloc(sizeof(double)*canthilos +1 );
    maxB = (double*)malloc(sizeof(double)*canthilos +1 );
    maxC = (double*)malloc(sizeof(double)*canthilos +1 );
    minA = (double*)malloc(sizeof(double)*canthilos +1 );
    minB = (double*)malloc(sizeof(double)*canthilos +1 );
    minC = (double*)malloc(sizeof(double)*canthilos +1 );

    srand ( time(NULL) );
    //srand genera la semilla random y cargo las matrices
      for( i = 0 ; i < N ; i++){
        for ( j = 0; j < N; j++){
            A[i*N+j] = rand() % nRandom;
            B[i*N+j] = rand() % nRandom;
            C[i*N+j] = rand() % nRandom;
        }
      }

    double timetick;
    timetick = dwalltime();
    pthread_t threads[canthilos];
    pthread_attr_t attr;
    int ids[canthilos];
    // CREACION DE HILOS
    for (i = 0; i < canthilos; i++){
      ids[i] = i;
      pthread_create(&threads[i], NULL, computarExpresionEscalar, &ids[i]);  
    }
       
    for (i = 0; i < canthilos; i++)
      pthread_join(threads[i], NULL);

    int pthread_barrier_destroy(pthread_barrier_t * secondBarrier);

    //TERMINO
    printf("Tiempo en segundos %f\n", dwalltime() - timetick);
  
 return(0);
}