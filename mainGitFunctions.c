#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "commit.h"
#include "branchutils.h"
#include "merge.h"
#include "checkout.h"
#include "FonctionHash.h"

int main(){

    // Q1, Q2 :
    /*
    kvp* keyval = createKeyVal("cle","valeur");
    printf("kvp : %s\n",kvts(keyval));
    freeKeyVal(keyval);
    //printf("kvp : %s\n",kvts(keyval));
    */

    /*
    // createKeyVal
    printf("Fonction createKeyVal :\n");
    kvp* keyval=createKeyVal("lacle","lavaleur");
    printf("\n");
    // freeKeyVal
    printf("Fonction freeKeyVal :\n");
    freeKeyVal(keyval);
    printf("\n");
    // kvts
    printf("Fonction kvts :\n");
    printf("keyval = %s\n",kvts(keyval));
    printf("\n");
    //stkv
    printf("Fonction stkv :\n");
    char *t=strdup("num:val2");
    kvp* keyval2 = stkv(t);
    printf("kvp2 : %s\n",kvts(keyval2));
    printf("\n");
    // initCommit
    printf("Fonction initCommit :\n");
    Commit* com=initCommit();
    printf("\n");
    // commitSet
    printf("Fonction commitSet :\n");
    commitSet(com,"pomme","rouge");
    commitSet(com,"fraise","rose");
    printf("\n");
    // createCommit
    printf("Fonction createCommit :\n");
    Commit* com2=createCommit("0create");
    printf("\n");
    // commitGet
    printf("Fonction commitGet :\n");
    char *str1=commitGet(com,"pomme");
    if(str1!=NULL){
        printf("%s\n",str1);
    }
    printf("\n");
    // cts
    printf("Fonction cts :\n");
    printf("%s\n",cts(com));
    printf("\n");
    // stc
    printf("Fonction stc :\n");
    char *str=strdup("cle:valeur\ncle2:valeur2");
    Commit *c=stc(str);
    printf("%s\n",cts(c));
    // ctf
    printf("Fonction ctf :\n");
    ctf(com,"fichier_test/fichier_commit.txt");
    printf("\n");
    // ftc
    printf("Test ftc : \n");
    Commit *resc=ftc("fichier_test/fichier_commit.txt");
    printf("%s",cts(resc));
    printf("\n");
    // blobCommit
    printf("Test blobCommit : \n");
    blobCommit(resc);
    printf("\n");


    // initRefs
    printf("Test initRefs : \n");
    initRefs();
    printf("\n");
    // createUpdateRef
    printf("Test createUpdateRef : \n");
    createUpdateRef("test","hashage");
    printf("\n");
    // deleteRef
    printf("Test deleteRef : \n");
    deleteRef("test");
    printf("\n");
    // getRef
    printf("Test getRef : \n");
    printf("%p\n",getRef("test"));
    printf("\n");
    // myGitadd
    printf("Test myGitadd : \n");
    myGitAdd("fichier_test/file.txt");
    printf("\n");
    // myGitCommit
    printf("Test myGitCommit : \n");
    myGitCommit("master","Ceci est un test");
    printf("\n");
    */

   
    kvp *k1=createKeyVal("test","valeur");
    printf("%s:%s\n",k1->key,k1->value);
    char *str_kvp1=kvts(k1);
    printf("%s\n",str_kvp1);
    char *str=strdup("test1:val1");
    kvp *k2=stkv(str);
    char *str_kvp2=kvts(k2);
    printf("%s\n",str_kvp2);
    printf("------------------\n");
    Commit *c1=initCommit();
    commitSet(c1,"test3","val3");
    commitSet(c1,"test4","val4");
    char *str_commit1=cts(c1);
    printf("%s\n",str_commit1);
    printf("------------------\n");
    Commit *c2=stc(str_commit1);
    char *str_commit2=cts(c2);
    printf("%s\n",str_commit2);
    ctf(c1,"fichier_test/file_commit.txt");
    Commit *c3=ftc("fichier_test/file_commit.txt");
    char *str_commit3=cts(c3);
    printf("%s\n",str_commit3);
    char *blob=blobCommit(c1);
    printf("------------------\n");
    printf("Exercice 7\n");
    

    
    //Liberation memoire;
    freeKeyVal(k1);
    freeKeyVal(k2);
    free(str_kvp1);
    free(str);
    free(str_kvp2);
    freeCommit(c1);
    free(str_commit1);
    freeCommit(c2);
    free(str_commit2);
    free(str_commit3);
    freeCommit(c3);
    free(blob);
    
    //Test : Gestion d'une timeline arborescente
    /*
    initBranch();
    printf("Existance de master?: %d\n",branchExists("master"));
    createBranch("Feature1");
    char *res=getCurrentBranch();
    printf("%s\n",res);
    free(res);
    printBranch("master");
    printBranch("Feature1");
    List *branch=branchList("master");
    free_List((*branch));
    free(branch);

    List *getallCommit=getAllCommits();
    Cell *tmp=*getallCommit;
    while(tmp){
        printf("%s\n",tmp->data);
        tmp=tmp->next;
    }
    free_List((*getallCommit));
    free(getallCommit);
    List *lili=initList();
    printf("TESTTTTTTT\n");
    insertFirst(lili,buildCell("tessio"));
    insertFirst(lili,buildCell("test"));
    insertFirst(lili,buildCell("essaie"));
    List *res7=filterList(lili,"lo");
    while((*res7)){
        printf("%s\n",(*res7)->data);
        (*res7)=(*res7)->next;
    }
    */
    //myGitCheckoutBranch("master");
    //myGitCheckoutCommit("7");
    //tester si git checkout hash nous fais revenir à la version du commit du hash
    //tester si git checkout branch nous fais passer de la branche acturelle à branch
    //Et que les choses modi
    //List *res=merge("Feature1","essai");
    
    //createDeletionCommit
    
}