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
#include <assert.h>

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

int saisieIntervalle(char* message, char* erreur, int min, int max);
void viderBuffer(void);

int main(void) {

   int* donnees = malloc(2*sizeof(int));
   *(donnees) = saisieIntervalle(MSG_BILLES,MSG_ERREUR,MIN_BILLES,MAX_BILLES);
   *(donnees + 1) = saisieIntervalle(MSG_RANGEE,MSG_ERREUR,MIN_RANGEE,MAX_RANGEE);

   return EXIT_SUCCESS;
}


void viderBuffer(void) {
   int c;
   while ((c = getchar()) != '\n' && c != EOF);
}

int saisieIntervalle(char* message, char* erreur, int min, int max) {
      assert(min < max);
      int entree;
      do {
         printf("%s [%d - %d] : ", message, min, max);
      } while(
         (!(scanf("%"CHAINE(MAX_CHAR)"d%*[^\n]", &entree)) || (entree > max || entree < min))
         && (viderBuffer(),printf("%s\n",erreur)));
      return entree;
}
