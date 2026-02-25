#include <stdio.h>
#include "pfa.h"

int main()
{
    init_integration("trapezes", 0.01);

    printf("========================================\n");
    printf("TESTS DES FONCTIONS DE DISTRIBUTION\n");
    printf("========================================\n");
    printf("Explication : La fonction phi represente la densite de la loi normale (la courbe en cloche).\n");
    printf("La fonction PHI represente sa fonction de repartition, c'est-a-dire l'aire sous la courbe (une probabilite de 0 a 1).\n\n");

    printf("phi(0.0)    = %f (Attendu : 0.398942) - Point culminant de la cloche.\n", phi(0.0));
    printf("PHI(0.0)    = %f (Attendu : 0.500000) - La moitie de l'aire totale est couverte.\n", PHI(0.0));
    printf("PHI(1.96)   = %f (Attendu : ~0.975002) - Probabilite classique pour un intervalle de confiance.\n\n", PHI(1.96));

    printf("========================================\n");
    printf("TESTS FINANCE : PRIX D'OPTIONS\n");
    printf("========================================\n");
    printf("Explication : Une option est un contrat d'assurance financier. Un 'Call' donne le droit d'acheter une action a un prix fixe K.\n");
    printf("Un 'Put' donne le droit de vendre cette action au prix garanti K.\n");
    printf("S0 = Prix de l'action aujourd'hui, K = Prix garanti pour le futur, T = Temps restant avant l'echeance (en annees).\n");
    printf("mu = Tendance generale d'evolution, sig = Volatilite (mesure du risque ou des variations).\n\n");

    Option call_option = {CALL, 100.0, 100.0, 1.0, 0.05, 0.2};
    Option put_option = {PUT, 100.0, 100.0, 1.0, 0.05, 0.2};

    printf("Prix d'un Call (Droit d'acheter) (S0=100, K=100, T=1, mu=0.05, sig=0.2) : %f (Attendu : ~10.986)\n", optionPrice(&call_option));
    printf("Prix d'un Put  (Droit de vendre) (S0=100, K=100, T=1, mu=0.05, sig=0.2) : %f (Attendu : ~5.859)\n\n", optionPrice(&put_option));

    printf("========================================\n");
    printf("TESTS ASSURANCE : VARIABLES ALEATOIRES\n");
    printf("========================================\n");
    printf("Explication : On simule un client d'une compagnie d'assurance. Ce client peut declarer 0, 1 ou 2 sinistres dans l'annee.\n");
    printf("La variable X represente le cout de remboursement pour UN SEUL sinistre.\n");
    printf("La variable S represente la somme totale remboursee par la compagnie a ce client sur toute l'annee.\n\n");

    double probas[3] = {0.80, 0.15, 0.05};
    InsuredClient client = {0.0, 1.0, probas};

    printf("Profil du client : m = %f, s = %f (Ce sont les parametres mathematiques du cout d'un accident moyen)\n", client.m, client.s);
    printf("Probabilite que le client n'ait AUCUN sinistre (0) : %f\n", probas[0]);
    printf("Probabilite que le client ait UN sinistre (1)      : %f\n", probas[1]);
    printf("Probabilite que le client ait DEUX sinistres (2)   : %f\n\n", probas[2]);

    printf("--- Test pour UN SEUL sinistre (Variable X) ---\n");
    printf("Densite (PDF) pour un cout de 1.0 : clientPDF_X(1.0) = %f (Attendu : 0.398942)\n", clientPDF_X(&client, 1.0));
    printf("Probabilite que le cout du sinistre soit <= 1.0 : clientCDF_X(1.0) = %f (Attendu : 0.500000)\n\n", clientCDF_X(&client, 1.0));

    printf("--- Test pour DEUX sinistres cumules (Variable X1+X2) ---\n");
    printf("Densite (PDF) pour un cout cumule de 2.0 : clientPDF_X1X2(2.0) = %f\n", clientPDF_X1X2(&client, 2.0));
    printf("Probabilite que le cout cumule de ces deux sinistres soit <= 2.0 : clientCDF_X1X2(2.0) = %f\n\n", clientCDF_X1X2(&client, 2.0));

    printf("--- Test pour le TOTAL ANNUEL (Variable S) ---\n");
    printf("Probabilite que l'assureur ne debourse rien du tout : clientCDF_S(0.0) = %f (Attendu : %f, car 80%% de chance d'avoir 0 sinistre)\n", clientCDF_S(&client, 0.0), probas[0]);
    printf("Probabilite que l'assureur debourse au maximum 2.0 sur toute l'annee : clientCDF_S(2.0) = %f\n", clientCDF_S(&client, 2.0));

    return 0;
}