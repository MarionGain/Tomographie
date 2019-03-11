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
int cout (image* img, ensemble* signature);
void rechercheImage (ensemble* signature);
//void echangeValeur(image* img, ensemble* signature, int c);
void echangeValeur(image* img, int i, int j, int k, int l);
void modification(image* img, ensemble* signature, int c);
void afficheSignature(ensemble* signature);

int main(int argc, const char * argv[]) {
    // insert code here...
     srand(time(NULL));
    image* img = imageAlea(9);
    
    image* im = imageAlea(9);
    ensemble* s = initMat();
//    int lignes [] = {2,2,2,3};
//    int colonnes [] = {2,1,3,3};
//    int diagD [] = {1,1,2,1,1,2,1};
//    int diagM [] = {0,0,2,4,1,1,1};
//    s->colonnes = colonnes;
//    s->lignes = lignes;
//    s->diagD = diagD;
//    s->diagM = diagM;
    DessineImage(img);
    afficheSignature(s);
   
    printf("Cout : %d\n", cout(img, s));
    printf("Cout : %d\n", cout(im, s));
    //rechercheImage(s);
    
    printf("Penalité lignes : %d\n", penaliteLignes(img,s));
    printf("Penalité colonnes : %d\n", penaliteColonnes(img,s));
    printf("Penalité diagM : %d\n", penaliteDiagM(img,s));
    printf("Penalité diagD : %d\n", penaliteDiagD(img,s));
    printf("Cout : %d\n", cout(img, s));
    printf("Penalité lignes : %d\n", penaliteLignes(img,s));
    printf("Penalité colonnes : %d\n", penaliteColonnes(img,s));
    printf("Penalité diagM : %d\n", penaliteDiagM(img,s));
    printf("Penalité diagD : %d\n", penaliteDiagD(img,s));
    return 0;
}

void rechercheImage (ensemble* signature){
    // calcul du nombre de 1 dans l'image
    int nombrePoints = 0;
    for(int i = 0; i < signature->n; i++){
        nombrePoints += signature->colonnes[i];
    }
    printf("Nombre points : %d\n", nombrePoints);
    image* img = imageAlea(nombrePoints);
    DessineImage(img);
    int c = cout(img, signature);
    printf("C : %d\n", c);
    int compteur = 0;
    while (cout(img, signature) != 0 && compteur < 10){
        modification(img, signature, c);
        compteur ++;
    }
   // DessineImage(img);
}

// echange les valeurs
void echangeValeur(image* img, int i, int j, int k, int l){

    // echange les valeurs
    int temp = img->tab[i][j];
    img->tab[i][j] = img->tab[k][l];
    img->tab[k][l] = temp;
}


void modification(image* img, ensemble* signature, int c){
    int i;
    int j;
    int k;
    int l;
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
    DessineImage(img);
    int nouveauCout = cout(img, signature);
    printf("Ancien cout : %d, nouveau cout : %d\n", c, nouveauCout);
    if( nouveauCout > c){
        echangeValeur(img,i,j,k,l);

        // annuler l'echange
    }
    DessineImage(img);
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
        penalite += fabs(signature->diagM[s] - sommeDiag);
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

image* imageAlea(int nbPoints){
    int compteur = 1;
    int temp;
    // allocations mémoire
    
    //int n = rand()%5;
    int n = 4;
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
        penalite += fabs(signature->lignes[i] - sommeLigne);
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
//        printf("Somme colonne : %d\n", sommeColonne);
        penalite += fabs(signature->colonnes[j] - sommeColonne);
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
