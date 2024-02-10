#ifndef COMMIT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "listCell.h"
#include "ManipBase.h"

typedef struct key_value_pair{
    char* key;
    char* value;
} kvp;

typedef struct hash_table{
    kvp** T;
    int n;
    int size;
} HashTable;

typedef HashTable Commit;

// Exercice 6

kvp* createKeyVal(char* key, char* val);
unsigned long hash(char *str);
void freeKeyVal(kvp* kv);
void freeCommit(Commit *c);
char* kvts(kvp* k);
kvp* stkv(char* str);
Commit* initCommit();
void commitSet(Commit* c, char* key, char* value);
Commit* createCommit(char* hash);
char* commitGet(Commit* c, char* key);
char* cts(Commit* c);
Commit* stc(char* ch);
void ctf(Commit* c, char* file);
Commit* ftc(char* file);
char* blobCommit(Commit* c);

// Exercice 7

char* hashToPathCommit(char* hash);
void initRefs();
void createUpdateRef(char* ref_name, char* hash);
void deleteRef(char* ref_name);
char* getRef(char* ref_name);
void myGitAdd(char* file_or_folder);

void myGitCommit(char* branch_name, char* message);




#endif