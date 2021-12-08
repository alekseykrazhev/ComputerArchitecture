#include <omp.h> 
#include <iostream> 
#include <time.h>
# define PI  3.14159265358979323846  /* pi */
double real_D = 6.450515242956587;

using namespace std;

double f_1(double x) {
    return 2 * x - 2;
}
double f_2(double x) {
    return sin(x - 1);
}

int main(int argc, char* argv[])
{
    omp_set_num_threads(5);

    double a;
    const double b = PI / 2 + 1;
    int N;//1000
    cout << "Please enter N and a (a < 1)" << endl;
    cin >> N >> a;
    real_D = -1 * a * a + 2 * a;

    double step = (b - a) / N;
    double summ = 0;

    clock_t start = clock();

#pragma omp parallel for reduction(+:summ)
    for (int i = 1; i < N; i++)
    {
        double a_i = a + i * step;
        double b_i = a_i + step;
        double a_b_i = (a_i + b_i) / 2;

#pragma omp critical
        {
            if (a < b_i && b_i < 1)
                summ += (b_i - a_i) * (f_1(a_i) + 4 * f_1(a_b_i) + f_1(b_i)) / 6;
            else
                summ += (b_i - a_i) * (f_2(a_i) + 4 * f_2(a_b_i) + f_2(b_i)) / 6;
        }
    }

    cout << "for " << N << " points \n integrate is " << summ;
    cout << "\nreal integrate is " << real_D;
    cout << "\nerror of calculus is " << real_D - summ << endl;

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);
}