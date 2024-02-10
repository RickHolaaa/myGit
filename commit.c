#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "commit.h"
#include "FonctionHash.h"


#define TAILLE_MAX 5

//Gestion des commits

kvp* createKeyVal(char* key, char* val){
    kvp* res = (kvp*)malloc(sizeof(kvp));
    res->key=strdup(key);
    res->value=strdup(val);
    return res;
}

void freeKeyVal(kvp* kv){
    free(kv->key);
    free(kv->value);
    free(kv);
}

void freeCommit(Commit *c){
    if(c==NULL){
        return;
    }
    for(int i=0;i<c->size;i++){
        if(c->T[i]!=NULL){
            freeKeyVal(c->T[i]);
        }
    }
    free(c->T);
    free(c);
}

char *kvts(kvp *k){
    char buffer[256];
    sprintf(buffer,"%s:%s",k->key,k->value);
    return strdup(buffer);
}

kvp *stkv(char *str){
    char *key=strtok(str,":");
    char *val=strtok(NULL,":");
    return createKeyVal(key,val);
}



Commit* initCommit(){
    Commit* res = (Commit*)malloc(sizeof(Commit));
    res->n=0;
    res->size=TAILLE_MAX;
    res->T=(kvp**)(malloc(sizeof(kvp*)*TAILLE_MAX));
    for(int i=0;i<res->size;i++)
    {
        res->T[i]=NULL;
    }
    return res;
}

//Fonction de hachage choisi : lose lose
unsigned long hash(char *str){
    int hash=0;
    int c;
    while (c=*str++){
        hash +=c;
    }
    return hash;
}

void commitSet(Commit* c, char* key, char* value){
    if(c->n<c->size){
        int num=(hash(key))%(c->size);
        while(c->T[num]!=NULL){
            num=(num+1)%(c->size); //Probing lineaire
        }
        c->T[num]=createKeyVal(key,value);
        c->n++;
    }else{
        printf("Nombre de commit maximum dépassé\n");
    }
}

Commit* createCommit(char* hash){
    Commit* res = initCommit();
    commitSet(res,"tree",hash);
    return res;
}

char* commitGet(Commit* c, char* key){
    if(c==NULL){
        return NULL;
    }
    int num=(hash(key))%(c->size);
    int tmp=num;
    if(c->T[tmp]!=NULL && strcmp(key,c->T[tmp]->key)==0){
        return strdup(c->T[tmp]->value);
    }
    tmp++;
    for(int i=tmp;i!=num;i=(i+1)%(c->size)){
        if(c->T[i]!=NULL && strcmp(key,c->T[i]->key)==0){
            return strdup(c->T[i]->value);
        }
    }
    return NULL;
}

char* cts(Commit* c){
    kvp** tmp=c->T;
    char res[256];
    res[0]='\0';
    int i;
    for(i=0;i<c->size;i++){
        if(tmp[i]!=NULL){
            char *chaine=kvts(tmp[i]);
            strcat(res,chaine);
            strcat(res,"\n");
            free(chaine);
        }
    }
    if(strlen(res)>=1){
        res[strlen(res)-1]='\0';
    }
    return strdup(res);
}

Commit* stc(char* ch){
    int i=0;
    int indice=0;
    char buffer[256];
    buffer[0]='\0';
    Commit* res = initCommit();
    kvp* tmp;

    while(ch[i]!='\0'){
        if(ch[i]=='\n'){
            // entre indice et i-1
            strncpy(buffer,ch+indice,i);
            buffer[i]='\0';
            tmp=stkv(buffer);
            commitSet(res,tmp->key,tmp->value);
            indice=i+1;
            buffer[0]='\0';
            freeKeyVal(tmp);
        }
        i++;
    }
    strncpy(buffer,ch+indice,i);
    buffer[i]='\0';
    tmp=stkv(buffer);
    commitSet(res,tmp->key,tmp->value);
    freeKeyVal(tmp);
    return res;
}


void ctf(Commit* c, char* file){
    FILE *f = fopen(file,"w");
    if(f==NULL){
        printf("Erreur ouverture fichier %s\n",file);
        return ;
    }
    char *c_str=cts(c);
    fprintf(f,"%s\n",c_str);
    fclose(f);
    free(c_str);
}

Commit* ftc(char* file){
    FILE *f = fopen(file,"r");
    if(f==NULL){
        printf("Erreur ouverture fichier %s\n",file);
        return NULL;
    }

    char buffer[256];
    Commit* res=initCommit();
    char key[256], value[256];

    while(fgets(buffer,256,f)){
        if(buffer[strlen(buffer)-1]=='\n'){
            buffer[strlen(buffer)-1]='\0';
        }
        kvp* tmp=stkv(buffer);
        commitSet(res,tmp->key,tmp->value);
        freeKeyVal(tmp);
    }
    fclose(f);
    return res;
}


char *blobCommit(Commit* c){
    ctf(c,"/tmp/file_tmp.txt");
    char *hash=sha256sum("/tmp/file_tmp.txt");
    if(hash==NULL)
    {
        return NULL;
    }
    char *path=hashToPath(hash);
    char new_path[256];
    sprintf(new_path,"%s.c",path);
    char buffer[256];
    sprintf(buffer,"[ ! -e %c%c ] && mkdir %c%c",path[0],path[1],path[0],path[1]);
    system(buffer); //Creation de du repertoire ayant pour nom les deux premiére lettre de path uniquement ci celui si n'existe pas
    cp(new_path,"/tmp/file_tmp.txt");
    sprintf(buffer,"rm %s","/tmp/file_tmp.txt");
    system(buffer);
    free(path);
    return hash;
}


// Exercice 7

void initRefs(){
    if(file_exists(".refs")!=-1){
        system("mkdir .refs");
        system("touch .refs/master");
        system("touch .refs/HEAD");
    }
}

void createUpdateRef(char* ref_name, char* hash){
    char buffer[256];
    char cmd[256];
    if(file_exists(ref_name)!=1){
        sprintf(cmd,"touch .refs/%s",ref_name);
        system(cmd);
    }
    sprintf(buffer,"echo %s > .refs/%s",hash,ref_name);
    system(buffer);
}

void deleteRef(char* ref_name){
    char cmd[260];
    char chemin[256];
    sprintf(chemin,".refs/%s",ref_name);
    if(file_exists(chemin)==1){

        sprintf(cmd,"rm %s",chemin);
        int val=system(cmd);
        if(val==0){
            printf("Suppression de %s réussie\n",ref_name);
        }
        else{
            printf("Erreur de suppression de la référence %s\n",chemin);
        }
    }
}

char* getRef(char* ref_name){
    char chemin[256];
    sprintf(chemin,".refs/%s",ref_name);
    if(file_exists(chemin)!=1){ // Si le fichier n'existe pas
        printf("La reference n'existe pas\n");
        return NULL;
    }
    char cmd[350];
    sprintf(cmd,"g=$(cat %s) && [ -z \"$g\" ]",chemin);
    int val=system(cmd);
    if(val==0){ // Fichier vide
        return strdup("");
    }
    else{
        FILE *f=fopen(chemin,"r");
        char buffer[256];
        fgets(buffer,256,f); // On récupère le hash
        buffer[strlen(buffer)-1]='\0';
        char* tmp = strdup(buffer);
        fclose(f);
        return tmp;
    }
}

void myGitAdd(char *file_or_folder){
    WorkTree* wt;
    if(file_exists(".add")!=1){ // On vérifie si .add existe
        system("touch .add");
        wt=initWorkTree();
    } else{
        wt=ftwt(".add");
    }
    // Puis on vérifie si file_or_folder existe
    if(file_exists(file_or_folder)!=0){
        // Si oui, on ajoute le fichier dans le worktree et on actualise le fichier .add
        int n=appendWorkTree(wt,file_or_folder,NULL,0);
        wttf(wt,".add");
    }
    free_wt(wt);
}

//Commit

void myGitCommit(char* branch_name, char* message){
    // Si le dossier .refs n'existe pas
    if(file_exists(".refs")!=-1){
        printf("Initialiser d'abord les références du projet\n");
        return ;
    }
    char chemin[256];
    sprintf(chemin,".refs/%s",branch_name);
    // Si le branch_name n'existe pas
    if(file_exists(chemin)!=1){
        printf("La branche %s n'existe pas\n",branch_name);
        return ;
    }
    // Si les fichiers HEAD et branch_name ne pointent pas vers la même chose
    char* last_hash=getRef(branch_name);
    char* head_hash=getRef("HEAD");
    if(strcmp(last_hash,head_hash)!=0){
        printf("HEAD doit pointer sur le dernier commit de la branche\n");
        free(last_hash);
        free(head_hash);
        return ;
    }

    // Charger le WorkTree et supprimer .add
    free(head_hash);

    if(file_exists(".add")!=1){
        printf("commit impossible: fichier .add non existant\n");
        free(last_hash);
        return;
    }
    WorkTree* wt = ftwt(".add");
    if(wt!=NULL){
        system("rm .add");
        printf("Suppression de .add réussie\n");
    }

    // Enregistrer un instantané de ce Worktree
    char* hash = saveWorkTree(wt,".");
    free_wt(wt);
    char *hash_path=hashToPath(hash);
    printf("---------------------------------------------\n");
    printf("Localisation de l'instantané du wt: %s.t\n",hash_path);
    printf("---------------------------------------------\n");
    free(hash_path);

    // Créer un commit
    Commit* c = createCommit(hash);
    free(hash);
    // Lire le fichier branch_name
    if(strlen(last_hash)!=0){
        commitSet(c,"predecessor",last_hash);
    }
    free(last_hash);
    // Si l'argument message n'est pas NULL
    if(message!=NULL){
        commitSet(c,"message",message);
    }
    commitSet(c,"author",getenv("USER"));

    char* hashc=blobCommit(c);
    printf("Localisation de l'instantané du commit: %s.c\n",hashc);
    printf("---------------------------------------------\n");
    freeCommit(c);

    createUpdateRef(branch_name,hashc);
    createUpdateRef("HEAD",hashc);
    free(hashc);
    printf("commit sur la branch %s realise avec succes\n",branch_name);
}

char* hashToPathCommit(char* hash){
    char* buffer=(char*)malloc(256*sizeof(char));
    char *h=hashToPath(hash);
    sprintf(buffer,"%s.c",h);
    free(h);
    return buffer;
}