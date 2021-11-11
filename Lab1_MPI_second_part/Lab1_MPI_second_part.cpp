#include <iostream>
#include <thread>
#include <Windows.h>
//#include "Randomizer.h"
#include <mpi.h>

using namespace std;


int num = 10; // размер массива
int* mas = new int[num]; // Выделение памяти для массива

void Creat_mas() {
    printf("\n\n");
    for (int i = 0; i < num; i++) {
        // Заполнение массива и вывод значений его элементов
        mas[i] = i;
        printf("poc_mas[%d] = %d\n", i, mas[i]);
    }
}

void show_mas() {

}

int main(int argc, char** argv) {
    int ProcNum, ProcRank;
    int delta;
    int ost;
    int min;
    int max;

    MPI_Init(&argc, &argv);//инициализация начала MPI-программы
    MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);//функция расчёта количества процессоров
    MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);//функция расчёта ранга процессоров

    delta = num / ProcNum;
    //printf("\ndelta: %d / %d = %d\n", num, ProcNum, delta);
    ost = num % ProcNum;
    //printf("ost: %d / %d = %d\n", num, ProcNum, ost);
    min = ProcRank * delta;
    //printf("imin: %d * %d = %d\n", ProcRank, delta, min);
    max = (ProcRank + 1) * delta;


    if (ProcRank == 0)
    {
        Creat_mas();
        printf("\n\nProc - %d\n", ProcRank);
        if (ProcRank == (ProcNum - 1)) max += ost;
        for (int i = min; i < max; i++)
        {
            mas[i] += 10;
            printf("\npoc_C[%d] = %d", i, mas[i]);
        }
    }
    else
    {
        printf("\n\nProc - %d\n", ProcRank);
        if (ProcRank == (ProcNum - 1)) max += ost;
        for (int i = min; i < max; i++)
        {
            mas[i] += 5;
            printf("\npoc_C[%d] = %d", i, mas[i]);
        }
    }
    MPI_Finalize();
    return 0;
}