#include <iostream>
#include <vector>
#include <math.h>

#include "mpi.h"
#include "mpi_func.h"

using namespace std;

inline int C(int k, int m){
  long res = 1; 
  const long mk = m - k;
  for (int i = m; i > k; i--)
    res *= i;
  for (int i = 1; i <= mk; i++)
    res /= i;
  return static_cast<int>(res);
}


// MPI processing code
void mpi1(vector<double> &input, vector<double> &output, int N, int M, int input_size,
 int my_rank, int pool_size, bool iammaster, int MASTER_RANK){
  
  const double A = 18.4;
  const double PI = atan(1)*4;

  double t;
  double H, U, X, Y;

  double Sum, sum_tmp;
  double localdata;

  int chunk_size;

  double f_k;
  double sum_cj[M+1];

  //compute sum_cj
  sum_cj[0] = 0.;
  for(int i = 1; i < M+1; i++){
    sum_cj[i] = sum_cj[i-1] + C(i,M);
  }

  for(int p = 0; p < input_size; p++){
    if(iammaster){
      t = input[p];
    }
    MPI_Bcast(&t, 1, MPI_DOUBLE, MASTER_RANK, MPI_COMM_WORLD);
    //syncing the processes, might be useless
    MPI_Barrier(MPI_COMM_WORLD);

    H = PI/t;
    U = exp(A/2.)/t;
    X = A/(2.*t);
    Y = 0.;

    Sum = L(X,0.).real()/2.;

    chunk_size = N/pool_size;
    // i = 1..(N-N%chunk_size)
    for(int i = (my_rank*chunk_size)+1; i <= (my_rank+1)*chunk_size; i++){
      Y = i*H;
      localdata = ((i & 0x1) == 0x0 ? L(X,Y).real() : -L(X,Y).real());
      // sum_tmp = sum(localdata)
      MPI_Reduce(&localdata, &sum_tmp, 1, MPI_DOUBLE, MPI_SUM, MASTER_RANK, MPI_COMM_WORLD);
      if(iammaster){
        Sum += sum_tmp;
      }
    }
    // i=(N-N&chunk_size)..N
    if(iammaster){
      for(int i = N-(N%pool_size)+1; i <= N; i++){
        Y = i*H;
        Sum += ((i & 0x1) == 0x0 ? L(X,Y).real() : -L(X,Y).real());
      }
    }

    int i;
    chunk_size = M/pool_size;
    // k = 0..(M-M%chunk_size)
    for(int k = (my_rank*chunk_size); k < (my_rank+1)*chunk_size; k++) {
      i = N+k;
      Y = i*H;
      f_k = (i & 0x1) == 0x0 ? L(X,Y).real() : -L(X,Y).real();
      localdata = (1 - (sum_cj[k]/(sum_cj[M]+1)))*f_k;
      MPI_Reduce(&localdata, &sum_tmp, 1, MPI_DOUBLE, MPI_SUM, MASTER_RANK, MPI_COMM_WORLD);
      
      if(iammaster){
        Sum += sum_tmp;
      }
    }
    // k = (M-M%chunk_size)..M
    if(iammaster){
      for(int k = M-(M%pool_size)+1; k < M; k++){
        i = N+k;
        Y = i*H;
        f_k = (i & 0x1) == 0x0 ? L(X,Y).real() : -L(X,Y).real();
        Sum += (1 - (sum_cj[k]/(sum_cj[M]+1)))*f_k;
      }
    }

    if(iammaster){
      output[p] = U*Sum;
    }
  }
}