//util va putea specifica dir de monotirizat ac arg in linie de comanda . iar prog va urmari schimbarile
//care apar in acesta si in subdirectoarele sale
//la fiecare rulare a prog se va actualiza captura/ snapshotu
//dir stocand datele fiecarei intrari

//salvat in snapshot nume, nr inodu, data ultimei modif si dim fisierului


#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<dirent.h>


int main(int argc, char **argv){

    if(argc != 2){
     
    fprintf(stderr, "vezi ca n ai dat destule arg vericule\n");   
    exit(-1);
    }

    DIR *directory;

    if((directory = opendir(argv[1])) == NULL){
        perror("nu se poate deschide directorul");
        exit(-1);
    }

    //testare daca am primit chiar un fisier sau nu

    struct stat fileTest;

    if(lstat(argv[1], &fileTest) < 0){
        perror("nu s a putut face lstat");
        exit(-1);
    }

    if(S_ISDIR(fileTest.st_mode) == 0){  
    //if we have a returning value of 0, we don t have the file 
    //that we search for
        fprintf(stdout, "we do NOT have a DIRECTORY\n");
        if(S_ISREG(fileTest.st_mode) != 0){
            fprintf(stdout, "we have a REGULAR FILE\n");
        }
        else{
            fprintf(stdout, "we have a LINK\n");
           
        }
        exit(-1);
    } 
    else
    {
        fprintf(stdout, "we HAVE a directory\n");  
    }




    return 0;
}