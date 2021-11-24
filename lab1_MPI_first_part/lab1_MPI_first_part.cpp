#include <iostream>
#include <chrono>
#include <random>
#include <ctime>
#include <vector>
#include <numeric>
#include <mpi.h>

std::vector<int> get_sizes(int vectors_length, int processes_count) {
    int process_size = vectors_length / processes_count;
    int i = vectors_length % processes_count;
    std::vector<int> sizes(processes_count, process_size);
    std::fill(sizes.end() - i, sizes.end(), process_size + 1);

    return sizes;
}

std::vector<int> get_displacements(const std::vector<int>& sizes) {
    std::vector<int> displ(sizes.size());
    for (int i = 1; i < displ.size(); ++i) {
        displ[i] = displ[i - 1] + sizes[i - 1];
    }

    return displ;
}

int scalar_product(const std::vector<int>& v1, const std::vector<int>& v2) {
    int result = 0;
    for (int i = 0; i < v1.size(); ++i) {
        result += v1[i] * v2[i];
    }

    return result;
}

int main(int argc, char* argv[]) {
    int processes_count;
    int rank;

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        std::cerr << "Mpi initialization error";
        return 0;
    }

    MPI_Comm_size(MPI_COMM_WORLD, &processes_count);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int vector_length = 0;
    if (rank == 0) {
        std::cout << "Enter vector length\n";
        std::cin >> vector_length;
    }
    MPI_Bcast(&vector_length, 1, MPI_INT, 0, MPI_COMM_WORLD);

    std::vector<int> sizes = get_sizes(vector_length, processes_count);
    std::vector<int> displ;
    std::vector<int> v1, v2;
    std::vector<int> buffer1(sizes[rank]), buffer2(sizes[rank]);

    if (rank == 0) {
        displ = get_displacements(sizes);
        v1.resize(vector_length);
        v2.resize(vector_length);

        for (int i = 0; i < vector_length; ++i) {
            v1[i] = rand() % 2;
            v2[i] = rand() % 2;
        }
    }

    MPI_Scatterv(v1.data(), sizes.data(), displ.data(), MPI_INT, buffer1.data(), sizes[rank], MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatterv(v2.data(), sizes.data(), displ.data(), MPI_INT, buffer2.data(), sizes[rank], MPI_INT, 0, MPI_COMM_WORLD);

    int result = 0;
    int local_scalar_product = scalar_product(buffer1, buffer2);
    MPI_Reduce(&local_scalar_product, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        for (int num : v1) {
            std::cout << num << ' ';
        }
        std::cout << '\n';

        for (int num : v2) {
            std::cout << num << ' ';
        }

        std::cout << "\nresult = " << result;
    }

    MPI_Finalize();

    return 0;
}