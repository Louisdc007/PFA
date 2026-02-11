#define INTEGRATION_C

#include "integration.h"
#include <stdio.h>


bool setQuadFormula(QuadFormula* qf, char* name)
{
	if (qf == NULL) return false;

	strcpy(qf->name, name);
  return true;
}

 //This function is not required ,but it may useful to debug 
void printQuadFormula(QuadFormula* qf)
{
  printf("Quadratic formula: %s\n", qf->name);
  // Print everything else that may be useful 
}





double **subdivisions(double a, double b, int N) {
	if (N == 0) return NULL;

	double **p = malloc(sizeof(double *)*N);

	for (int i = 0; i < N; i++) {

		p[i] = calloc(2, sizeof(double));

		double ai = a + i * ((b-a)/N);
		double bi = a + (i + 1) * ((b-a)/N);

		p[i][0] = ai;
		p[i][1] = bi;
	}
	return p;

}


double median_point_sub(double a, double b) {
	return (a+b)/2;
}

double median_point(double a, double b, double n) {
	return (b-a)/n;
}



// Approximate the integral of function f from a to b.
//   - f is a pointer to a function pointer
//   - a and b are the integration bounds
//   - Interval [a,b] is split in N subdivisions [ai,bi]
//   - Integral of f on each subdivision [ai,bi] is approximated by the quadrature formula qf.


double integrate(double (*f)(double), double a, double b, int N, QuadFormula *qf)
{
	if (f == NULL || qf == NULL) return 0.0;

	if (strcmp(qf->name, "middle")==0) {
		double res = 0;
		double median_point_v = median_point(a, b, N);

		double **sub = subdivisions(a, b, N);

		for (int i = 0; i < N; i++) {
			res += median_point_v * (*f)(median_point_sub(sub[i][0], sub[i][1]));
		}
		return res;
	} else if (strcmp(qf->name, "left")==0) {
			double res = 0;
			double **sub = subdivisions(a, b, N);

			for (int i = 0; i<N; i++) {
				res += (sub[i][1] - sub[i][0]) * (*f)(sub[i][0]);
			}

			return res;

	} else if (strcmp(qf->name, "right") == 0) {
		double res = 0;
		double **sub = subdivisions(a, b, N);

		for (int i = 0; i<N; i++) {
			res += (sub[i][1] - sub[i][0]) * (*f)(sub[i][1]);
		}

		return res;
	} else if (strcmp(qf->name, "trapezes") == 0) {
		double res = 0;
		double **sub = subdivisions(a, b, N);

		for (int i = 0; i < N; i++) {
			res += (sub[i][1] - sub[i][0]) * (0.5 * (*f)(sub[i][0]) + 0.5 * (*f)(sub[i][1]));
		}

		return res;
	}
	return 0.0;
}

/*
double integrate_dx(double (*f)(double), double a, double b, double dx, QuadFormula* qf)
{
  return 0.0;
}
*/


double fun(double a) {
	return a * a;
}

int main() {
	//double (*f)(double) = &fun;
	//printf("%f\n", (*f)(9));
	
	QuadFormula *p = malloc(sizeof(QuadFormula));


	setQuadFormula(p, "left");

	printf("r = %f\n", integrate(&fun, 0, 1, 400000, p));

	return 0;
}

