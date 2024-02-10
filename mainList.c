#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "listCell.h"

//Fonction me permettant d'afficher ma liste
void affiche(List *l)
{
    Cell *c=*l;
    while(c)
    {
        printf("%s\n",c->data);
        c=c->next;
    }
}

//Fonction me permettant de supprimer ma liste
void supp(List *l)
{
    Cell *c=*l;
    Cell *tmp;
    while(c)
    {
        tmp=c;
        c=c->next;
        free(tmp->data);
        free(tmp);
    }
    free(l);
}

int main(){

    //Test de initList
    List* l=initList();

    //Test de buildCell
    Cell* c3 = buildCell("Test");
    Cell* c2 = buildCell("du");
    Cell* c1 = buildCell("programme");

    //Test de insertFirst
    insertFirst(l,c1);
    insertFirst(l,c2);
    insertFirst(l,c3);

    //Affichage de la liste l
    affiche(l);

    //Test de ctos
    printf("Test de ctos\n");
    char* test = ctos(*l);
    printf("%s\n",test);

    //Test de ltos
    printf("Test de ltos\n");
    char* test2 = ltos(l);
    printf("%s\n",test2);

    //Test de listGet
    printf("Test de listGet\n");
    Cell* trouv2 = listGet(l,2);
    if(trouv2!=NULL){
        printf("Trouvé : %s\n",ctos(trouv2));
    }else{
        printf("NULL\n");
    }

    //Test de searchList
    printf("Test de searchList\n");
    Cell* trouv = searchList(l,"d");
    if(trouv!=NULL){
        printf("Trouvé : %s\n",ctos(trouv));
    }else{
        printf("NULL\n");
    }
    
    //Test de stol
    printf("Test de stol\n");
    List *res=stol(test2);
    affiche(res);
    free(test2); //Liberation en memoire de test2

    //Test de ltof
    printf("Test de ltof\n");
    ltof(l,"fichier_test/file.txt");

    //Test de ftol
    printf("Test de ftol\n");
    List *res2=ftol("fichier_test/file.txt");
    affiche(res2);

    //Test de listdir
    List *res3=listdir("/mnt/c/Users/essab/Desktop/projet_scv");
    affiche(res3);

    //Test de file_exists
    printf("%d\n",file_exists("fichier_test/test_liste"));

    //Test de cp
    cp("fichier_test/file2.txt","fichier_test/file1.txt");

    //Test de hashToPath
    char *res4=hashToPath("dqgdfgdfdhh");
    printf("%s\n",res4);
    free(res4);

    //Test de blobFile
    blobFile("main.c");

    //Liberation en memoire des listes
    supp(l);
    supp(res);
    supp(res2);
    supp(res3);

    printf("%d\n",file_exists("fichier_test/fichier_test2"));
    printf("%d\n",file_exists("10/d1eeba8cf6633529f1e8e4d41757d8c047d143e3881e9655e49242acff712"));


    return 0;
}