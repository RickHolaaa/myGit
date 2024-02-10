#ifndef MANIPBASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define SIZE_WORKTREE 10

typedef struct {
    char* name;
    char *hash;
    int mode;
} WorkFile;

typedef struct {
    WorkFile* tab;
    int size;
    int n;
} WorkTree;


//Fonction de manipulation de base
//MANIPULATION DE WORKFILE
WorkFile *createWorkFile(char *name);
char *wfts(WorkFile *wf);
WorkFile *stwf(char *ch);

//MANIPULATION DE WORKTREE
WorkTree *initWorkTree();
int inWorkTree(WorkTree *wt, char *name);
int appendWorkTree(WorkTree *wt, char *name, char *hash, int mode);
char *wtts(WorkTree *wt);
int wttf(WorkTree *wt, char *file);
WorkTree *ftwt(char *file);

//Enregistrement instantané et restauration d'un WorkTree
int getChmod(const char *path);
void setMode(int mode, char *path);
char *blobWorkTree(WorkTree *wt);
char *saveWorkTree(WorkTree *wt, char *path);
void restoreWorkTree(WorkTree *wt, char *path);

void free_wf(WorkFile wf);
void free_wt(WorkTree *wt);

char *searchNameWorkTree(WorkTree *wt, char *str);

#endif