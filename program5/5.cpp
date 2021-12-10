#include <cstring>
#include <mpi.h>

#define BUFFER_SIZE 32

char messages[][BUFFER_SIZE] = {
  "Hello", "CSE", "RVCE"
};

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int my_rank, num_procs;
  char message[BUFFER_SIZE];
  MPI_Status status;
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  if (my_rank == 0) {
    for (int i = 1; i < num_procs; ++i) {
      MPI_Recv(message, BUFFER_SIZE, MPI_CHAR, i, 1, MPI_COMM_WORLD, &status);
      printf("Received message %s from process %d\n", message, i);
    }
  } else {
    strcpy(message, messages[my_rank - 1]);
    MPI_Send(message, BUFFER_SIZE, MPI_CHAR, 0, 1, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return 0;
}

