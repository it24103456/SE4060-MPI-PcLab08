#include <stdio.h>
#include <mpi.h>
#define N 10000000LL
int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    long long chunk = N / size;
    long long start = rank * chunk + 1;
    long long end = (rank == size - 1) ? N : start + chunk - 1;   // last rank takes the remainder
    long long local = 0, total = 0;
    for (long long i = start; i <= end; i++)
        local += i;
    MPI_Reduce(&local, &total, 1, MPI_LONG_LONG, MPI_SUM, 0, MPI_COMM_WORLD);

    double t = MPI_Wtime() - t0, tmax;
    MPI_Reduce(&t, &tmax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0)
        printf("procs=%d sum=%lld expected=%lld time=%.6f\n", size, total, N * (N + 1) / 2, tmax);
    MPI_Finalize();
    return 0;
}
