#include <omp.h> 
#include <iostream> 
#include <time.h>
# define PI  3.14159265358979323846 

double real_D = 5.87971891616169 + 0.570796326794897;

using namespace std;

double f_1(double x) {
    return 1 - cos(x);
}
double f_2(double x) {
    return x - PI / 2;
}

int main(int argc, char* argv[])
{
    omp_set_num_threads(5);

    const double a = 0;
    const double b = 5;
    int N;
    N = atoi(argv[1]);
    N++;
    int N2 = 4 * N;

    double step = (b - a) / N;
    double step2 = (b - a) / N2;
    double summ = 0;
    double summ2 = 0;

    clock_t start = clock();

#pragma omp parallel for reduction(+:summ)
    for (int i = 1; i < N; i++)
    {
        double a_i = a + i * step;
        double b_i = a_i + step;
        double a_b_i = (a_i + b_i) / 2;

#pragma omp critical
        {
            if (0 < b_i && b_i < PI / 2)
                summ += (b_i - a_i) * (f_1(a_i) + 4 * f_1(a_b_i) + f_1(b_i)) / 6;
            else
                summ += (b_i - a_i) * (f_2(a_i) + 4 * f_2(a_b_i) + f_2(b_i)) / 6;
        }
    }

#pragma omp parallel for reduction(+:summ)
    for (int i = 1; i < N2; i++)
    {
        double a_i2 = a + i * step2;
        double b_i2 = a_i2 + step2;
        double a_b_i2 = (a_i2 + b_i2) / 2;

#pragma omp critical
        {
            if (0 < b_i2 && b_i2 < PI / 2)
                summ2 += (b_i2 - a_i2) * (f_1(a_i2) + 4 * f_1(a_b_i2) + f_1(b_i2)) / 6;
            else
                summ2 += (b_i2 - a_i2) * (f_2(a_i2) + 4 * f_2(a_b_i2) + f_2(b_i2)) / 6;
        }
    }

    cout << "1: for " << N << " points \n integrate is " << summ;
    cout << "\nreal integrate is " << real_D;
    cout << "\nerror of calculus is " << real_D - summ << endl;

    cout << "\n\n2: for " << N2 << " points \n integrate is " << summ2;
    cout << "\nreal integrate is " << real_D;
    cout << "\nerror of calculus is " << real_D - summ2 << endl;

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);
}