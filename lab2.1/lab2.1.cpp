#include <omp.h> 
#include <iostream> 
#include <vector> 
#include <time.h>
#define _USE_MATH_DEFINES
#include <cmath>
#define M_PI 3.14159265358979323846
using namespace std;
double f(double x) {
    return sin(x) - x;
}
double secondDerivative(double x, double _x, double x_, double h) {
    return (f(_x) - 2 * f(x) + f(x_)) / pow(h, 2);
}
double accurateFunc(double x) {
    return -1 * sin(x);
}

int main(int argc, char* argv[]) {
    const int ThreadsCount = 5;

    const double a = -1 * M_PI / 2;
    const double b = M_PI / 2;
    cout << "Enter N, please." << endl;
    int N;
    cin >> N;
    N++;

    double max = -1.;
    double point = a;
    double step = (b - a) / N;

    clock_t start = clock();

#pragma omp parallel for num_threads(ThreadsCount)
    for (int i = 0; i < N; i++)
    {
        double  y = a + i * step;
        double x = secondDerivative(y, y - step, y + step, step);
#pragma omp critical
        {
            if (max < x)
            {
                max = x;
                point = y;
            }
        }
    }

    double real_D = accurateFunc(point);
    cout << "for " << N << " points \nmaximum of Second derivative is " << max << " in point " << point;
    cout << endl << "accurate size is " << real_D << endl;
    cout << "\nerror of calculus is " << real_D - max << endl;

    clock_t end = clock();
    double seconds = (double)(end - start) / CLOCKS_PER_SEC;
    printf("The time: %f seconds\n", seconds);
}