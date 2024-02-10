#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "commit.h"
#include "branchutils.h"
#include "FonctionHash.h"


void initBranch(){
    FILE *f=fopen(".current_branch","w");
    fputs("master",f);
    fclose(f);
}

int branchExists(char* branch){
    char buffer[256];
    sprintf(buffer,".refs/%s",branch);
    return file_exists(buffer)==1;
}

void createBranch(char* branch){
    char* hash=getRef("HEAD");
    createUpdateRef(branch,hash);
    free(hash);
}

char* getCurrentBranch(){
    FILE* f=fopen(".current_branch","r");
    if(f==NULL){
        printf("Erreur de lecture du fichier .current_branch\n");
        return NULL;
    }
    char buffer[256];
    fgets(buffer,256,f);
    fclose(f);
    return strdup(buffer);
}


void printBranch(char* branch){
    char* hash=getRef(branch);
    char *hashToPathC=hashToPathCommit(hash);
    Commit* c=ftc(hashToPathC);
    free(hashToPathC);
    while(c!=NULL){
        char *mess=commitGet(c,"message");
        if(mess!=NULL){
            printf("Hash: %s -> Message: %s\n",hash,mess);
            free(mess);
        }
        else{
            printf("Hash: %s\n",hash);
        }
        free(hash);
        char *pred=commitGet(c,"predecessor");
        freeCommit(c);
        if(pred!=NULL){
            hash=pred;
            hashToPathC=hashToPathCommit(hash);
            c=ftc(hashToPathC);
            free(hashToPathC);
        }
        else{
            c=NULL;
        }
    }
}

List* branchList(char* branch){
    List* L=initList();
    char* hash=getRef(branch);
    char *hashToPathC=hashToPathCommit(hash);
    Commit* c=ftc(hashToPathC);
    free(hashToPathC);
    while(c!=NULL){
        Cell* cell=buildCell(hash);
        free(hash);
        insertFirst(L,cell);
        char *pred=commitGet(c,"predecessor");
        freeCommit(c);
        if(pred!=NULL){
            hash=pred;
            hashToPathC=hashToPathCommit(hash);
            c=ftc(hashToPathC);
            free(hashToPathC);
        }
        else{
            c=NULL;
        }
    }
    return L;
}

List* getAllCommits(){
    List* Lres=initList();

    List* Ltotal=listdir(".refs");
    for(Cell* c=*Ltotal;c!=NULL;c=c->next){
        List* l=branchList(c->data);
        Cell* cell=*l;
        while(cell!=NULL){
            if(searchList(Lres,cell->data)==NULL){
                insertFirst(Lres,buildCell(cell->data));
            }
            cell=cell->next;
        }
        free_List((*l));
        free(l);
    }
    free_List((*Ltotal));
    free(Ltotal);
    return Lres;
}
