#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "commit.h"
#include "branchutils.h"
#include "checkout.h"
#include "FonctionHash.h"

void restoreCommit(char *hash_commit){
    char *commit_path=hashToPathCommit(hash_commit);
    Commit *c=ftc(commit_path);
    char *hash_tree=commitGet(c,"tree");
    char *hash_treepath=hashToPath(hash_tree);
    char buffer[300];
    buffer[0]='\0';
    sprintf(buffer,"%s.t",hash_treepath);
    WorkTree *wt=ftwt(buffer);
    restoreWorkTree(wt,".");
    free(commit_path);
    freeCommit(c);
    free(hash_tree);
    free_wt(wt);
    free(hash_treepath);
}

void myGitCheckoutBranch(char* branch){
    //Modification de la branche courrante
    FILE *f=fopen(".current_branch","w");
    fprintf(f,"%s",branch);
    fclose(f);
    
    //Recuperation du hash du commit de la branch de destination
    char *hash_commit=getRef(branch);
    
    createUpdateRef("HEAD",hash_commit);

    //Restauration du worktree associe au commit dont le hash est hash_commit
    restoreCommit(hash_commit);
    free(hash_commit);
}

List* filterList(List* L, char* pattern){
    List *l_res=initList();
    Cell *c=*L;
    while(c){
        if(strncmp(c->data,pattern,strlen(pattern))==0){
            insertFirst(l_res,buildCell(c->data));
        }
        c=c->next;
    }
    return l_res;
}

void myGitCheckoutCommit(char* pattern){
    List *l_allc=getAllCommits();
    List *l_filterc=filterList(l_allc,pattern);

    if((*l_filterc)==NULL){
        printf("Erreur: aucun commit n'est associé au pattern %s\n",pattern);
        //Liberation de la memoire
        free_List(*l_allc);
        free(l_allc);
        free_List(*l_filterc);
        free(l_filterc);
        return;
    }
    if((*l_filterc)->next==NULL){
        printf("Un seul pattern trouver: %s\n",(*l_filterc)->data);
        char *branch=getCurrentBranch();
        char *commit_hash=(*l_filterc)->data;
        createUpdateRef("HEAD",commit_hash);
        createUpdateRef(branch,commit_hash);
        restoreCommit(commit_hash);
        free(branch);
    }else{
        printf("Veuillez precisez votre requete: \n");
        Cell *c_tmp=(*l_filterc);
        while(c_tmp){
            printf("-> %s\n",c_tmp->data);
            c_tmp=c_tmp->next;
        }
    }
    //Liberation de la memoire
    free_List(*l_allc);
    free(l_allc);
    free_List(*l_filterc);
    free(l_filterc);
}
