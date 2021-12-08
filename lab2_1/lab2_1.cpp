#include <omp.h>
#include <iostream>
#include <sstream>
#include <ctime>
#include <cmath>

static double f(double x)
{
	return (x * x + 6 * x + 4);
}

static double integral(double func(double x), double A, double B, long long N)
{
	double summ = 0;
	double step = (B - A) / N;
#pragma omp parallel for reduction(+:summ)
	//Для каждой переменной создаются локальные копии в каждом потоке
	//Локальные копии инициализируются как 0
	//Над локальными копиями переменных после выполнения всех операторов параллельной области выполняется сложение
	for (int i = 0; i < N; ++i)
	{
		double a = A + i * step;
		double b = a + step;
		double a_b = (a + b) / 2;

		summ += (b - a) * (func(a) + 4 * func(a_b) + func(b)) / 6;

	}
	return summ;
}

static double diff(double func(double x), double h) {

	
}

int main(char* argv[])
{
	omp_set_num_threads(4);
	int N = atoi(argv[0]);
	int h = N;
	if (h > 3) {
		std::cerr << "Number out of bounds!";
		return 1;
	}
	//std::cout << "Integral equals: " << integral(f, 0, 15, 100000) << std::endl;
	return 0;
}