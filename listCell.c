#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "listCell.h"
#include <sys/stat.h>

#include "FonctionHash.h"

List* initList(){
    List *l=(List*)malloc(sizeof(List));
    *l=NULL;
    return l;
}

void free_List(List l){
    List tmp;
    while(l){
        free(l->data);
        tmp=l;
        l=l->next;
        free(tmp);
    }
}

Cell* buildCell(char* ch){
    Cell* res = (Cell*)malloc(sizeof(Cell));
    res->data=strdup(ch);
    res->next=NULL;
    return res;
}

void insertFirst(List* L,Cell* C){
    C->next=*L;
    *L=C;
}

char* ctos(Cell* c){
    return c->data;
}

char* ltos(List* L){
    char buffer[256];
    buffer[0]='\0';
    Cell* c = *L;
    if(c==NULL)
    {
        return NULL;
    }
    while(c->next!=NULL){
        strcat(buffer,ctos(c));
        strcat(buffer,"|");
        c=c->next;
    }
    strcat(buffer,ctos(c));
    char* tmp=strdup(buffer);
    return tmp;
}

Cell* listGet(List* L, int i){
    Cell* tmp = *L;
    int cpt=0;
    while(tmp!=NULL && cpt<i){
        tmp=tmp->next;
        cpt++;
    }
    return tmp;
}

Cell* searchList(List* L, char* str){
    Cell* l=*L;
    while(l){
        if(strcmp(l->data,str)==0){
            return l;
        }
        l=l->next;
    }
    return NULL;
}


List *stol(char *s){
    char buffer[256];
    List *l=initList();
    int indice=0;
    while((*s)!='\0'){
        if(*s=='|'){
            buffer[indice]='\0';
            insertFirst(l,buildCell(buffer));
            indice=0;
            s++;
        }
        buffer[indice++]=*s;
        s++;
    }
    buffer[indice]='\0';
    if(buffer[0]!='\0'){
        insertFirst(l,buildCell(buffer));
    }
    return l;
}

void ltof(List *L, char *path){
    FILE *f=fopen(path,"w");
    if(!f)
    {
        printf("Erreur de lecture du fichier\n");
        return;
    }
    char *c=ltos(L);
    fprintf(f,"%s\n",c);
    fclose(f);
    free(c);
}

List *ftol(char *path)
{
    FILE *f=fopen(path,"r");
    if(!f)
    {
        printf("Erreur de lecture du fichier\n");
        return NULL;
    }
    char buffer[256];
    fgets(buffer,256,f);
    List *l=stol(buffer);
    fclose(f);
    return l;
}

//Exercice 3

List* listdir(char *root_dir){
    DIR *dp=opendir(root_dir);
    List *l=initList();
    struct dirent *ep;
    if(dp!=NULL){
        while((ep=readdir(dp))!=NULL)
        {
            if(strcmp(ep->d_name,".")!=0 && strcmp(ep->d_name,"..")!=0){
                insertFirst(l,buildCell(ep->d_name));
            }
        }
    }
    closedir(dp);
    return l;
}

/*
int file_exists(char *file){
    List *l=listdir(".");
    Cell *c=searchList(l,file);
    char buffer[256];
    int val_retour;
    if(c!=NULL){
        sprintf(buffer,"[ -f %s ]",file); //On verifie que c'est un fichier et non un repertoires
        val_retour=system(buffer);
        if(val_retour==0)
        {
            return 1;
        }
    }
    return 0;
}
*/

int file_exists(char *file){
    struct stat buff;

    if(stat(file,&buff)==0){
        if(S_ISREG(buff.st_mode)){
            return 1; //Dans le cas ou c'est un fichier
        }
        if(S_ISDIR(buff.st_mode)){
            return -1; //Dans le cas ou c'est un directory
        }
    }
    return 0; //Dans le cas ou c'est ni un fichier, ni un directory
}

void cp(char *to,char *from){
    int val_retour=file_exists(from); //On verifie si le fichiers from existe
    if(val_retour==0)
    {
        printf("ERREUR: %s N'EXISTE PAS\n",from);
        return;
    }
    if(val_retour==-1)
    {
        printf("ERREUR: %s EST UN DIRECTORY\n",from);
        return;
    }
    FILE *f_prov=fopen(from,"r");
    FILE *f_dest=fopen(to,"w");
    if(f_prov==NULL || f_dest==NULL){
        printf("ERREUR D'OUVERTURE D'UN DES DEUX FICHIERS\n");
        return;
    }
    char buffer[256];
    while(fgets(buffer,256,f_prov)){
        fprintf(f_dest,"%s",buffer);
    }
    fclose(f_prov);
    fclose(f_dest);
}

char *hashToPath(char *hash){
    char *hashPath=malloc(sizeof(char)*strlen(hash)+5);//Accorde suffisament d'espace pour pouvoire réaliser les concatenation d'extension avec strcat sans deborder
    strncpy(hashPath,hash,2);
    hashPath[2]='/';
    strncpy(hashPath+3,hash+2,strlen(hash)-2);
    hashPath[strlen(hash)+1]='\0';
    return hashPath;
}


void blobFile(char *file){
    char *hash=sha256sum(file);
    if(hash==NULL)
    {
        return;
    }
    char *ch2=strdup(hash);
    ch2[2]='\0';
    if(file_exists(ch2)!=-1)/*Dans le cas ou le directory n'existe pas*/{
        char buffer[256];
        sprintf(buffer,"mkdir %s",ch2);
        system(buffer); //Creation de du repertoire ayant pour nom les deux premiére lettre de path uniquement ci celui si n'existe pas
    }
    char *path=hashToPath(hash);
    cp(path,file);
    free(hash);
    free(path);
    free(ch2);
}




