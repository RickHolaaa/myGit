#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include "commit.h"
#include "branchutils.h"
#include "checkout.h"
#include "merge.h"
#include "FonctionHash.h"

WorkTree *mergeWorkTrees(WorkTree* wt1, WorkTree* wt2, List** conflicts){
    if(wt1==NULL){
        return wt2;
    }
    if(wt2==NULL){
        return wt1;
    }
    List* conflit_tmp=*conflicts;
    if(conflit_tmp==NULL){
        conflit_tmp=initList();
    }
    WorkTree *wt=initWorkTree();
    for(int i=0;i<wt1->n;i++){
        int pos=inWorkTree(wt2,wt1->tab[i].name);
        if(pos!=-1){
            if(strcmp(wt2->tab[pos].hash,wt1->tab[i].hash)!=0){
                insertFirst(conflit_tmp,buildCell(wt1->tab[i].name));
            }
        }else{
            int val=appendWorkTree(wt,wt1->tab[i].name,wt1->tab[i].hash,wt1->tab[i].mode);
        }
    }
    for(int i=0;i<wt2->n;i++){
        Cell *l=searchList(conflit_tmp,wt2->tab[i].name);
        if(l==NULL){
            int val=appendWorkTree(wt,wt2->tab[i].name,wt2->tab[i].hash,wt2->tab[i].mode);
        }
    }
    return wt;
}

List* merge(char* remote_branch, char* message){
    if(branchExists(remote_branch)!=1){
        printf("Branche %s inexistante\n",remote_branch);
        return NULL;
    }
    //Recuperation des reference des deux branches
    char *curr_branch=getCurrentBranch();
    char *current_hash=getRef(curr_branch);
    char *remote_hash=getRef(remote_branch);
    //Chargement des commit
    char *htp_curhash=hashToPathCommit(current_hash);
    char *htp_remhash=hashToPathCommit(remote_hash);
    Commit *c1=ftc(htp_curhash);
    Commit *c2=ftc(htp_remhash);
    free(htp_curhash);
    free(htp_remhash);
    //Recuperation du hash du WorkTree du point de sauvegarde
    char *str1=commitGet(c1,"tree");
    freeCommit(c1);
    char *str2=commitGet(c2,"tree");
    freeCommit(c2);
    //Recuperation du chemin de ces hash
    char buffer[300];
    char *htp=hashToPath(str1);
    sprintf(buffer,"%s.t",htp);
    free(str1);
    free(htp);
    //Chargement des WorkTree
    WorkTree *wt1=ftwt(buffer);
    htp=hashToPath(str2);
    sprintf(buffer,"%s.t",htp);
    free(str2);
    free(htp);
    WorkTree *wt2=ftwt(buffer);

    List *conflicts=initList();
    //Creation du WorkTree de fusion
    WorkTree *merge_wt=mergeWorkTrees(wt1,wt2,&conflicts);
    free_wt(wt1);
    free_wt(wt2);
    //Verifier si il existe au moins un conflits ou non
    if((*conflicts)!=NULL){
        free(remote_hash);
        free(current_hash);
        free_wt(merge_wt);
        free(curr_branch);
        printf("Erreur merge: il y'a des conflits\n");
        return conflicts;
    }
    free_List(*conflicts);
    free(conflicts);
    
    //Enregistrement instantannée du WorkTree de fusion
    char *hash_wt=blobWorkTree(merge_wt);
    //Creation du commit associé au WorkTree de fusion
    Commit *c=createCommit(hash_wt);
    free(hash_wt);
    //Indication de ces predecesseurs,message
    commitSet(c,"predecessor",current_hash);
    free(current_hash);
    commitSet(c,"merged_predecessor",remote_hash);
    free(remote_hash);
    commitSet(c,"message",message);
    //Enregistrement instantanée du commit
    char* hashc=blobCommit(c);
    freeCommit(c);
    //maj refferences de la branche courante et HEAD
    char *current_branch=getCurrentBranch();
    createUpdateRef(current_branch,hashc);
    free(current_branch);
    createUpdateRef("HEAD",hashc);
    free(hashc);
    //Restauration du projet correspondant au WorkTree de fusion
    restoreWorkTree(merge_wt,".");
    free_wt(merge_wt);
    printf("Fusion de %s avec %s reussie\n",curr_branch,remote_branch);
    free(curr_branch);
    //Suppression reference branche passé en paramettre
    deleteRef(remote_branch);
    printf("La fusion a ete realiser avec succes\n");
    return NULL;
}

void createDeletionCommit(char* branch, List* conflicts, char* message){
    // On récupère le nom de la branche de départ
    char* depart=getCurrentBranch();
    // On se déplace sur la branche branch
    myGitCheckoutBranch(branch);

    // On récupère le dernier commit et le worktree associé
    char* hash=getRef(branch); // hash du dernier commit
    char *hashToPathC=hashToPathCommit(hash); // chemin vers le dernier commit
    free(hash);
    // LE DERNIER COMMIT
    Commit* c1=ftc(hashToPathC);
    free(hashToPathC);
    // LE WORKTREE DU DERNIER COMMIT 
    char *str1=commitGet(c1,"tree");
    // On vide la zone de préparation
    char *hashtmp=hashToPath(str1);
    free(str1);
    char buffer[300];
    buffer[0]='\0';
    sprintf(buffer,"%s.t",hashtmp); // chemin vers le fichier représentant le dernier commit
    free(hashtmp);
    WorkTree *wt=ftwt(buffer);
    // On parcourt le worktree et on ajoute dans .add les fichiers/répertoires ne faisant pas partie de la liste de conflits
    for(int i=0;i<wt->n;i++){
        if(searchList(conflicts,wt->tab[i].name)==NULL){
            myGitAdd(wt->tab[i].name);
        }
    }
    free_wt(wt);
    freeCommit(c1);
    /*
    Dans le cas ou touts les fichiers de conflicts sont dans wt, aucun add n'est fait et donc aucun commit de suppression ne sera creer.
    Il est donc important de traiter ce cas.
    Nous avons fait en sorte que lorsque ce cas à lieu le champs predecessor du commit creer contient le hashdu dernier commit
    et le champs merge_predecessor contient le dernier commit de la branche distante
    */
    if(file_exists(".add")!=1){
        myGitCheckoutBranch(depart);
        free(depart);
        free_List(*conflicts);
        *conflicts=NULL;
        return;
    }
    myGitCommit(branch,message);
    printf("commit de suppression sur la branch %s realise avec succes\n",branch);
    //Retour sur la branche de depart
    myGitCheckoutBranch(depart);
    free(depart);
}
