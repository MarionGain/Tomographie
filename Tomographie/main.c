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
image* initImage();
void DessineImage(image* img);
image* imageAlea();
int penaliteLignes(image* img, ensemble* signature);
int penaliteDiagD(image* img, ensemble* signature);
int penaliteDiagM(image* img, ensemble* signature);
//int penaliteDiagMontante(image* img, ensemble* signature);
int penaliteColonnes(image* img, ensemble* signature);

int main(int argc, const char * argv[]) {
    // insert code here...
     srand(time(NULL));
    image* img = imageAlea();
    ensemble* s;// = initMat();
    int lignes [] = {2,2,2,3};
    int colonnes [] = {2,1,3,3};
    int diagD [] = {1,1,2,1,1,2,1};
    int diagM [] = {0,0,2,4,1,1,1};
    s->lignes = lignes;
    s->colonnes = colonnes;
    s->diagD = diagD;
    s->diagM = diagM;
    DessineImage(img);
    printf("%d", penaliteDiagM(img, s));

   // printf("%d", penaliteLignes(img));
    return 0;
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
        printf("Penalite de la diagonale : %d\n", sommeDiag);
        printf("Pour i = %d\n", i);
        for(int j = i; j >= 0; j--){
            k = i-j;
            printf("tab[%d][%d] : %d |",k,j,img->tab[k][j]);
            sommeDiag += img->tab[k][j];
        }
        penalite += fabs(signature->diagM[s] - sommeDiag);
        sommeDiag = 0;
        s++;
    }

    printf("Somme première moitié : %d", sommeDiag);
    // deuxieme moitie
    k = 0;
    for(int j = 0; j < (img->n)-1; j ++){
        k = j;
        printf("Somme de la diagonale : %d\n", sommeDiag);
        for (int i = 3; i > j ; i--) {
            k++;
            printf("tab[%d][%d] : %d |",i,k,img->tab[i][k]);
            sommeDiag += img->tab[i][k];
        }
        penalite += fabs(signature->diagM[s] - sommeDiag);
        sommeDiag = 0;
        s++;
    }
    return penalite;
}

image* initImage(){
    image* img = (image*) calloc(1, sizeof(image));
    img->tab = (int**) calloc(1, sizeof(int*));
    img->n = 0;
    return img;
}

image* imageAlea(){
    //int n = rand()%5;
    int n = 4;
    image* img = (image*) malloc(sizeof(image));
    img->tab = (int**) malloc(n * sizeof(int*));
    img->n = n;
    printf("%d %d \n", n, img->n);
    for(int i = 0; i < img->n; i++){
        img->tab[i] = (int*) calloc(img->n, sizeof(int));
    }
    for(int i = 0; i < img->n; i++){
        for(int j = 0; j < img->n; j++){
            img->tab[i][j] = rand()%2;
        }
    }
    return img;
}

void DessineImage(image* img){
    for (int i = 0; i < img->n; i++){
        for(int j = 0; j < img->n; j++){
            printf("%d | ", img->tab[i][j]);
        }
        printf("\n");
    }
}

// penalite ligne ok
int penaliteLignes(image* img, ensemble* signature){
    int penalite = 0;
    int sommeLigne = 0;
    for (int i = 0; i < img->n; i++){
        for(int j = 0; j < img->n; j++){
            sommeLigne += img->tab[i][j];
        }
        printf("Somme ligne : %d\n", sommeLigne);
        penalite += fabs(signature->lignes[i] - sommeLigne);
        sommeLigne = 0;
    }
    return penalite;
}

// initialise une matrice
ensemble* initMat (){
    // alloue memoire pour la matrice
    ensemble* m = (ensemble*) calloc(1, sizeof(ensemble));
    m->n = rand()%9; // nombre random de taille d'image
    
    // initialise les lignes
    m->lignes = (int*) calloc(m->n, sizeof(int));
   
    // initialise les colonnes
    m->colonnes = (int*) calloc(m->n, sizeof(int));

    // initialise les diagonales montantes
    int tailleDiag = 2*(m->n)-1;
    m->diagM = (int*) calloc(tailleDiag, sizeof(int));

    // initialise les diagonales descendantes
    m->diagD = (int*) calloc(tailleDiag, sizeof(int));
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
        penalite += fabs(signature->diagD[s] - sommeDiag);
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
        penalite += fabs(signature->diagD[s] - sommeDiag);
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
        printf("Somme colonne : %d\n", sommeColonne);
        penalite += fabs(signature->colonnes[j] - sommeColonne);
        sommeColonne = 0;
    }
    return penalite;
}
