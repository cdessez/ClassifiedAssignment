#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <complex>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include <unistd.h>

#include "mpi.h"
#include "mystery.h"

using namespace std;

static const int MASTER_RANK = 0;
static const string ifile = "in.txt";
static const string ofile = "out.txt";

void notifyWrapperViaSocket(int port){
  int fd, numbytes;   /* files descriptors */     
  struct hostent *he;         /* structure that will get information about remote host */ 
  struct sockaddr_in server;  /* server's address information */  

  if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){ 
    cerr << "socket() error" << endl; 
    exit(EXIT_FAILURE);  
  }

  if ((he=gethostbyname("127.0.0.1"))==NULL){
    cerr << "gethostbyname() error" << endl; 
    exit(EXIT_FAILURE); 
  } 

  server.sin_family = AF_INET; 
  server.sin_port = htons(port); 
  server.sin_addr = *((struct in_addr *)he->h_addr);  /*he->h_addr passes "*he"'s info to "h_addr" */ 
  //bzero(&(server.sin_zero),8); 

  if(connect(fd, (struct sockaddr *) &server, sizeof(struct sockaddr)) == -1){ 
    cerr << "connect() error" << endl; 
    exit(EXIT_FAILURE); 
  } 
  close(fd);
}

int C(int k, int m){
  long res = 1; 
  const long mk = m - k;
  for (int i = m; i > k; i--)
    res *= i;
  for (int i = 1; i <= mk; i++)
    res /= i;
  return static_cast<int>(res);
}

int main(int argc, char **argv){
  
  // MPI-related vars
  int pool_size, my_rank;
  bool iammaster;
  double tbeg, tend;

  int N, M, input_size; // arguments for the laplace inversion algorithm
  int port; // used to send the signal to wake up the wrapper

  if (argc != 5){
    cerr << "Wrong call to mpicore: it takes 4 arguments (N, M, size of" 
          << "the input and wake up port)" << endl;
    exit(EXIT_FAILURE);
  }
  N = atoi(argv[1]);
  M = atoi(argv[2]);
  port = atoi(argv[4]);
  input_size = atoi(argv[3]);
  if (N == 0 or M == 0){
    cerr << "Error while parsing the arguments, or bad arguments" << endl;
    exit(EXIT_FAILURE);
  }

  vector<double> input(0), output(0); // used only by master

  MPI_Init(&argc, &argv);
  tbeg = MPI_Wtime();
  MPI_Comm_size(MPI_COMM_WORLD, &pool_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  iammaster = my_rank == MASTER_RANK;

  if (iammaster){
    input.resize(input_size);
    output.resize(input_size);
    // code filling the vector input with the input values
    ifstream finput(ifile.c_str(), ifstream::in);
    if (!finput){
      cerr << "Cannot open input file" << ifile << endl;
      exit(EXIT_FAILURE);
    }
    string line;
    for (int i = 0; i < input_size; i++){
      getline(finput,line);
      input[i] = atof(line.c_str());
    }
    finput.close();
  }
  
  // MPI processing code
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
    //only the master knows input and output
    //we read t and then broadcast it to the other processes
    if(iammaster){
      t = input[p];
    }
    MPI_Bcast(&t, 1, MPI_DOUBLE, MASTER_RANK, MPI_COMM_WORLD);
    
    //syncing the processes, might be useless
    MPI_Barrier(MPI_COMM_WORLD);

    //init local variables
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
      MPI_Reduce(&localdata, &sum_tmp, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
      if(iammaster){
        Sum += sum_tmp;
      }
    }
    // only executed by the master
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
      // sum_tmp = sum(localdata)
      MPI_Reduce(&localdata, &sum_tmp, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
      
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

    

  //if (iammaster){
  //  for(int i = 0; i < input_size; i++){
  //    output[i] = input[i];
  //  }
  //  sleep(3);
  //} // this block must be replaced by the real code ;)
  // ...
  
  if (iammaster){
    // MPI: the master should gather the output values in the right order in
    // the vector 'output'
    
    // code sending the output values back to the main process
    ofstream foutput;
    foutput.open(ofile.c_str());
    if (!foutput.is_open()){
      cerr << "Cannot open output file " << ofile << endl;
    }
    for(int i = 0; i < input_size; i++){
      foutput << output[i] << endl;
    }
    foutput.close();
    
    tend = MPI_Wtime();
    cout << "Execution time: " << tend - tbeg << " s" << endl;
    notifyWrapperViaSocket(port);
  }
  
  MPI_Finalize();

  return 0;
}

