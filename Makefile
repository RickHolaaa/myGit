#CFLAGS = -g -Wall -Wextra -pedantic -Wno-unused-parameter
CFLAGS = -g -Wno-unused-parameter
CC = gcc

PROGRAMS = test_liste test_manipbase test_gitfunctions myGit

.PHONY:	all clean

all: $(PROGRAMS)

test_manipbase : ManipBase.o mainManipBase.o FonctionHash.o listCell.o
	$(CC) -o $@ $(CFLAGS) $^

test_liste : listCell.o mainList.o FonctionHash.o
	$(CC) -o $@ $(CFLAGS) $^

test_gitfunctions : commit.o mainGitFunctions.o FonctionHash.o listCell.o ManipBase.o merge.o checkout.o branchutils.o
	$(CC) -o $@ $(CFLAGS) $^

myGit : myGit.o FonctionHash.o listCell.o commit.o ManipBase.o merge.o checkout.o branchutils.o
	$(CC) -o $@ $(CFLAGS) $^

myGit.o : myGit.c
	gcc $(CFLAGS) -c myGit.c

listCell.o: listCell.c
	gcc $(CFLAGS) -c listCell.c

mainList.o: mainList.c
	gcc $(CFLAGS) -c mainList.c

ManipBase.o: ManipBase.c
	gcc $(CFLAGS) -c ManipBase.c

mainManipBase.o: mainManipBase.c
	gcc $(CFLAGS) -c mainManipBase.c

FonctionHash.o: FonctionHash.c
	gcc $(CFLAGS) -c FonctionHash.c

commit.o : commit.c 
	gcc $(CFLAGS) -c commit.c


mainGitFunctions.o : mainGitFunctions.c
	gcc $(CFLAGS) -c mainGitFunctions.c

merge.o: merge.c
	gcc $(CFLAGS) -c merge.c

checkout.o: checkout.c
	gcc $(CFLAGS) -c checkout.c

branchutils.o: branchutils.c
	gcc $(CFLAGS) -c branchutils.c


#Ou plus simplement
#%.o:%.c %.h
#	$(CC)  $(GCC_FLAGS) -c  $<


clean:
	rm -f *.o *~ $(PROGRAMS)
