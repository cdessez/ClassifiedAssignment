#include <iostream>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <string>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>
#include <unistd.h>

#include "mpi.h"
#include "mystery.h"
#include "mpi_func.h"

using namespace std;

static const int MASTER_RANK = 0;
static const string ifile = "in.txt";
static const string ofile = "out.txt";

enum ParallelType {
  MPI1 = 0,
  MPI2 = 1,
  MPI1_OPENMP = 2,
  MPI2_OPENMP = 3
};

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

int main(int argc, char **argv){
  
  // MPI-related vars
  int pool_size, my_rank;
  bool iammaster;
  double tbeg, tend;

  int N, M, input_size; // arguments for the laplace inversion algorithm
  int ptype; // determine the type of computation
  int port; // used to send the signal to wake up the wrapper

  if (argc != 6){
    cerr << "Wrong call to mpicore: it takes 4 arguments (N, M, size of" 
          << "the input and wake up port)" << endl;
    exit(EXIT_FAILURE);
  }
  N = atoi(argv[1]);
  M = atoi(argv[2]);
  port = atoi(argv[5]);
  ptype = atoi(argv[4]);
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
  switch(ptype){
    case MPI1 :
      mpi1(input, output, N, M, input_size, my_rank, pool_size, iammaster, MASTER_RANK);
      break;
    case MPI2 :
      break;
    case MPI1_OPENMP :
      break;
    case MPI2_OPENMP :
      break;
    default :
      if(iammaster)
	cerr << "Something bad happened (unknown process type)" << endl;
      exit(EXIT_FAILURE);
      break;
  }
  
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

