#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

int N=2048;
double *A,*B,*C,*D,*E;

//Para calcular tiempo
double dwalltime(){
        double sec;
        struct timeval tv;

        gettimeofday(&tv,NULL);
        sec = tv.tv_sec + tv.tv_usec/1000000.0;
        return sec;
}

int main(int argc,char*argv[]){
    int i,j,k;
    int totalA=0;
    int totalB=0;
    int totalC=0;
    int maxA = 0;
    int maxB = 0;
    int maxC = 0;
    int minA = 1000;
    int minB = 1000;
    int minC = 1000;
    //Aloca memoria para las matrices
    A=(double*)malloc(sizeof(double)*N*N);
    B=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
    D=(double*)malloc(sizeof(double)*N*N);
    E=(double*)malloc(sizeof(double)*N*N);
    
    srand ( time(NULL) );
    //srand genera la semilla random y cargo las matrices
      for(i = 0 ; i < N ; i++){
        for (j = 0; j < N; j++){
            A[i*N+j] = rand() % 1000;
            B[i*N+j] = rand() % 1000;
            C[i*N+j] = rand() % 1000;
        }
      }

     
    double timetick;
    timetick = dwalltime();
    // RECORRO A Y CALCULO AVG,MAX,MIN
    for(i = 0 ; i < N ; i++){
        for(j = 0; j < N; j++){
            totalA+= A[i*N+j];
            if(A[i*N+j] > maxA) maxA = A[i*N+j] ;
            if(A[i*N+j] < minA) minA = A[i*N+j] ;
        }
    }
  // RECORRO B Y CALCULO AVG,MAX,MIN
    for(i = 0 ; i < N ; i++){
        for(j = 0; j < N; j++){
            totalB+= B[i*N+j];
            if(B[i*N+j] > maxB) maxB = B[i*N+j] ;
            if(B[i*N+j] < minB) minB = B[i*N+j] ;
        }
    }
    // RECORRO C Y CALCULO AVG,MAX,MIN
    for(i = 0 ; i < N ; i++){
        for(j = 0; j < N; j++){
            totalC+= C[i*N+j];
            if(C[i*N+j] > maxC) maxC = C[i*N+j] ;
            if(C[i*N+j] < minC) minC = C[i*N+j] ;
        }
    }
    // printf("Promedio A: %f\n", totalA/(N*N) );
    // printf("Promedio B: %f\n", totalB/(N*N) );
    // printf("Promedio C: %f\n", totalC/(N*N) );
    // printf("Mi minimoo total A %d\n", minA);
    // printf("Mi maximo total A %d\n", maxA);
    // printf("Mi minimoo total B %d\n", minB);
    // printf("Mi maximo total b %d\n", maxB);
    // printf("Mi minimoo total C %d\n", minC);
    // printf("Mi maximo total C %d\n", maxC);


    // EN ESTA ALTURA YA TENEMOS EL PROMEDIO Y LOS LIMITES, SOLO QUEDA MULTIPLICARLOS POR LAS MATRICES

    //Realiza la multiplicacion

    // E = A.B
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            for(k=0;k<N;k++){
                E[i*N+j]= A[i*N+k] * B[k+j*N];
            }
        }
    }   
    // D = E.C
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            for(k=0;k<N;k++){
                D[i*N+j]= E[i*N+k] * C[k+j*N];
            }
        }
    }
    // MULTIPLICO POR EL ESCALAR
    float escalar = (maxA * maxB * maxC - minA * minB * minC) / totalA/(N*N) * totalB/(N*N) *totalC/(N*N);
    for(i=0;i<N;i++){
        for(j=0;j<N;j++){
            D[i*N+j]=D[i*N+j] * escalar;

        }
    }

    printf("Tiempo en segundos %f\n", dwalltime() - timetick);

 return(0);

}