#include<stdio.h>
#include<stdlib.h>
#include <sys/time.h>

int N;
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

    //Controla los argumentos al programa
    if ((argc != 2) || ((N = atoi(argv[1])) <= 0) )
    {
        printf("\nUsar: %s n\n  n: Dimension de la matriz (nxn X nxn)\n", argv[0]);
        exit(1);
    }

    int i,j,k;
    int random = 11;
    int totalA=0;
    int totalB=0;
    int totalC=0;
    int maxA = 0;
    int maxB = 0;
    int maxC = 0;
    int minA = random;
    int minB = random;
    int minC = random;
    //Aloca memoria para las matrices
    A=(double*)malloc(sizeof(double)*N*N);
    B=(double*)malloc(sizeof(double)*N*N);
    C=(double*)malloc(sizeof(double)*N*N);
    D=(double*)malloc(sizeof(double)*N*N);
    E=(double*)malloc(sizeof(double)*N*N);
    
    srand ( time(NULL) );
    //srand genera la semilla random y cargo las matrices
      for(i = 0 ; i < N ; i++){
        int disp = i * N;
        for (j = 0; j < N; j++){
            A[disp+j] = rand() % random;
            B[disp+j] = rand() % random;
            C[disp+j] = rand() % random;
        }
      }

     
    double timetick;
    timetick = dwalltime();
    // RECORRO A Y CALCULO AVG,MAX,MIN
    for(i = 0 ; i < N ; i++){
        int disp = i * N;
        for(j = 0; j < N; j++){
            int pos = disp+j;
            totalA+= A[pos];
            if(A[pos] > maxA) maxA = A[pos] ;
            if(A[pos] < minA) minA = A[pos] ;
        }
    }
  // RECORRO B Y CALCULO AVG,MAX,MIN
    for(i = 0 ; i < N ; i++){
        int disp = i * N;
        for(j = 0; j < N; j++){
            int pos = disp+j;
            totalB+= B[pos];
            if(B[pos] > maxB) maxB = B[pos] ;
            if(B[pos] < minB) minB = B[pos] ;
        }
    }
    // RECORRO C Y CALCULO AVG,MAX,MIN
    for(i = 0 ; i < N ; i++){
        int disp = i * N;
        for(j = 0; j < N; j++){
            int pos = disp+j;
            totalC+= C[pos];
            if(C[pos] > maxC) maxC = C[pos] ;
            if(C[pos] < minC) minC = C[pos] ;
        }
    }

    // EN ESTA ALTURA YA TENEMOS EL PROMEDIO Y LOS LIMITES, SOLO QUEDA MULTIPLICARLOS POR LAS MATRICES

    //Realiza la multiplicacion

    // E = A.B
    for(i=0;i<N;i++){
        int dispFila = i * N;
        for(j=0;j<N;j++){
            int dispColumna = j * N;
            int posResultado = dispFila + j;            
            for(k=0;k<N;k++){
                E[posResultado]= A[dispFila + k] * B[dispColumna + k];
            }
        }
    }   
    // D = E.C
    for(i=0;i<N;i++){
        int dispFila = i * N;
        for(j=0;j<N;j++){
            int dispColumna = j * N;
            int posResultado = dispFila + j;            
            for(k=0;k<N;k++){
                D[posResultado]= E[dispFila + k] * C[dispColumna + k];
            }
        }
    }
    // MULTIPLICO POR EL ESCALAR
    double escalar = (maxA * maxB * maxC - minA * minB * minC) / totalA/(N*N) * totalB/(N*N) *totalC/(N*N);
    for(i=0;i<N;i++){
        int disp = i * N;
        for(j=0;j<N;j++){
            D[disp+j]=D[disp+j] * escalar;

        }
    }

    printf("Tiempo en segundos %f\n", dwalltime() - timetick);

 return(0);

}