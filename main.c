/*
 -----------------------------------------------------------------------------------
 Nom du fichier : main.c
 Auteur(s)      : Ylli Fazlija
                  Kylian Manzini
 Date creation  : 26.04.22

 Description    : Laboratoire 01 - PRG2 : Simulation d'une planche de type
                  "Galton Board".

                  Deux entrées sont demandées à l'utilisateur, le nombre de billes
                  et le nombre de rangées de la planche, dans cet ordre.

                  Affichage du nombre de billes à chaques rangée de la planche,
                  ainsi qu'un histogramme de la ditribution des billes.

 Remarque(s)    : Le programme va demander deux entrées à l'utilisateur puis
                  attendra une pression du clavier pour terminer le programme.

 Compilateur    : Mingw-w64 gcc 11.2.0
 -----------------------------------------------------------------------------------
*/

#include <stdio.h>    // Utilisé pour le I/O du programme (scanf, printf)
#include <stdlib.h>   // Macros utiles (EXIT_SUCCESS)
#include <time.h>     // Pour time(NULL), création d'aléatoire
#include <stdint.h>   // Afin d'utiliser des grandeurs de variables fixes.
#include <assert.h>   // Ajout de assert()
#include <math.h>     // Utilisation de ceil() et log10()
#include <inttypes.h> // Pour PRIu16

// Utiliser la macro CHAINE permet de placer le contenu d'une macro
// (qui n'est pas une chaîne de caractères) dans une chaîne de caractères.
#define CHAINE_CODE(x) #x
#define CHAINE(x) CHAINE_CODE(x)
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

// Chaînes de caractères utilisées dans le programme.
#define MSG_BILLES "Entrez le nombre de billes"
#define MSG_RANGEE "Entrez le nombre de rangees de compteurs"
#define MSG_ERREUR "Saisie incorrecte. Veuillez SVP recommencer."
#define CHAR_VIDE ' '
#define CHAR_HIST '*'

// Limites des intervalles ouvertes.
#define MIN_BILLES 1000
#define MAX_BILLES 30000
#define MIN_RANGEE 10
#define MAX_RANGEE 20

// Hauteur maximale de l'histogramme.
#define PALIER_HIST 15

// Limite de la taille du chiffre en entrée pour éviter les dépassements d'entiers
// non signés.
#define MAX_CHAR 5

/// Permet à l'utilisateur de saisir un nombre entre l'intervalle [min, max]
/// \param message Message de contexte destiné à l'utilisateur
/// \param erreur Message en cas de saisie erronée
/// \param min Valeur minimum
/// \param max Valeur maximum
/// \return uint16_t Valeur saisie par l'utilisateur après vérification
uint16_t saisieIntervalle(char* message,
                          char* erreur,
                          uint16_t min,
                          uint16_t max);

/// Affiche les différents passages des billes dans la planche de Galton
/// et son histogramme
/// \param plancheGalton planche de galton associée
/// \param nbrRangees nombre de rangée de la planche
void affichageSimulationGaltonBoard(const uint16_t** plancheGalton,
                                    uint16_t nbrRangees);

/// Crée une planche de galton avec les paramètres fournis
/// /!\ Attention : nécessaire d'utiliser free(plancheGaltion[0])
///                 et ensuite nécessaire d'utiliser free(plancheGaltion)
///                 afin d'eviter les fuites de mémoires
/// \param nbrRangees nombre de rangées de la planche
/// \param nbrBilles nombre de billes
/// \return tableau de 2 pointeurs
uint16_t** simulationPlancheGalton(uint16_t nbrRangees,
                                   uint16_t nbrBilles);

void viderBuffer(void);

int main(void) {
   // Entrées utilisateurs pour obtenir le nombre de billes et de rangées souhaitées.
   uint16_t nbrBilles = saisieIntervalle(MSG_BILLES, MSG_ERREUR,
                                         MIN_BILLES, MAX_BILLES);
   uint16_t nbrRangees = saisieIntervalle(MSG_RANGEE, MSG_ERREUR,
                                          MIN_RANGEE, MAX_RANGEE);

   // Démarrage de la simulation et récupération d'un pointeur sur les données.
   uint16_t **plancheGalton = simulationPlancheGalton(nbrRangees, nbrBilles);

   affichageSimulationGaltonBoard((const uint16_t **) plancheGalton,
                                  nbrRangees);
   system("pause");

   // Libération de la mémoire allouée pour le stockage des données.
   free(plancheGalton[0]);
   free(plancheGalton);

   return EXIT_SUCCESS;
}

void viderBuffer(void) {
   int c;
   while ((c = getchar()) != '\n' && c != EOF);
}

uint16_t saisieIntervalle(char *message, char *erreur,
                          uint16_t min, uint16_t max) {
   assert(min < max);
   uint32_t entree;

   // La condition du do...while permet de vérifier l'entrée et d'afficher
   // le message d'erreur. Nous avons pris avantage des cas de court-circuitages et
   // de l'opérateur virgule afin de compacter le code.
   do {
      printf("%s [%"PRIu16" - %"PRIu16"] : ", message, min, max);
   } while (
      (!(scanf("%"CHAINE(MAX_CHAR)""PRIu32"%*[^\n]", &entree))
       || (entree > max || entree < min))
       && (viderBuffer(), printf("%s\n", erreur)));

   viderBuffer();
   assert(entree < UINT16_MAX);
   return (uint16_t) entree;
}

void affichageSimulationGaltonBoard(const uint16_t **plancheGalton,
                                    uint16_t nbrRangees) {
   /////////////////////
   // PARTIE PYRAMIDE
   /////////////////////

   size_t indexCompteur = 0;
   uint8_t nbrChiffreSommet = (uint8_t)
      ceil(log10((double) plancheGalton[0][0] + 1));

   uint16_t multiplicateurEspace = nbrRangees;

   printf("\n");
   for (size_t i = 0; i < nbrRangees; ++i, --multiplicateurEspace) {
      for (size_t j = 0; j <= i; ++j, ++indexCompteur) {
         if (!j)
            printf("%*c", (multiplicateurEspace * (nbrChiffreSommet + 1)) / 2,
                   CHAR_VIDE);
         printf("%*"PRIu16, nbrChiffreSommet + 1,
                plancheGalton[0][indexCompteur]);
      }
      printf("%c", '\n');
   }
   printf("%c", '\n');

   /////////////////////
   // PARTIE HISTOGRAMME
   /////////////////////

   // On veut trouver la valeur maxmimum de notre dernière rangée de clous.
   // On fait une recherche linéaire.
   uint16_t maximumTotal = 0;
   uint16_t maximumActuel;
   uint16_t gauche;
   uint16_t droite;

   for (int i = 1; i < nbrRangees + 1; ++i) {
      droite = plancheGalton[1][i];
      gauche = plancheGalton[1][i - 1];
      maximumActuel = MAX(gauche, droite);
      maximumTotal = MAX(maximumTotal, maximumActuel);
   }

   uint16_t palierBarre = maximumTotal / PALIER_HIST;

   uint16_t histogramme[nbrRangees + 1];

   for (int i = 0; i < nbrRangees; ++i) {
      uint16_t nbrBarre = plancheGalton[1][i] / palierBarre;
      histogramme[i] = nbrBarre;
   }
   for (int i = PALIER_HIST; i > 0; --i) {
      printf("%*c", nbrChiffreSommet - 2, CHAR_VIDE);
      for (int j = 0; j < nbrRangees; ++j) {
         if (histogramme[j] >= i)
            printf("%*c", nbrChiffreSommet + 1, CHAR_HIST);
         else
            printf("%*c", nbrChiffreSommet + 1, CHAR_VIDE);
      }
      printf("\n");
   }
   printf("\n");
}

uint16_t** simulationPlancheGalton(uint16_t nbrRangees, uint16_t nbrBilles) {

   // Génération d'une graine aléatoire
   srand((unsigned int) time(NULL));

   uint16_t nbrCloux = (uint16_t)
      (((nbrRangees * (nbrRangees + 1)) / 2) - (nbrRangees));

   uint16_t *compteurBilles = (uint16_t *)
      calloc((size_t) (nbrCloux + nbrRangees + 1), sizeof(uint16_t));

   assert(compteurBilles != NULL); // Assertion si allocation impossible.
   uint16_t *compteurBacBilles = &compteurBilles[nbrCloux];

   // La rangée actuelle
   uint16_t rangeeActuel = 1;
   // Nombre de clous précedant la rangée actuelle
   uint16_t sommeClous = 1;
   // Valeur du compteur du premier clou
   compteurBilles[0] = nbrBilles;

   // Itération à travers tous les clous de la planche
   for (size_t clouActuel = 0; clouActuel < nbrCloux; ++clouActuel) {

      // Condition pour déterminer la rangée actuelle
      if (clouActuel + 1 > sommeClous) {
         rangeeActuel = (uint16_t) (rangeeActuel + 1);
         sommeClous   = (uint16_t) (sommeClous + rangeeActuel);
      }

      // Incrémentation des compteurs sous le compteur "n° du clouActuel" selon
      // la sélection aléatoire
      for (size_t j = 0; j < compteurBilles[clouActuel]; ++j)
         rand() & 1 ? ++compteurBilles[clouActuel + rangeeActuel + 1] :
         ++compteurBilles[clouActuel + rangeeActuel];

   }

   uint16_t** plancheGalton = (uint16_t **) malloc(2 * sizeof(uint16_t *));
   assert(plancheGalton != NULL);

   plancheGalton[0] = compteurBilles;
   plancheGalton[1] = compteurBacBilles;

   return plancheGalton;
}
