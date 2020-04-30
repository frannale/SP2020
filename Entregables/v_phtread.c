#include <pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

//cantidad de hilos
int canthilos = 4;
int manager = 4;
// tamanio de la matriz
int N=4096;
// maximo numero generado en cada posicion, hay limite? 0 y 1?
// la ultima posicion es la que usara el manager 
int nRandom = 50;
int totalA[5];
int totalB[5];
int totalC[5];
int maxA[5];
int maxB[5];
int maxC[5];
int minA[5];
int minB[5];
int minC[5];
double *A,*B,*C,*D,*E;
float escalar;

// HILO QUE CALCULA EL PROMEDIO Y MAXIMOS,MINIMOS
void *promedioYLimites(void *id) {
    // habria que multiplicar aca?? o despues de procesar todo
    int i,j,k;
    int threadId = (int) (__intptr_t) id;
    int inicio = threadId * (N/canthilos);
    int fin = inicio + (N/canthilos);
    //RECORRO A
   
    for( i = inicio ; i < fin ; i++){
      for( j = 0; j < N; j++){
        totalA[threadId] = totalA[threadId] + A[i*N+j];        
        if(A[i*N+j] > maxA[threadId]) maxA[threadId] = A[i*N+j] ;
        if(A[i*N+j] < minA[threadId]) minA[threadId] = A[i*N+j] ;
      }
    }
    //RECORRO B
    for( i = inicio ; i < fin ; i++){
      for( j = 0; j < N; j++){
        totalB[threadId] += B[i*N+j];
        if(B[i*N+j] > maxB[threadId]) maxB[threadId] = B[i*N+j] ;
        if(B[i*N+j] < minB[threadId]) minB[threadId] = B[i*N+j] ;
      }
    }
    //RECORRO C
    for( i = inicio ; i < fin ; i++){
      for( j = 0; j < N; j++){
        totalC[threadId] += C[i*N+j];
        if(C[i*N+j] > maxC[threadId]) maxC[threadId] = C[i*N+j] ;
        if(C[i*N+j] < minC[threadId]) minC[threadId] = C[i*N+j] ;
      }
    }
    // E = A.B
    for( i=inicio;i<fin;i++){
        for(j=0;j<N;j++){
            for(k=0;k<N;k++){
                E[i*N+j]= A[i*N+k] * B[k+j*N];
            }
        }
    }   
    // D = E.C
    for(i=inicio;i<fin;i++){
        for(j=0;j<N;j++){
            for(k=0;k<N;k++){
                D[i*N+j]= E[i*N+k] * C[k+j*N];
            }
        }
    }


}

// HILO QUE MULTIPLICA POR EL ESCALAR
void *multiplicarEscalar(void *id) {
    // MULTIPLICO POR EL ESCALAR
    int i,j;
    int threadId = (int) (__intptr_t) id;
    int inicio = threadId * (N/canthilos);
    int fin = inicio + (N/canthilos);
    for(i=inicio;i<fin;i++){
        for(j=0;j<N;j++){
            D[i*N+j]=D[i*N+j] * escalar;

        }
    }

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
    //Aloca memoria para las matrices
    A=(double*)malloc(sizeof(double)*N*N);
    B=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
    D=(double*)malloc(sizeof(double)*N*N);
    E=(double*)malloc(sizeof(double)*N*N);

    pthread_t threads[canthilos];
    pthread_attr_t attr;
    int ids[canthilos];

    // SETEO DE LOS VALORES EN LOS DISTINTOS ARREGLOS, CADA HILO TIENE UN ELEMENTO UNICO Y EXCLUSIVO EN CADA UNO DE ELLOS
    for(int i = 0; i <= manager; i++){
   		totalA[i] = 0;
      totalB[i] = 0;
      totalC[i] = 0;
      maxA[i] = 0;
      maxB[i] = 0; 
      maxC[i] = 0;
      maxC[i] = 0;
      minA[i] = nRandom;
      minB[i] = nRandom;
      minC[i] = nRandom;

    } 
    
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
    


    for (i = 0; i < canthilos; i++)
      pthread_create(&threads[i], NULL, promedioYLimites, (void *) (__intptr_t) i);  
    
    for (i = 0; i < canthilos; i++)
      pthread_join(threads[i], NULL);

    // TODOS LOS HILOS COMPLETARON EL PROCESAMIENTO, AHORA HABRIA QUE CALCULAR EL ESCALAR
    // RECORRO LO GENERADO POR LO HILOS Y ME QUEDO CON LO MEJOR, aca se puede ser mas eficiente?? recorrer en muchos for? al igual que en los hilos?
    for(int i = 0; i < canthilos; i++){
      totalA[manager] += totalA[i];
      totalB[manager] += totalB[i];
      totalC[manager] += totalC[i];
      if( minA[manager] > minA[i] ) minA[manager] = minA[i];
      if( minB[manager] > minB[i] ) minB[manager] = minB[i];
      if( minC[manager] > minC[i] ) minC[manager] = minC[i];
      if( maxA[manager] < maxA[i] ) maxA[manager] = maxA[i];
      if( maxB[manager] < maxB[i] ) maxB[manager] = maxB[i];
      if( maxC[manager] < maxC[i] ) maxC[manager] = maxC[i];

    } 

    // printf("Promedio A: %d\n", totalA[manager]/(N*N) );
    // printf("Promedio B: %d\n", totalB[manager]/(N*N) );
    // printf("Promedio C: %d\n", totalC[manager]/(N*N) );
    // printf("Mi minimoo total A %d\n", minA[manager]);
    // printf("Mi maximo total A %d\n", maxA[manager]);
    // printf("Mi minimoo total B %d\n", minB[manager]);
    // printf("Mi maximo total b %d\n", maxB[manager]);
    // printf("Mi minimoo total C %d\n", minC[manager]);
    // printf("Mi maximo total C %d\n", maxC[manager]);

    escalar = (maxA[manager] * maxB[manager] * maxC[manager] - minA[manager] * minB[manager] * minC[manager]) / totalA[manager]/(N*N) * totalB[manager]/(N*N) *totalC[manager]/(N*N);
    
    // EN ESTA ALTURA YA TENEMOS EL ESCALAR, LOS HILOS SE ENCARGAN DE MULTIPLICAR POR EL MISMO
    for (i = 0; i < canthilos; i++)
      pthread_create(&threads[i], NULL, multiplicarEscalar, (void *) (__intptr_t) i);  
    
    for (i = 0; i < canthilos; i++)
      pthread_join(threads[i], NULL);

    //TERMINO

    printf("Tiempo en segundos %f\n", dwalltime() - timetick);
  

 return(0);

}