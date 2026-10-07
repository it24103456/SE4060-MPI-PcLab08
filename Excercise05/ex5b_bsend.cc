#include <cstdio>
#include <cstdlib>
#include <mpi.h>
int main(void)
{
    int rank;
    MPI_Status status;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    char name[MPI_MAX_PROCESSOR_NAME];
    int len;
    MPI_Get_processor_name( name, &len );
    int x[10], y[10];
    if (rank == 1) {
      for (int r = 0; r < 10; r++)
         x[r] = 10*r;

      int bufsize = 10 * sizeof(int) + MPI_BSEND_OVERHEAD;
      void *buffer = malloc(bufsize);
      MPI_Buffer_attach(buffer, bufsize);

      printf("Sending message to computer 3 from computer 1 (Bsend)\n");
      MPI_Bsend(x, 10, MPI_INT, 3, 0, MPI_COMM_WORLD);
      for (int r = 0; r < 10; r++) x[r] = -1;      // safe: data already copied to the buffer
      printf("Rank 1: x overwritten with -1 after Bsend\n");

      MPI_Buffer_detach(&buffer, &bufsize);        // waits until buffered message is delivered
      free(buffer);
    }
    else if (rank == 3) {
      MPI_Recv(y, 10, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
      printf("in computer 3 the value of y is printed\n");
      for (int r = 0; r < 10; r++)
         printf(" %d ", y[r]);
      printf("\n");
    }
    else
      printf("Just a normal process From rank %d machine %s\n", rank, name);
    MPI_Finalize();
}
