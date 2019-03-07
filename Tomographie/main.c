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
int penaliteDiagD(image* img);
int penaliteDiag(image* img);

int main(int argc, const char * argv[]) {
    // insert code here...
     srand(time(NULL));
    image* img = imageAlea();
    ensemble* s = initMat();
    DessineImage(img);
    printf("%d", penaliteDiagD(img));

   // printf("%d", penaliteLignes(img));
    return 0;
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

int penaliteLignes(image* img, ensemble* signature){
    int penalite = 0;
    int sommeLigne = 0;
    for (int i = 0; i < img->n; i++){
        for(int j = 0; j < img->n; j++){
            sommeLigne += img->tab[i][j];
            
        }
        penalite += signature->lignes[i] - sommeLigne;
        sommeLigne = 0;
    }
    return penalite;
}

// OK
int penaliteDiagD(image* img){
    
    // premiere moitie
    int penalite = 0;
    int temp = 0;
    int k = 0;
    for (int i = 0; i < img->n; i++) {
        k = 0;
        printf("Penalite de la diagonale : %d\n", temp);
        temp = 0;
        printf("Pour i = %d\n", i);
        for (int j = 0; j < ((img->n)-i); j++) {
            k = i + j;
            printf("tab[%d][%d] : %d |",k,j,img->tab[k][j]);
            temp = img->tab[k][j];
            
            penalite += temp;
        }
    }
    printf("Penalité première moitié : %d", penalite);
    // deuxieme moitie
    for (int i = 1; i < (img->n); i++) {
        k = 0;
        printf("Penalite de la diagonale : %d\n", temp);
        temp = 0;
        printf("Pour i = %d\n", i);
        for (int j = (img->n)-1; j >= i ; j--) {
            k = j - i ;
            printf("tab[%d][%d] : %d |",k,j,img->tab[k][j]);
            temp = img->tab[k][j];
            
            penalite += temp;
        }
    }
    return penalite;
}

//int penaliteDiag(image* img){
//    int penalite = 0;
//    int temp = 0;
//    int k = 0;
//    for (int i = 1; i < (img->n); i++) {
//        k = 0;
//        printf("Penalite de la diagonale : %d\n", temp);
//        temp = 0;
//        printf("Pour i = %d\n", i);
//        for (int j = (img->n)-1; j >= i ; j--) {
//            k = j - i ;
//            printf("tab[%d][%d] : %d |",k,j,img->tab[k][j]);
//            temp += img->tab[k][j];
//
//            penalite += temp;
//        }
//    }
//    return penalite;
//}



int penaliteColonnes(image* img){
    int penalite = 0;
    
    
    return penalite;
}



int penaliteDiagM(image* img){
    
    // premiere moitie
    
    int penalite = 0;
    int temp = 0;
    int k = 0;
    for (int i = (img->n)-1; i >= 0; i--) {
        k = 0;
        printf("Penalite de la diagonale : %d\n", temp);
        temp = 0;
        printf("Pour i = %d\n", i);
        for (int j = 0; j <= i; j++) {
            k = i - j;
            printf("tab[%d][%d] : %d |",k,j,img->tab[k][j]);
            temp += img->tab[k][j];
            
            penalite += temp;
        }
    }
    
    // deuxieme moitie
    
    
    
    
    
    return penalite;
}


// initialise une matrice avec 1 et 0 aleatoires
ensemble* initMat (){
    // alloue memoire pour la matrice
    ensemble* m = (ensemble*) calloc(1, sizeof(ensemble));
    m->n = rand()%9; // nombre random de taille d'image
    
    // initialise les lignes
    m->lignes = (int*) calloc(m->n, sizeof(int));
    for (int i = 0; i < m->n; i ++){
        m->lignes[i] = rand()%2;
        //  printf("%d", m->lignes[i]);
    }
    //  printf("\n");
    
    // initialise les colonnes
    m->colonnes = (int*) calloc(m->n, sizeof(int));
    for (int i = 0; i < m->n; i ++){
        m->colonnes[i] = rand()%2;
        //printf("%d", m->colonnes[i]);
    }
    //  printf("\n");
    
    // initialise les diagonales montantes
    int tailleDiag = 2*(m->n)-1;
    m->diagM = (int*) calloc(tailleDiag, sizeof(int));
    for (int i = 0; i < tailleDiag; i ++){
        m->diagM[i] = rand()%2;
        // printf("%d", m->diagM[i]);
    }
    // printf("\n");
    
    // initialise les diagonales descendantes
    m->diagD = (int*) calloc(tailleDiag, sizeof(int));
    for (int i = 0; i < tailleDiag; i ++){
        m->diagD[i] = rand()%2;
        //printf("%d", m->diagD[i]);
    }
    // printf("\n");
    return m;
}

