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

int main(int argc, char* argv[]){
    /*
    Les différentes fonctions disponibles :
        ./myGit init
        ./myGit list-refs
        ./myGit create-ref <name> <hash>
        ./myGit delete-ref <name>
        ./myGit add <elem> [<elem2> <elem3> ...]
        ./myGit list-add
        ./myGit clear-add
        ./myGit commit <branch_name> [-m <message>]
        ./myGit get-current-branch
        ./myGit branch <branch-name>
        ./myGit branch-print <branch-name>
        ./myGit checkout-branch <branch-name>
        ./myGit checkout-commit <pattern>
        ./myGit merge <branch> <message>
    */
    if(argc==1){
        printf("Les différentes fonctions disponibles :\n");
        printf("./myGit init\n");
        printf("./myGit list-refs\n");
        printf("./myGit create-ref <name> <hash>\n");
        printf("./myGit delete-ref <name>\n");
        printf("./myGit add <elem> [<elem2> <elem3> ...]\n");
        printf("./myGit list-add\n");
        printf("./myGit clear-add\n");
        printf("./myGit commit <branch_name> [-m <message>]\n");
        printf("./myGit get-current-branch\n");
        printf("./myGit branch <branch-name>\n");
        printf("./myGit branch-print <branch-name>\n");
        printf("./myGit checkout-branch <branch-name>\n");
        printf("./myGit checkout-commit <pattern>\n");
        printf("./myGit merge <branch> <message>\n");
        return 0;
    }
    // ./myGit init
    if(strcmp(argv[1],"init")==0){
        printf("Dossier .refs initialisé\n");
        initRefs();
        initBranch();
        return 0;
    }

    int val=system("[ -e .refs ]");
    if(val!=0){
        printf("Veuillez initialiser le dossier .refs avec la commande : %s init\n",argv[0]);
        return 0;
    }
    // ./myGit list-refs
    if(strcmp(argv[1],"list-refs")==0){
        int val=system("[ -e .refs ]");
        if(val==0){
            printf("Les références : \n");
            List* l = listdir(".refs");
            for(Cell* c=*l;c!=NULL;c=c->next){
                char *content=getRef(c->data);
                printf("- %s \t %s \n",c->data,content);
                free(content);
            }
            free_List(*l);
            free(l);
        }
        else{
            printf("Le dossier .refs n'est pas initialisé\n");
        }
        return 0;
    }
    // ./myGit create-ref <name> <hash>
    if(strcmp(argv[1],"create-ref")==0){
        if(argc<3){
        printf("Veuillez ajouter au moins un élément\n");
        return 0;
        }
        else{
            if(argc==3){
                printf("Veuillez ajouter un hash\n");
                return 0;
            }
            printf("Référence %s crée avec succès\n",argv[2]);
            createUpdateRef(argv[2],argv[3]);
        }
        return 0;
    }
    // ./myGit delete-ref <name>
    if(strcmp(argv[1],"delete-ref")==0){
        if(argc<3){
        printf("Veuillez ajouter au moins un élément\n");
        return 0;
        }
        else{
            deleteRef(argv[2]);
        }
        return 0;
    }
    // ./myGit add <elem> [<elem2> <elem3> ...]
    if(strcmp(argv[1],"add")==0){
        if(argc<3){
            printf("Veuillez ajouter au moins un élément\n");
            return 0;
        }
        for(int i=2;i<argc;i++){
            printf("Ajout du fichier : %s\n",argv[i]);
            myGitAdd(argv[i]);
        }
        return 0;
    }
    // ./myGit list-add
    if(strcmp(argv[1],"list-add")==0){
        if(file_exists(".add")){
            WorkTree* wt = ftwt(".add");
            char *wt_str=wtts(wt);
            printf("%s",wt_str);
            free_wt(wt);
            free(wt_str);
        }
        else{
            printf("Le fichier .add n'existe pas\n");
        }
        return 0;
    } 
    // ./myGit clear-add
    if(strcmp(argv[1],"clear-add")==0){
        if(file_exists(".add")){
            int val=system("rm .add");
            if(val==0){
                printf("Supression réussie\n");
            }
            else{
                printf("Suppression echouée\n");
            }
        }
        else{
            printf("Le fichier .add n'existe pas\n");
        }
        return 0;
    }
    // ./myGit commit <branch_name> [-m <message>]
    if(strcmp(argv[1],"commit")==0){
        if(argc<3){
            printf("Veuillez ajouter au moins un élément\n");
            return 0;
        }
        else{
            if(argc>=4 && strcmp(argv[3],"-m")!=0){
                printf("L'option %s est non disponible\n",argv[3]);
                return 0;
            }
            if(argc==4 && strcmp(argv[3],"-m")==0){
                printf("Veuillez rajouter un message\n");
                return 0;
            }
            if(argc==5 && strcmp(argv[3],"-m")==0){
                myGitCommit(argv[2],argv[4]);
            }
            else{
                myGitCommit(argv[2],NULL);
            }
        }
        return 0;
    }
    // ./myGit get-current-branch
    if(strcmp(argv[1],"get-current-branch")==0){
        char *curr_branch=getCurrentBranch();
        printf("%s\n",curr_branch);
        free(curr_branch);
        return 0;
    }
    // ./myGit branch <branch-name>
    if(strcmp(argv[1],"branch")==0){
        if(branchExists(argv[2])==0){
            createBranch(argv[2]);
            printf("Branche %s creer avec succes !\n",argv[2]);
        }else{
            printf("Erreur: La branche existe deja\n");
        }
        return 0;
    }
    // ./myGit branch-print <branch-name>
    if(strcmp(argv[1],"branch-print")==0){
        if(branchExists(argv[2])==0){
            printf("Erreur: La branche n'existe pas\n");
        }else{
            printBranch(argv[2]);
        }
        return 0;
    }
    // ./myGit checkout-branch <branch-name>
    if(strcmp(argv[1],"checkout-branch")==0){
        if(branchExists(argv[2])==0){
            printf("Erreur: La branche n'existe pas\n");
        }else{
            myGitCheckoutBranch(argv[2]);   
            printf("Changement de branche reussi !\nVous etes actuellement sur %s\n",argv[2]);
        }
        return 0;
    }
    // ./myGit checkout-commit <pattern>
    if(strcmp(argv[1],"checkout-commit")==0){
        myGitCheckoutCommit(argv[2]); 
        return 0;  
    }

    // ./myGit merge <branch> <message>
    if(strcmp(argv[1],"merge")==0){
        if(argc<4){
            printf("Veuillez indiquer la branche et le message\n");
            return 0;
        }
        List *conflicts=merge(argv[2],argv[3]); 
        if(conflicts==NULL){
            return 0;
        }

        printf("Il y'a des collision, choisisez une des options suivantes: \n");
        char *branch_tmp=getCurrentBranch();
        printf("1- Garder les fichiers de %s (branche courante)\n",branch_tmp);
        printf("2- Garder les fichiers de %s (remote branch)\n",argv[2]);
        printf("3- Choix manuel des fichiers/repertoires en conflits à garder sur chaque branche\n");
        int choix;
        scanf("%d",&choix);
        Cell *tmp;
        int num;
        List *conflicts_current;
        List *conflicts_remote;
        switch (choix)
        {
            case 1:
                createDeletionCommit(argv[2],conflicts,argv[3]);
                if(*conflicts==NULL){
                    printf("ici\n");
                    //On est ici dans le cas ou le commit de supression ne peut pas etre creer car touts ces fichiers/repertoires font partie de la liste de conflits
                    //On creer donc un nouveau commit ou le predecessor est le commit actuel,
                    //le merge_predecessor est le commit de la remote branch
                    char *hashcom=getRef(branch_tmp);
                    char *hashremote=getRef(argv[2]);
                    //Conversion de branche remote en commit (ftc)
                    char *hash_tmp=hashToPathCommit(hashcom);
                    Commit *res=ftc(hash_tmp);
                    free(hash_tmp);
                    //Recuperation du worktree associe
                    char *hashwt=commitGet(res,"tree");

                    Commit *final=createCommit(hashwt);
                    //Ajout des differentes cle dans le commit
                    commitSet(final,"predecessor",hashcom);
                    commitSet(final,"merged_predecessor",hashremote);
                    commitSet(final,"message",argv[3]);
                    //enregistrement instantanee du commit
                    char* hashc=blobCommit(final);
                    //maj refferences de la branche courante et HEAD
                    createUpdateRef(branch_tmp,hashc);
                    createUpdateRef("HEAD",hashc);
                    deleteRef(argv[2]);
                    printf("La fusion a ete realiser avec succes\n");
                    free(conflicts);
                    free(hashcom);
                    free(hashremote);
                    freeCommit(res);
                    free(hashwt);
                    freeCommit(final);
                    free(hashc);
                    break;
                }
                free_List(*conflicts);
                free(conflicts);
                conflicts=merge(argv[2],argv[3]);
                break;

            case 2:
                createDeletionCommit(branch_tmp,conflicts,argv[3]);
                if(*conflicts==NULL){
                    //On est ici dans le cas ou le commit de supression ne peut pas etre creer car touts ces fichiers/repertoires font partie de la liste de conflits
                    char *hashcom=getRef(branch_tmp);
                    char *hashremote=getRef(argv[2]);
                    //Conversion de branche remote en commit (ftc)
                    char *hashtopathc=hashToPathCommit(hashremote);
                    Commit *res=ftc(hashtopathc);
                    free(hashtopathc);
                    //Recuperation du worktree associe
                    char *hashwt=commitGet(res,"tree");

                    Commit *final=createCommit(hashwt);
                    char buffer[300];
                    char *htp=hashToPath(hashwt);
                    sprintf(buffer,"%s.t",htp);
                    free(htp);
                    free(hashwt);
                    WorkTree *wt=ftwt(buffer);
                    //Ajout des differentes cle dans le commit
                    commitSet(final,"predecessor",hashcom);
                    free(hashcom);
                    commitSet(final,"merged_predecessor",hashremote);
                    free(hashremote);
                    commitSet(final,"message",argv[3]);
                    //enregistrement instantanee du commit
                    char* hashc=blobCommit(final);
                    //maj refferences de la branche courante et HEAD
                    createUpdateRef(branch_tmp,hashc);
                    createUpdateRef("HEAD",hashc);
                    free(hashc);
                    deleteRef(argv[2]);
                    //Restauration du projet correspondant au WorkTree de fusion
                    restoreWorkTree(wt,".");
                    printf("La fusion a ete realiser avec succes\n");
                    free_wt(wt);
                    free(conflicts);
                    freeCommit(res);
                    freeCommit(final);
                    break;
                }
                free_List(*conflicts);
                free(conflicts);
                conflicts=merge(argv[2],argv[3]);
                break;
            
            case 3:
                conflicts_current=initList();
                conflicts_remote=initList();
                tmp=*conflicts;
                printf("Voici les fichier/repertoires en conflits:\n");
                while(tmp){
                    printf("-> %s\n",tmp->data);
                    tmp=tmp->next;
                }
                printf("---------------------------------\n");
                tmp=*conflicts;
                printf("Choisir 1 pour garder le fichier/repertoire dans %s (branche courrante) et 2 pour le garder dans %s (branche distante)\n",branch_tmp,argv[2]);
                while(tmp){
                    printf("-> %s\n",tmp->data);
                    scanf("%d",&num);
                    switch (num)
                    {
                        case 1:
                            insertFirst(conflicts_remote,buildCell(tmp->data));
                            break;
                        
                        case 2:
                            insertFirst(conflicts_current,buildCell(tmp->data));
                            break;

                        default:
                            printf("Numero incorecte\n");
                            free_List(*conflicts);
                            free(conflicts);
                            free_List(*conflicts_current);
                            free(conflicts_current);
                            free_List(*conflicts_remote);
                            free(conflicts_remote);
                            return 0;
                    }
                    tmp=tmp->next;
                }
                createDeletionCommit(branch_tmp,conflicts_current,argv[3]);
                createDeletionCommit(argv[2],conflicts_remote,argv[3]);
                free_List(*conflicts);
                free(conflicts);
                conflicts=merge(argv[2],argv[3]);
                free_List(*conflicts_current);
                free(conflicts_current);
                free_List(*conflicts_remote);
                free(conflicts_remote);
                break;
            
            default: 
                printf("Numero incorecte\n");
                free_List(*conflicts);
                free(conflicts);
        }
        free(branch_tmp);
        return 0;  
    }

    printf("La commande n'existe pas\n");
    return 0;
}