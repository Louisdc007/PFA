#include <stdio.h>
#include <math.h>
#include <float.h> // Pour DBL_MAX
#include "integration.h"

// ==========================================
// 1. Définition des fonctions à tester
// ==========================================

double f_poly(double x) { return x * x; }
double f_sin(double x)  { return sin(x); }
double f_exp(double x)  { return exp(x); }
double f_atan(double x) { return 1.0 / (1.0 + x * x); }

// ==========================================
// 2. Fonction utilitaire de test avec comparateur
// ==========================================

void run_test(char* desc, double (*f)(double), double a, double b, double exact_val) {
    char* methods[] = {"left", "right", "middle", "trapezes", "simpson", "gauss2", "gauss3"};
    int N_values[] = {10, 100, 1000};

    int nb_methods = 7;
    int nb_N = 3;

    QuadFormula qf;

    printf("\n=================================================================\n");
    printf("TEST: %s\n", desc);
    printf("Intervalle: [%.2f, %.2f] | Valeur exacte: %.10f\n", a, b, exact_val);
    printf("=================================================================\n");

    // variation N
    for (int j = 0; j < nb_N; j++) {
        int n = N_values[j];

        double results[7];
        double errors[7];
        double min_error = DBL_MAX;

        // variation methodes
        for (int i = 0; i < nb_methods; i++) {
            if (!setQuadFormula(&qf, methods[i])) {
                results[i] = 0.0;
                errors[i] = DBL_MAX;
                continue;
            }

            results[i] = integrate(f, a, b, n, &qf);
            errors[i] = fabs(results[i] - exact_val);

            // cherche error min
            if (errors[i] < min_error) {
                min_error = errors[i];
            }
        }

        // Affichage
        printf("--- N = %d ---\n", n);
        printf("%-10s | %-12s | %-12s | %s\n", "Methode", "Resultat", "Erreur", "Best");
        printf("----------------------------------------------------------\n");

        for (int i = 0; i < nb_methods; i++) {
            // marker pour la best des best
            char* marker = (fabs(errors[i] - min_error) < 1e-15) ? "(*)" : "";

            printf("%-10s | %.8f   | %.2e     | %s\n",
                   methods[i], results[i], errors[i], marker);
        }
        printf("\n");
    }
}

// ==========================================
// 3. Main
// ==========================================

int main() {
    double PI = acos(-1.0);
    double E  = exp(1.0);

    // Test 1: Polynôme x^2 (Intégrale = 1/3)
    // Simpson et Gauss devraient être parfaits ou presque tout de suite
    run_test("f(x) = x^2", f_poly, 0, 1, 1.0/3.0);

    // Test 2: Sinus (Intégrale = 2)
    run_test("f(x) = sin(x)", f_sin, 0, PI, 2.0);

    // Test 3: Exponentielle (Intégrale = e - 1)
    run_test("f(x) = exp(x)", f_exp, 0, 1, E - 1.0);

    // Test 4: 1/(1+x^2) (Intégrale = PI/4)
    run_test("f(x) = 1/(1+x^2)", f_atan, 0, 1, PI / 4.0);

    return 0;
}