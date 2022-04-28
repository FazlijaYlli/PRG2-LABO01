/*
 -----------------------------------------------------------------------------------
 Nom du fichier : main.c
 Auteur(s)      : Ylli Fazlija
                  Kylian Manzini
 Date creation  : 26.04.22

 Description    : Laboratoire 01 de PRG2 : Simulation d'une planche de type "Galton Board".

                  Deux entrée sont demandées à l'utilisateurs, le nombre de billes et le nombre de rangées de
                  la planche, dans cet ordre.

                  Affichage du nombre de billes à chaques rangée de la planche, ainsi qu'un histogramme
                  de la ditribution des billes.

 Remarque(s)    : -

 Compilateur    : Mingw-w64 gcc 11.2.0
 -----------------------------------------------------------------------------------
*/

#include <stdio.h>   // Utilisé pour le I/O du programme (scanf, printf)
#include <stdlib.h>  // Macros utiles (EXIT_SUCCESS)
#include <stdint.h>  // Afin d'utiliser des grandeurs de variables fixes.
#include <stdbool.h> // Ajout de paramètres booléens
#include <assert.h> // Ajout de assert

// Utiliser la macro CHAINE permets de placer le contenu d'une macro (qui n'est pas une chaine de caractères)
// dans une chaîne de caractères.
#define CHAINE_CODE(x) #x
#define CHAINE(x) CHAINE_CODE(x)

#define MSG_BILLES "Entrez le nombre de billes"
#define MSG_RANGEE "Entrez le nombre de rangees de compteurs"
#define MSG_ERREUR "Saisie incorrecte. Veuillez SVP recommencer."
#define MIN_BILLES 1000
#define MAX_BILLES 30000
#define MIN_RANGEE 10
#define MAX_RANGEE 20

// Limite de la taille du chiffre en entrée pour éviter les overflow de int.
#define MAX_CHAR 6

uint16_t saisieIntervalle(char* message, char* erreur, uint16_t min, uint16_t max);
void viderBuffer(void);

#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))

void affichageSimulationGaltonBoard(const uint16_t** GALTON_BOARD, uint16_t nbrEtages){

   size_t indexCompteur = 0;
   uint8_t largeurNombreSommet = (uint8_t) ceil(log10(GALTON_BOARD[0][0]));

   for (size_t i = 0 ; i < nbrEtages ; ++i) {
      for(size_t s = 0 ; s < nbrEtages-i ; ++s)
         printf("%*c",(largeurNombreSommet+1)/2 ,' ');
      for(size_t j = 0 ; j <= i ; ++j, ++indexCompteur) {
         printf("%*d",largeurNombreSommet+1 ,GALTON_BOARD[0][indexCompteur]);
      }
      printf("%c",'\n');
   }
   printf("%c",'\n');
}

void affichageHistogramme(const uint16_t** GALTON_BOARD, uint16_t nbrEtages){

   uint16_t maximumTotal = 0;
   uint16_t maximumActuel;
   uint16_t gauche;
   uint16_t droite;

   for (int i = 1; i < nbrEtages+1; ++i) {
      droite = GALTON_BOARD[1][i];
      gauche = GALTON_BOARD[1][i-1];
      maximumActuel = MAX(gauche, droite);
      maximumTotal  = MAX(maximumTotal,maximumActuel);
   }

   uint16_t palierBarre = maximumTotal/PALIER_HIST;

   uint16_t histogramme[nbrEtages+1];

   for (int i = 0; i < nbrEtages; ++i) {
      uint16_t nbrBarre = GALTON_BOARD[1][i]/palierBarre;
      histogramme[i] = nbrBarre;
   }
   for (int i = 15; i > 0; --i) {
      for (int j = 0; j < nbrEtages+1; ++j) {
         if (histogramme[j] >= i) {
            printf("* ");
         } else {
            printf("  ");
         }
      }
      printf("\n");
   }
}

uint16_t** simulationPlancheGalton(uint16_t nbrEtages, uint16_t nbrBilles){

   uint16_t nbrCloux  = (uint16_t) (((nbrEtages * (nbrEtages + 1)) / 2) - (nbrEtages));
   uint16_t* compteurBilles = (uint16_t*) calloc(nbrCloux + nbrEtages + 1, sizeof(uint16_t));
   uint16_t* compteurBacBilles = &compteurBilles[nbrCloux];

   compteurBilles[0] = nbrBilles;
   for (size_t i = 0; i < nbrCloux; ++i){
      uint16_t etageActuel = (uint8_t)((sqrt(1+(double)(8*(i)))-1)/2)+1;
      for (size_t j = 0; j < compteurBilles[i]; ++j) {
        if (rand()%2)
           ++compteurBilles[i + etageActuel + 1];
        else
           ++compteurBilles[i + etageActuel];
      }
   }
   uint16_t** GaltonBoard = (uint16_t**) calloc(2, sizeof(uint16_t*));
   GaltonBoard[0] = compteurBilles;
   GaltonBoard[1] = compteurBacBilles;
   return GaltonBoard;
}

int main(void) {
   uint16_t **GaltonBoard = simulationPlancheGalton(nbrEtages, nbrBilles);
   affichageSimulationGaltonBoard(GaltonBoard,nbrEtages);
   affichageHistogramme(GaltonBoard,nbrEtages);
   system("pause");
   uint16_t nbrEtages = 10;        // de entree utilisateur 50-200
   uint16_t nbrBilles = 1000;      // de entree utilisateur 1000-30000
   return EXIT_SUCCESS;
}


void viderBuffer(void) {
   int c;
   while ((c = getchar()) != '\n' && c != EOF);
}

uint16_t saisieIntervalle(char* message, char* erreur, uint16_t min, uint16_t max) {
   assert(min < max);
   uint16_t entree;
   do {
      printf("%s [%d - %d] : ", message, min, max);
   } while(
      (!(scanf("%"CHAINE(MAX_CHAR)"d%*[^\n]", &entree)) || (entree > max || entree < min))
      && (viderBuffer(),printf("%s\n",erreur)));
   return entree;
}
