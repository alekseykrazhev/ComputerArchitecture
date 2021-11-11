#include "mpi.h"
#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static void read_vectors(int* data, int length) {
	for (auto i = 0; i < length; i++)
	{
		data[i] = rand() % 2;
	}
}

int main(int argc, char* argv[])
{
	double res, p_res = 0;
	MPI_Status status;
	int n, myid, numprocs, i, N;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	printf("Input vector length:\n");
	std::cin >> N;
	int* x = new int[N];
	int* y = new int[N];

	if (0 == myid) {
		read_vectors(x, N);
		read_vectors(y, N);
	}

	for (int i = 0; i < N; i++) {
		std::cout << x[i] << ' ';
	}
	std::cout << '\n';
	for (int i = 0; i < N; i++) {
		std::cout << y[i] << ' ';
	}

	MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(y, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	for (i = myid * N / numprocs; i < (myid + 1) * N / numprocs; i++)
		p_res = p_res + x[i] * y[i];
	MPI_Reduce(&p_res, &res, 1, MPI_DOUBLE, MPI_SUM, 0,
		MPI_COMM_WORLD);
	if (0 == myid)
		printf("\n Scalar multiplication = % 10.1f"
			, res);
	MPI_Finalize();
	return 0;
}
