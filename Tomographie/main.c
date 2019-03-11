//
//  main.c
//  Tomographie
//
//  Created by Marion Gain on 04/03/2019.
//  Copyright © 2019 Marion Gain. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// -- Structures --
typedef struct {
    int* lignes; // vecteur lignes
    int* colonnes; //vecteur colonnes
    int* diagM; // vecteur diagonales montantes
    int* diagD; // vecteur diagonales descendantes
    int n ; // largeur et hauteur de l'image
} ensemble;

typedef struct{
    int n; // largeur et hauteur
    int** tab; // pointeur vers la matrice
} image;

// -- Protocoles --

ensemble* initMat ();
ensemble* signature2();
image* initImage();
void DessineImage(image* img);
image* imageAlea(int n, int nbPoints);
void afficheSignature(ensemble* signature);
void afficheImage(image* img);

// Penalités
int penaliteLignes(image* img, ensemble* signature);
int penaliteDiagD(image* img, ensemble* signature);
int penaliteDiagM(image* img, ensemble* signature);
int penaliteColonnes(image* img, ensemble* signature);
int cout (image* img, ensemble* signature);

// recherche d'une image à partir d'une signature
void rechercheImage (ensemble* signature);
void echangeValeur(image* img, int i, int j, int k, int l);
void modification(image* img, ensemble* signature, int c);


int main(int argc, const char * argv[]) {
    // insert code here...
    long temps = 0;
    srand(time(NULL));
    ensemble* s = signature2();
    afficheSignature(s);
    rechercheImage(s);
    
    temps = (long) clock();
    printf("Temps d'execution = %ld ms", temps);
    return 0;
}

void rechercheImage (ensemble* signature){
    // calcul du nombre de 1 dans l'image
    int nombrePoints = 0;
    for(int i = 0; i < signature->n; i++){
        nombrePoints += signature->colonnes[i];
    }
    printf("Nombre points : %d\n", nombrePoints);
    
    // crée une image aléatoire avec nbPoints cases à 1
    image* img = imageAlea(signature->n, nombrePoints);
    DessineImage(img);
   
    printf("Cout de départ : %d\n", cout(img, signature));
    int compteur = 0;
    
    // tant que le cout n'est pas nul ou que l'on a pas atteint les 90 itérations
    // on appelle la méthode d'échange de valeurs
    while (cout(img, signature) != 0 ){ // && compteur < 3000
        printf("Itération n° %d\n", compteur);
        modification(img, signature, cout(img, signature));
        compteur ++;
    }
    printf("Image finale : \n");
    DessineImage(img);
    afficheImage(img);
    printf("Coût final : %d\n", cout(img, signature));
}

// echange les valeurs
void echangeValeur(image* img, int i, int j, int k, int l){

    int temp = img->tab[i][j];
    img->tab[i][j] = img->tab[k][l];
    img->tab[k][l] = temp;
}

void modification(image* img, ensemble* signature, int c){
    int i;
    int j;
    int k;
    int l;
    // choisi deux indices aléatoires pour le changement de valeurs
    do {
        i = rand()%img->n;
        j = rand()%img->n;
    }
    while(img->tab[i][j] != 1);

    do {
        k = rand()%img->n;
        l = rand()%img->n;
    }
    while(img->tab[k][l] != 0);

    // echange les valeurs
    echangeValeur(img, i, j, k, l);
    printf("Image après échange : \n");
  //  DessineImage(img);
    int nouveauCout = cout(img, signature);
    printf("Ancien cout : %d, nouveau cout : %d\n", c, nouveauCout);
    
    // si le nouveau cout est supérieur au précédent on annule l'échange
    if( nouveauCout > c){
        echangeValeur(img,i,j,k,l);
      //  printf("L'image est celle de départ\n");
    }
    
    // sinon on affecte le nouveau cout à c
    else {
     //   printf("L'image n'a pas changée\n");
        c = nouveauCout;
    }
  //  DessineImage(img);
}

// OK
int cout (image* img, ensemble* signature){
    int cout = 0;
   // printf("Penalité diagM : %d\n", penaliteDiagM(img, signature));
    cout += penaliteDiagM(img, signature);
 //   printf("cout : %d\n", cout);
    cout += penaliteDiagD(img, signature);
//    printf("Penalité diagD : %d\n", penaliteDiagD(img, signature));
//    printf("cout : %d\n", cout);
    cout += penaliteLignes(img, signature);
//    printf("Penalité lignes : %d\n", penaliteLignes(img, signature));
//    printf("cout : %d\n", cout);
    cout += penaliteColonnes(img, signature);
//    printf("Penalité colonnes : %d\n", penaliteColonnes(img, signature));
//    printf("cout : %d\n", cout);

    return cout;
}

// Ok
int penaliteDiagM(image* img, ensemble* signature){
     int sommeDiag = 0;
    // premiere moitie
    
    int penalite = 0;
    int k = 0;
    int s = 0;
    for(int i = 0; i < img->n; i++){
        k = 0;
//        printf("Penalite de la diagonale : %d\n", sommeDiag);
//        printf("Pour i = %d\n", i);
        for(int j = i; j >= 0; j--){
            k = i-j;
//            printf("tab[%d][%d] : %d |",k,j,img->tab[k][j]);
            sommeDiag += img->tab[k][j];
        }
        penalite += abs(signature->diagM[s] - sommeDiag);
        sommeDiag = 0;
        s++;
    }

//    printf("Somme première moitié : %d", sommeDiag);
    // deuxieme moitie
    k = 0;
    for(int j = 0; j < (img->n)-1; j ++){
        k = j;
//        printf("Somme de la diagonale : %d\n", sommeDiag);
        for (int i = 3; i > j ; i--) {
            k++;
//            printf("tab[%d][%d] : %d |",i,k,img->tab[i][k]);
            sommeDiag += img->tab[i][k];
        }
        penalite += abs(signature->diagM[s] - sommeDiag);
        sommeDiag = 0;
        s++;
    }
    return penalite;
}

//OK
// initialisation d'une image vide
image* initImage(){
    image* img = (image*) calloc(1, sizeof(image));
    img->tab = (int**) calloc(1, sizeof(int*));
    img->n = 0;
    return img;
}

//OK
// Initialisation d'une matrice à nbPoints 1
image* imageAlea(int n, int nbPoints){
    int compteur = 1;
    int temp;
    // allocations mémoire
    
    //int n = rand()%5;
    image* img = (image*) malloc(sizeof(image));
    img->tab = (int**) malloc(n * sizeof(int*));
    img->n = n;
//    printf("%d %d \n", n, img->n);
    for(int i = 0; i < img->n; i++){
        img->tab[i] = (int*) calloc(img->n, sizeof(int));
    }
    
    // Initialisation du tableau à 0 partout
    for(int i = 0; i < img->n; i++){
        for(int j = 0; j < img->n; j++){
            img->tab[i][j] = 0;
        }
    }
    
    // Cases random à 1
    while (compteur <= nbPoints){
        int i = rand()%(img->n);
        int j = rand()%(img->n);
    
        if(img->tab[i][j] == 0){
            img->tab[i][j] = 1;
            compteur ++;
        }
    }
    return img;
}

// ok
void DessineImage(image* img){
    for (int i = 0; i < img->n; i++){
        for(int j = 0; j < img->n; j++){
            printf("%d | ", img->tab[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// penalite ligne ok
int penaliteLignes(image* img, ensemble* signature){
    int penalite = 0;
    int sommeLigne = 0;
    for (int i = 0; i < img->n; i++){
        for(int j = 0; j < img->n; j++){
            sommeLigne += img->tab[i][j];
        }
//        printf("Somme ligne : %d\n", sommeLigne);
        penalite += abs(signature->lignes[i] - sommeLigne);
        sommeLigne = 0;
    }
    return penalite;
}

//OK
// initialise une matrice
ensemble* initMat (){
    
    // alloue memoire pour la matrice
    ensemble* m = (ensemble*) malloc(sizeof(ensemble));
    m->n = 4;
    //  m->n = rand()%9; // nombre random de taille d'image
    
    // initialise les lignes
    m->lignes = (int*) calloc(m->n, sizeof(int));
    m->lignes[0] = 2;
    m->lignes[1] = 2;
    m->lignes[2] = 2;
    m->lignes[3] = 3;
  
    // initialise les colonnes
    m->colonnes = (int*) calloc(m->n, sizeof(int));
    m->colonnes[0] = 2;
    m->colonnes[1] = 1;
    m->colonnes[2] = 3;
    m->colonnes[3] = 3;
    
    // initialise les diagonales montantes
    int tailleDiag = 2*(m->n)-1;
    m->diagM = (int*) calloc(tailleDiag, sizeof(int));
    m->diagM[0] = 0;
    m->diagM[1] = 0;
    m->diagM[2] = 2;
    m->diagM[3] = 4;
    m->diagM[4] = 1;
    m->diagM[5] = 1;
    m->diagM[6] = 1;

    // initialise les diagonales descendantes
    m->diagD = (int*) calloc(tailleDiag, sizeof(int));
    m->diagD[0] = 1;
    m->diagD[1] = 1;
    m->diagD[2] = 2;
    m->diagD[3] = 1;
    m->diagD[4] = 1;
    m->diagD[5] = 2;
    m->diagD[6] = 1;
    
    return m;
}

// OK
int penaliteDiagD(image* img, ensemble* signature){
    
    // premiere moitie
    int penalite = 0;
    int k = 0;
    int s = 0; // indice de la signature
    int sommeDiag; // somme de chaque diagonale
    for(int i = (img->n)-1; i >= 0; i--){
        //  for (int i = 0; i < img->n; i++) {
        k = 0;
        sommeDiag = 0;
        //  printf("Pour i = %d\n", i);
        //        for (int j = (img->n)-i; j > 0; j--){
        for (int j = 0; j < (img->n)-i; j++) {
            k = i + j;
            //    printf("tab[%d][%d] : %d |",k,j,img->tab[k][j]);
            sommeDiag += img->tab[k][j];
        }
        //   printf("Penalite de la diagonale n° %d : %d\n", i,sommeDiag);
        //    printf("Signature de la diagonale n° : %d = %d\n",s, signature->diagD[s]);
        penalite += abs(signature->diagD[s] - sommeDiag);
        s++;
    }
    
    //  printf("Penalité première moitié : %d", sommeDiag);
    // deuxieme moitie
    for (int i = 1; i < (img->n); i++) {
        k = 0;
        //  printf("Penalite de la diagonale : %d\n", sommeDiag);
        sommeDiag = 0;
        //  printf("Pour i = %d\n", i);
        for (int j = (img->n)-1; j >= i ; j--){
            k = j - i ;
            //   printf("tab[%d][%d] : %d |",k,j,img->tab[k][j]);
            sommeDiag += img->tab[k][j];
        }
        penalite += abs(signature->diagD[s] - sommeDiag);
        s++;
    }
    return penalite;
}

// penalite colonnes OK
int penaliteColonnes(image* img, ensemble* signature){
    int penalite = 0;
    int sommeColonne = 0;
    for (int j = 0; j < img->n; j++){
        for(int i = 0; i < img->n; i++){
            sommeColonne += img->tab[i][j];
        }
//        printf("Somme colonne : %d\n", sommeColonne);
        penalite += abs(signature->colonnes[j] - sommeColonne);
        sommeColonne = 0;
    }
    return penalite;
}

// OK
void afficheSignature(ensemble* s){
    printf("Signature : \n");
    printf("Lignes : \n{ ");
    for (int i = 0; i < s->n; i++) {
        printf("%d ", s->lignes[i]);
    }
    printf("}\nColonnes :\n{ ");
    for (int i = 0; i < s->n; i++) {
        printf("%d ", s->colonnes[i]);
    }
    printf("}\nDiagD :\n{ ");
    for (int i = 0; i < 2*(s->n)-1; i++) {
        printf("%d ", s->diagD[i]);
    }
    printf("}\nDiagDM :\n{ ");
    for (int i = 0; i < 2*(s->n)-1; i++) {
        printf("%d ", s->diagM[i]);
    }
    printf("}\n\n");
}

void afficheImage(image* img){
    for (int i = 0; i < img->n; i++){
        for(int j = 0; j < img->n; j++){
            if(img->tab[i][j]){
                 printf("X | ");
            }
            else {
                printf(" | ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

ensemble* signature2(){
    ensemble* signature = (ensemble*) malloc(sizeof(ensemble));
    signature->n = 8;
    //  m->n = rand()%9; // nombre random de taille d'image
    
    // initialise les lignes
    signature->lignes = (int*) calloc(signature->n, sizeof(int));
    signature->lignes[0] = 2;
    signature->lignes[1] = 2;
    signature->lignes[2] = 3;
    signature->lignes[3] = 6;
    signature->lignes[4] = 3;
    signature->lignes[5] = 2;
    signature->lignes[6] = 2;
    signature->lignes[7] = 2;

    // initialise les colonnes
    signature->colonnes = (int*) calloc(signature->n, sizeof(int));
    signature->colonnes[0] = 1;
    signature->colonnes[1] = 1;
    signature->colonnes[2] = 6;
    signature->colonnes[3] = 3;
    signature->colonnes[4] = 3;
    signature->colonnes[5] = 6;
    signature->colonnes[6] = 1;
    signature->colonnes[7] = 1;
    
    // initialise les diagonales montantes
    int tailleDiag = 2*(signature->n)-1;
    signature->diagM = (int*) calloc(tailleDiag, sizeof(int));
    signature->diagM[0] = 0;
    signature->diagM[1] = 0;
    signature->diagM[2] = 0;
    signature->diagM[3] = 2;
    signature->diagM[4] = 4;
    signature->diagM[5] = 1;
    signature->diagM[6] = 2;
    signature->diagM[7] = 4;
    signature->diagM[8] = 3;
    signature->diagM[9] = 3;
    signature->diagM[10] = 1;
    signature->diagM[11] = 1;
    signature->diagM[12] = 1;
    signature->diagM[13] = 0;
    signature->diagM[14] = 0;

    // initialise les diagonales descendantes
    signature->diagD = (int*) calloc(tailleDiag, sizeof(int));
    signature->diagD[0] = 0;
    signature->diagD[1] = 0;
    signature->diagD[2] = 1;
    signature->diagD[3] = 2;
    signature->diagD[4] = 1;
    signature->diagD[5] = 2;
    signature->diagD[6] = 3;
    signature->diagD[7] = 4;
    signature->diagD[8] = 2;
    signature->diagD[9] = 1;
    signature->diagD[10] = 3;
    signature->diagD[11] = 2;
    signature->diagD[12] = 1;
    signature->diagD[13] = 0;
    signature->diagD[14] = 0;

    return signature;
}
