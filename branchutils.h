#ifndef BRANCHUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


void initBranch();
int branchExists(char* branch);
void createBranch(char* branch);
char* getCurrentBranch();
void printBranch(char* branch);
List* branchList(char* branch);
List* getAllCommits();

#endif