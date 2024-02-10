#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "listCell.h"
#include "ManipBase.h"
#include "FonctionHash.h"

#include <sys/stat.h>


WorkFile *createWorkFile(char *name){
    WorkFile *wf=malloc(sizeof(WorkFile));
    wf->name=strdup(name);
    wf->hash=NULL;
    wf->mode=0;
    return wf;
}

void free_wf(WorkFile wf){
    free(wf.name);
    free(wf.hash);
}

void free_wt(WorkTree *wt){
    if(wt==NULL) 
        return;
    for(int i=0;i<wt->n;i++){
        free_wf(wt->tab[i]);
    }
    free(wt->tab);
    free(wt);
}

char *searchNameWorkTree(WorkTree *wt, char *str){
    if(wt==NULL){
        return NULL;
    }
    for(int i=0;i<wt->n;i++){
        if(strcmp(wt->tab[i].name,str)==0){
            return wt->tab[i].name;
        }
    }
    return NULL;
}


char *wfts(WorkFile *wf){
    char buffer[1000];
    if(wf->hash!=NULL){
    sprintf(buffer,"%s\t%s\t%d",wf->name,wf->hash,wf->mode);
    } else{
        sprintf(buffer,"%s\tNULL\t%d",wf->name,wf->mode);
    }
    return strdup(buffer);
}

WorkFile *stwf(char *ch){
    char name[256];
    char hash[256];
    int mode;
    sscanf(ch,"%s\t%s\t%d",name,hash,&mode);
    WorkFile *wf=createWorkFile(name);
    if(strncmp(hash,"NULL",4)!=0){
        wf->hash=strdup(hash);
    }
    wf->mode=mode;
    return wf;
}

WorkTree *initWorkTree(){
    WorkTree *wt=malloc(sizeof(WorkTree));
    wt->tab=malloc(sizeof(WorkFile)*SIZE_WORKTREE);
    wt->size=SIZE_WORKTREE;
    wt->n=0;
    return wt;
}


int inWorkTree(WorkTree *wt, char *name){
    for(int i=0;i<wt->n;i++){
        char *curr_name=wt->tab[i].name;
        if(strcmp(name,curr_name)==0)
        {
            return i;
        }
    }
    return -1;
}


int appendWorkTree(WorkTree *wt, char *name,char *hash, int mode){
    if(inWorkTree(wt,name)!=-1 || wt->n==wt->size){
        return 1;
    }
    WorkFile *wf_res=createWorkFile(name);
    if(hash){
        wf_res->hash=strdup(hash);
    }
    wf_res->mode=mode;
    wt->tab[wt->n++]=*wf_res;
    /*
        On free uniquement wf_res et non pas wf_res et ses attributs car les champs name et hash de 
        wt->tab[wt->n] pointent vers la meme zone memoire que ceux de wf_res.
        Donc si on les supprime, on aura des erreurs.
        Il est donc important de penser à les supprimer à la fin dans le main
    */
    free(wf_res); 
    return 0;
}

char *wtts(WorkTree *wt){
    WorkFile *curr;
    char buffer[1000];
    buffer[0]='\0';
    for(int i=0;i<wt->n;i++){
        curr=&(wt->tab[i]);
        char *str=wfts(curr);
        strcat(buffer,str);
        strcat(buffer,"\n");
        free(str);
    }
    return strdup(buffer);
}


int wttf(WorkTree *wt, char *file){
    FILE *f=fopen(file,"w");
    if(f==NULL){
        printf("ERREUR D'OUVERTURE DU FICHIER\n");
        return 1;
    }
    char *str=wtts(wt);
    fprintf(f,"%s",str);
    free(str);
    fclose(f);
    return 0;
}

WorkTree *ftwt(char *file){
    char buffer[256];
    FILE *f=fopen(file,"r");
    if(f==NULL){
        printf("ERREUR D'OUVERTURE DU FICHIER\n");
        return NULL;
    }
    WorkTree *wt=initWorkTree();
    int mode;
    char hash[256];
    char name[256];
    while(fgets(buffer,256,f)){
        sscanf(buffer,"%s\t%s\t%d\n",name,hash,&mode);
        if(strncmp(hash,"NULL",4)==0){
            int val=appendWorkTree(wt,name,NULL,mode);
        }else{
            int val=appendWorkTree(wt,name,hash,mode);
        }
    }
    fclose(f);
    return wt;
}

int getChmod(const char *path)
{
    struct stat ret;

    if(stat(path,&ret)==-1){
        return -1;
    }

    return (ret.st_mode & S_IRUSR) | (ret.st_mode & S_IWUSR) | (ret.st_mode & S_IXUSR) | /*owner*/(ret.st_mode & S_IRGRP) | (ret.st_mode & S_IWGRP) | (ret.st_mode & S_IXGRP) | /*group*/ (ret.st_mode & S_IROTH) | (ret.st_mode & S_IWOTH) | (ret.st_mode & S_IXOTH); /*other*/
}

void setMode(int mode, char *path)
{
    char buff[100];
    sprintf(buff,"chmod %o %s",mode ,path);
    system(buff);
}


char *blobWorkTree(WorkTree *wt){
    int val=wttf(wt,"/tmp/file_tmp.txt");
    char *hash=sha256sum("/tmp/file_tmp.txt");
    if(hash==NULL || val==1)
    {
        return NULL;
    }
    char *path=hashToPath(hash);
    char new_path[256];
    sprintf(new_path,"%s.t",path);
    char buffer[256];
    sprintf(buffer,"[ ! -e %c%c ] && mkdir %c%c",path[0],path[1],path[0],path[1]);
    system(buffer); //Creation de du repertoire ayant pour nom les deux premiére lettre de path uniquement ci celui si n'existe pas
    cp(new_path,"/tmp/file_tmp.txt");
    sprintf(buffer,"rm %s","/tmp/file_tmp.txt");
    system(buffer);
    free(path);
    return hash;
}


char *saveWorkTree(WorkTree *wt, char *path){
    WorkFile *tab=wt->tab;
    char buffer[256];
    char tmp_path[256];
    struct stat buff;
    List *l;
    WorkTree *newWT;
    for(int i=0;i<wt->n;i++){
        sprintf(tmp_path,"%s/%s",path,wt->tab[i].name);
        if(file_exists(tmp_path)==1){
            wt->tab[i].hash=sha256sum(tmp_path);
            wt->tab[i].mode=getChmod(tmp_path);
            blobFile(tmp_path);
        }else if(file_exists(tmp_path)==-1){
            l=listdir(tmp_path);
            newWT=initWorkTree();
            List tmp=*l;
            while(tmp!=NULL){
                int res=appendWorkTree(newWT,tmp->data,NULL,0);
                tmp=tmp->next;
            }
            free_List(*l);
            free(l);
            wt->tab[i].hash=saveWorkTree(newWT,tmp_path);
            free_wt(newWT);
            wt->tab[i].mode=getChmod(tmp_path);

        }
    }
    return blobWorkTree(wt);
}

int isWorkTree (char * hash){
    char buffer[strlen(hash)+3];
    char *hashPath=hashToPath(hash);
    strcpy(buffer,hashPath);
    strcat(buffer,".t");
    if (file_exists(buffer)==1){
        free(hashPath);
        return -1;
    }
    if (file_exists(hashPath)==1){
        free(hashPath);
        return 1;
    }
    free(hashPath);
    return 0;
}


void restoreWorkTree(WorkTree *wt, char *path){
    char absPath[1000];
    char *copyPath;
    char *hash;
    for(int i=0;i<wt->n;i++){
        hash=wt->tab[i].hash;
        sprintf(absPath,"%s/%s",path,wt->tab[i].name);
        copyPath=hashToPath(hash);
        if(isWorkTree(hash)==1){// Dans le cas ou c'est un fichier
            char *abs=strdup(absPath);
            cp(abs,copyPath);
            setMode(getChmod(copyPath),absPath);
            free(abs);
        } else if(isWorkTree(hash)==-1){//Dans le cas ou c'est un repertoire
            strcat(copyPath,".t");
            WorkTree *nwt=ftwt(copyPath);
            restoreWorkTree(nwt,absPath);
            setMode(getChmod(copyPath),absPath);
            free_wt(nwt);
        }
        free(copyPath);
    }
    return;
}




