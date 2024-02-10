#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "ManipBase.h"



int main()
{
    WorkFile *wf1=createWorkFile("test");
    char *c1=wfts(wf1);
    printf("%s\n",c1);
    free(c1);

    char *c2=wfts(wf1);
    WorkFile *wf2=stwf(c2);
    free(c2);

    free_wf(*wf1);
    free(wf1);

    c2=wfts(wf2);
    printf("%s\n",c2);

    free_wf(*wf2);
    free(wf2);
    free(c2);

    WorkTree *wt1=initWorkTree();
    printf("%d\n",inWorkTree(wt1,"test"));
    
    printf("%d\n",appendWorkTree(wt1,"fichier_test/file.txt",NULL,0));
    printf("%d\n",appendWorkTree(wt1,"fichier_test/file1.txt",NULL,0));
    printf("%d\n",appendWorkTree(wt1,"fichier_test/file2.txt",NULL,0));
    printf("%d\n",appendWorkTree(wt1,"fichier_test/dir1",NULL,0));
    printf("%d\n",inWorkTree(wt1,"fichier_test/test.tmp"));

    c2=wtts(wt1);
    printf("%s",c2);
    free(c2);
    printf("%d\n",wttf(wt1,"/tmp/file_test.txt"));

    
    WorkTree *wt2=ftwt("/tmp/file_test.txt");
    if(wt2->tab[inWorkTree(wt2,"fichier_test/file.txt")].hash==NULL)
    {
        printf("bon\n");
    }
    printf("------------------\n");
    c2=wtts(wt2);
    printf("test\n%s",c2);
    free(c2);
    printf("----------------------\n");
    //Recuperation du repertoire courrant afin d'eviter tout conflit lors des tests car notre worktree se situe dans le repertoire courant
    char buffer[1026];
    getcwd(buffer, sizeof(buffer));
    char *current_path=strdup(buffer);
    char *swt=saveWorkTree(wt1,current_path);
    printf("%s\n",swt);
    free(swt);


    //Test du bon fonctionnement de restoreWorkTree
    /*
    sprintf(buffer,"echo reussi>file.txt");
    system(buffer);
    sprintf(buffer,"echo reussi vrai>0d/78d8aaeae143f9b7ff92fa3706df38c0b52320fc26ab7cc01aafecc4a56553");
    system(buffer);
    */

    restoreWorkTree(wt1,current_path);
    free_wt(wt1);
    free_wt(wt2);
    free(current_path);
    return 0;
}

