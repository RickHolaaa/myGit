#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "FonctionHash.h"

/*
Question 1.2 :
sha256num <main.c >file.tmp
*/

int hashFile(char* source, char* dest){
    char res[256];
    sprintf(res,"sha256sum <%s >%s",source,dest);
    
    return system(res);
}

char* sha256sum(char* file){
    static char template[] = "/tmp/myfileXXXXXX";
    char fname[100];
    strcpy(fname,template);
    int fd=mkstemp(fname);
    
    // Formatage de buffer
    char buffer[256];
    sprintf(buffer,"[ -e %s ] && sha256sum <%s |cut -d\" \" -f 1 >%s",file,file,fname);

    // Lancement de la commande bash
    if(system(buffer)!=0)//Dans le cas ou le fichier n'existe pas
    {
        printf("Erreur :%s n'existe pas\n",file);
        return NULL;
    }

    // Lecture de ftmp afin de récupérer la chaîne de caractère contenant le hash du fichier donnée en paramètre
    char *hash;
    FILE *ftmp=fopen(fname,"r");
    fgets(buffer,256,ftmp);
    buffer[strlen(buffer)-1]='\0';
    hash=strdup(buffer);
    fclose(ftmp);

    // Suppression du fichier temporaire
    sprintf(buffer,"rm %s",fname);
    system(buffer);
    
    return hash;
}
