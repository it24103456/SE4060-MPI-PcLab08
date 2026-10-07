// Monte Carlo estimate of pi using 10,000,000 random points split across MPI ranks.
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#define TOTAL 10000000LL
static unsigned int next_rand(unsigned int *s) { *s = *s * 1664525u + 1013904223u; return *s >> 1; }
int main(int argc, char **argv)
{
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    MPI_Barrier(MPI_COMM_WORLD);
    double t0 = MPI_Wtime();

    long long n = TOTAL / size;
    if (rank == size - 1) n += TOTAL % size;
    unsigned int seed = 12345u + rank * 7919u;       // different stream per rank
    long long local = 0;
    for (long long i = 0; i < n; i++) {
        double x = next_rand(&seed) / 2147483647.0;
        double y = next_rand(&seed) / 2147483647.0;
        if (x * x + y * y <= 1.0) local++;
    }

    long long total = local;
    if (rank == 0) {
        for (int i = 1; i < size; i++) {
            long long c;
            MPI_Status st;
            MPI_Recv(&c, 1, MPI_LONG_LONG, i, 0, MPI_COMM_WORLD, &st);   // from a specific rank
            total += c;
            printf("  received %lld from rank %d\n", c, st.MPI_SOURCE);
        }
    } else {
        MPI_Send(&local, 1, MPI_LONG_LONG, 0, 0, MPI_COMM_WORLD);
    }

    double t = MPI_Wtime() - t0, tmax;
    MPI_Reduce(&t, &tmax, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    if (rank == 0)
        printf("procs=%d pi=%.8f inside=%lld time=%.6f\n", size, 4.0 * total / TOTAL, total, tmax);
    MPI_Finalize();
    return 0;
}
