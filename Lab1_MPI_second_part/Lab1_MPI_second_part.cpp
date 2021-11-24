#include <iostream>
#include <chrono>
#include <random>
#include <ctime>
#include <vector>
#include <numeric>
#include <mpi.h>

std::vector<int> get_sizes(int vectors_length, int processes_count, int M) {
    int process_size = vectors_length / processes_count;
    int i = vectors_length % processes_count;
    std::vector<int> sizes(processes_count, process_size);
    std::fill(sizes.end() - i, sizes.end(), process_size + 1);
    for (int i = 0; i < sizes.size(); i++)
        sizes[i] *= M;
    return sizes;
}

std::vector<int> get_displacements(const std::vector<int>& sizes) {
    std::vector<int> displ(sizes.size());
    for (int i = 1; i < displ.size(); ++i) {
        displ[i] = displ[i - 1] + sizes[i - 1];
    }

    return displ;
}

int main(int argc, char* argv[]) {
    int processes_count;
    int rank;

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        std::cerr << "Mpi initialization error\n";
        return 0;
    }

    MPI_Comm_size(MPI_COMM_WORLD, &processes_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int L = 3, M = 3;
    MPI_Bcast(&L, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&M, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> sizes = get_sizes(L, processes_count, M);
    std::vector<int> displ;
    std::vector<double> v1, v2, v3;
    std::vector<double> buffer1(sizes[rank]), buffer2(sizes[rank]);

    if (rank == 0) {
        displ = get_displacements(sizes);
        v1.resize(L * M);
        v2.resize(L * M);
        v3.resize(L * M);
        for (int i = 0; i < L; ++i) {
            for (int j = 0; j < M; j++) {
                v1[M * i + j] = 1 + rand() % 4000;
                v2[M * i + j] = 1 + rand() % 4000;
            }
        }
    }

    MPI_Scatterv(v1.data(), sizes.data(), displ.data(), MPI_DOUBLE, buffer1.data(), sizes[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatterv(v2.data(), sizes.data(), displ.data(), MPI_DOUBLE, buffer2.data(), sizes[rank], MPI_DOUBLE, 0, MPI_COMM_WORLD);

    for (int i = 0; i < buffer1.size(); i++) {
        buffer1[i] -= buffer2[i];
    }
    MPI_Gatherv(buffer1.data(), buffer1.size(), MPI_DOUBLE, v3.data(), sizes.data(), displ.data(), MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << "first matrix:" << '\n';
        for (int i = 0; i < L; i++) {
            for (int j = 0; j < M; j++) {
                std::cout << v1[i * M + j] << " ";
            }
            std::cout << '\n';

        }
        std::cout << "second matrix:" << '\n';
        for (int i = 0; i < L; i++) {
            for (int j = 0; j < M; j++) {
                std::cout << v2[i * M + j] << " ";
            }
            std::cout << '\n';
        }
        std::cout << '\n' << "result:" << '\n';
        for (int i = 0; i < L; i++) {
            for (int j = 0; j < M; j++) {
                std::cout << v3[i * M + j] << " ";
            }
            std::cout << '\n';
        }
        std::cout << '\n';

    }

    MPI_Finalize();

    return 0;
}