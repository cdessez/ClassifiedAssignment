#include <iostream>
#include <vector>
#include <math.h>
#include <omp.h>

#include "mpi.h"
#include "mpi_func.h"
#include "SeqLaplaceInv.h"

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
  const double expAo2 = exp(A/2.);

  double t;
  double H, U, X, Y;

  double Sum, sum_tmp;
  double localdata;

  int chunk_size;

  double f_k;
  double sum_cj[M+1];

  //compute sum_cj (partial sums of C(i,M))
  sum_cj[0] = 0.;
  for(int i = 1; i < M+1; i++){
    sum_cj[i] = sum_cj[i-1] + C(i,M);
  }

  //compute for each point
  for(int p = 0; p < input_size; p++){
    if(iammaster){
      t = input[p];
    }
    MPI_Bcast(&t, 1, MPI_DOUBLE, MASTER_RANK, MPI_COMM_WORLD);

    H = PI/t;
    U = expAo2/t;
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

void mpi2(vector<double> &input, vector<double> &output, int N, int M, int input_size,
 int my_rank, int pool_size, bool iammaster, int MASTER_RANK){

  SeqLaplaceInv seqL(&L, N, M);
  int chunk_size = input_size/pool_size;
  MPI_Bcast(&chunk_size, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
  vector <double> input_chunk(chunk_size), output_chunk(chunk_size);

  //we divide input in pool_size vectors of size chunk_size
  for(int i = 0; i < chunk_size; i++){
    MPI_Scatter(&input[i*pool_size], 1, MPI_DOUBLE,
	&input_chunk[i], 1, MPI_DOUBLE, MASTER_RANK, MPI_COMM_WORLD);
  }

  output_chunk = seqL(input_chunk);

  //we gather the output_chunk vectors into output
  for(int i = 0; i < chunk_size; i++){
    MPI_Gather(&output_chunk[i], 1, MPI_DOUBLE,
	&output[i*pool_size + my_rank], 1, MPI_DOUBLE, MASTER_RANK, MPI_COMM_WORLD);
  }

  //compute the remaining values
  if(iammaster){
    for(int i = input_size - (input_size%chunk_size); i < input_size; i++){
      output[i] = seqL(input[i]);
    }
  }

}


void mpi2_openmp(vector<double> &input, vector<double> &output, int N, int M, int input_size,
 int my_rank, int pool_size, bool iammaster, int MASTER_RANK){


  //8 cores : 2 threads per core
  omp_set_num_threads(16);

  SeqLaplaceInv seqL(&L, N, M);
  int chunk_size = input_size/pool_size;
  MPI_Bcast(&chunk_size, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
  vector <double> input_chunk(chunk_size), output_chunk(chunk_size);

  //we divide input in pool_size vectors of size chunk_size
  for(int i = 0; i < chunk_size; i++){
    MPI_Scatter(&input[i*pool_size], 1, MPI_DOUBLE,
	&input_chunk[i], 1, MPI_DOUBLE, MASTER_RANK, MPI_COMM_WORLD);
  }

  #pragma omp parallel for
  for(int i = 0; i < chunk_size; i++){
    output_chunk[i] = seqL(input_chunk[i]);
  }


  //we gather the output_chunk vectors into output
  for(int i = 0; i < chunk_size; i++){
    MPI_Gather(&output_chunk[i], 1, MPI_DOUBLE,
	&output[i*pool_size + my_rank], 1, MPI_DOUBLE, MASTER_RANK, MPI_COMM_WORLD);
  }

  //we compute the remaining values
  if(iammaster){
    #pragma omp parallel for
    for(int i = input_size - (input_size%chunk_size); i < input_size; i++){
      output[i] = seqL(input[i]);
    }
  }

}


