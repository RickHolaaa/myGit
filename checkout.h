#ifndef CHECKOUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void restoreCommit(char *hash_commit);
void myGitCheckoutBranch(char* branch);
List* filterList(List* L, char* pattern);
void myGitCheckoutCommit(char* pattern);

#endif