#include "integration.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>


/*
Ici on définit les trucs spécifiques à chaque méthodes de calculs
L'objectif est que pour n'importe quel interval que l'on souhaite caculer, on se ramène à l'interval [0, 1]
pour cela on utilise les formules du pdf et le découpage magique du saucisson
*/
bool setQuadFormula(QuadFormula* qf, char* name)
{
    if (qf == NULL || name == NULL) return false;

    strncpy(qf->name, name, 19);
    qf->name[19] = '\0';

    if (strcmp(name, "left") == 0) {
        qf->nb_points = 1;
        qf->w[0] = 1.0;
        qf->x[0] = 0.0;
    }
    else if (strcmp(name, "right") == 0) {
        qf->nb_points = 1;
        qf->w[0] = 1.0;
        qf->x[0] = 1.0;
    }
    else if (strcmp(name, "middle") == 0) {
        qf->nb_points = 1;
        qf->w[0] = 1.0;
        qf->x[0] = 0.5;
    }
    else if (strcmp(name, "trapezes") == 0) {
        qf->nb_points = 2;
        qf->w[0] = 0.5; qf->w[1] = 0.5;
        qf->x[0] = 0.0; qf->x[1] = 1.0;
    }
    else if (strcmp(name, "simpson") == 0) {
        qf->nb_points = 3;
        qf->w[0] = 1.0/6.0; qf->w[1] = 4.0/6.0; qf->w[2] = 1.0/6.0;
        qf->x[0] = 0.0;     qf->x[1] = 0.5;     qf->x[2] = 1.0;
    }
    else if (strcmp(name, "gauss2") == 0) {
        qf->nb_points = 2;
        qf->w[0] = 0.5;
        qf->w[1] = 0.5;
        qf->x[0] = (1.0 - 1.0/sqrt(3.0)) / 2.0;
        qf->x[1] = (1.0 + 1.0/sqrt(3.0)) / 2.0;
    }
    else if (strcmp(name, "gauss3") == 0) {
        qf->nb_points = 3;
        qf->w[0] = 5.0/18.0;
        qf->w[1] = 8.0/18.0;
        qf->w[2] = 5.0/18.0;
        qf->x[0] = (1.0 - sqrt(3.0/5.0)) / 2.0;
        qf->x[1] = 0.5;
        qf->x[2] = (1.0 + sqrt(3.0/5.0)) / 2.0;
    }
    else {
        return false;
    }

    return true;
}


/*
La on calcul l'intégrale, grâce à la formule du saucisson on retrouve notre interval d'origine
On calcule pour N morceaux
*/
double integrate(double (*f)(double), double a, double b, int N, QuadFormula *qf)
{
    if (f == NULL || qf == NULL || N <= 0) return 0.0;

    double h = (b - a) / N;
    double sum = 0.0;

    for (int i = 0; i < N; i++) {
        double ai = a + i * h;
        for (int k = 0; k < qf->nb_points; k++) {
            double t = ai + qf->x[k] * h;
            sum += qf->w[k] * f(t);
        }
    }

    return sum * h;
}

/*
la différence avec l'autre c'est qu'on choisit la taille du morceau mais pas le nombre
*/
double integrate_dx(double (*f)(double), double a, double b, double dx, QuadFormula* qf)
{
    if (f == NULL || qf == NULL || dx <= 0.0) return 0.0;

    int N = (int)round(fabs(b - a) / dx);
    if (N < 1) N = 1;

    return integrate(f, a, b, N, qf);
}