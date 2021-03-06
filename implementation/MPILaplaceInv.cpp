#include "MPILaplaceInv.h"

// parameters default values
static const int defaultN = 100;
static const int defaultM = 15;

// names of input and output files to send/retrieve data to/from mpicore
static const string ifile = "in.txt";
static const string ofile = "out.txt";

// mpi setup parameters
static const string hostsfile = "hosts";
static const int mpi_pool_size = 16;

// offset port number for the wake up mechanism
static int port = 10007;


MPILaplaceInv::MPILaplaceInv(int ptype): LaplaceInv(), ptype(ptype), 
      N(defaultN), M(defaultM){
  srand(time(NULL));
  port += rand() % 1000;
}

MPILaplaceInv::MPILaplaceInv(int ptype, int N, int M): 
      LaplaceInv(), ptype(ptype), N(N), M(M){
  srand(time(NULL));
  port += rand() % 1000;
}

double MPILaplaceInv::operator()(double t){
  // wrapper for the following method
  vector<double> v(1);
  v[0] = t;
  return (*this)(v)[0];
}

vector<double> MPILaplaceInv::operator()(vector<double> &v){
  // this method delegates the computation to another process (mpicore)
  
  // write the input values in a file
  ofstream input;
  input.open(ifile.c_str(), ofstream::out);
  if (!input.is_open()){
    cerr << "Cannot open " << ifile << endl;
    exit(EXIT_FAILURE);
  }
  for(vector<double>::iterator it = v.begin(); it != v.end(); ++it){
    input << *it << endl;
  }
  input.close();
  
  // fork to launch mpicore in the child process
  pid_t pid = -1;
  pid = fork();
  if (pid < 0){
    cerr << "Fork failed!" << endl;
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    // child (launches mpicore)
    char arg_psize[10];
    char arg_N[10];
    char arg_M[10];
    char arg_size[10];
    char arg_ptype[10];
    char arg_port[10];
    sprintf(arg_psize, "%d", mpi_pool_size);
    sprintf(arg_N, "%d", N);
    sprintf(arg_M, "%d", M);
    sprintf(arg_size, "%d", v.size());
    sprintf(arg_ptype, "%d", ptype);
    sprintf(arg_port, "%d", port);
    execl("/usr/bin/mpirun", "/usr/bin/mpirun", "-machinefile",
          hostsfile.c_str(), "-np", arg_psize, "./mpicore", arg_N, arg_M,
          arg_size, arg_ptype, arg_port, NULL);
    exit(EXIT_FAILURE);
  }

  // parent: wait to be waken up by the child
  waitForConnectionSocket(port);
  
  // retrieve the results from a file
  vector<double> res;
  ifstream output(ofile.c_str(), ifstream::in);
  if (!output){
    cerr << "Cannot open output file " << ofile << endl;
    exit(EXIT_FAILURE);
  }
  string line;
  while(getline(output,line)){
    res.push_back(atof(line.c_str()));
  }
  output.close();
  
  return res;
}

void MPILaplaceInv::waitForConnectionSocket(int port){
  // Open a TCP server socket on 127.0.0.1:[port]
  // It will wait until the child process connects

  int fd, fd2; // file descriptors  
  struct sockaddr_in server; // server's address information 
  struct sockaddr_in client; // client's address information
  socklen_t sin_size; 

  if ((fd=socket(AF_INET, SOCK_STREAM, 0)) == -1 ){ 
    cerr << "socket() error" << endl; 
    exit(EXIT_FAILURE); 
  } 

  server.sin_family = AF_INET;         
  server.sin_port = htons(port);
  server.sin_addr.s_addr = inet_addr("127.0.0.1");  
  bzero(&(server.sin_zero),8); 

  if(bind(fd,(struct sockaddr*)&server,sizeof(struct sockaddr))==-1){ 
    cerr << "bind() error" << endl; 
    exit(EXIT_FAILURE); 
  }     

  if(listen(fd, 1) == -1){ 
    cerr << "listen() error" << endl; 
    exit(EXIT_FAILURE); 
  } 

  sin_size = sizeof(struct sockaddr_in); 
  if ((fd2 = accept(fd,(struct sockaddr *) &client, &sin_size)) == -1){
    cerr << "accept() error" << endl; 
    exit(EXIT_FAILURE); 
  } 
  close(fd2);
  close(fd);
}
