#ifndef MPI_FUNC_H
#define MPI_FUNC_H

#include <vector>
#include <complex>

#include "mystery.h"

using namespace std;

void mpi1(vector<double> &input, vector<double> &output, int N, int M, int input_size,
 int my_rank, int pool_size, bool iammaster, int MASTER_RANK);

#endif
