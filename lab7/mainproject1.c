#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<dirent.h>
#include<string.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

//cerinta saptamana 7
//se actulizeaza functionalitatea prog a i sa primeasca oricate arg, dar nu mai mult de 10
//mentiune: niciun argument nu se va repeta
//programul proceseaza doar directoare, alte tipuri de argumente vor fi ignorate
//logica de captura a metadatele se va aplica la toate arg valide
//doar pe directoare
//in cazul care se dir se va modifica, utilizatorul va compara versiunea trecuta cu versiunea
//actuala
//daca exista diferenta, snapshotul vechi va fi modificat a i sa afiseze ultimele modificari
//functionalitatea cod extinsa ca sa primeasca un arg suplimentar, care va fo directorul de iesire
//in care vor fi stocate toate snapshoturile intrarile specificate in linia de comanda


void list_directory(const char *path) {
    DIR *dir;
    struct dirent *entry;
    struct stat fileStat;
    char *filePath;
    int fileD;
    char *fileN = "snapshot.txt";

    if(lstat(path, &fileStat) < 0){
     
        perror("nu se poate face lstat here cause we have error");
        exit(-1);
    }

    if(S_ISDIR(path.st_mode) != 0)  //if the returning value is 0, we don t receive what we search for
    {
        fprintf(stdout, "we receive a directory");

    }
    else
    {
        fprintf(stdout, "we don t receive a directory TAICA");
        exit(-1);
        
    }

    if ((dir = opendir(path)) == NULL) {
        perror("nu se poate deschide directorul");
        exit(-1);
    }

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue; // skip . and ..

        sprintf(filePath, "%s/%s", path, entry->d_name); // create file path
        if (lstat(filePath, &fileStat) < 0) {
            perror("nu s a putut face stat");
            continue;
        }


// ...
    if((fileD = open(fileN, O_CREAT | O_WRONLY, S_IRUSR|S_IWUSR|S_IXUSR)) < 0){
        perror("nu s a putut deschide fisierul");
        exit(-1);
    }

        char buffer[1024];

        sprintf(buffer, "Nume: %s\n", filePath);  //la nume trebuie sa punem calea completa
        if(write(fileD, buffer, strlen(buffer))<0)
        {
            perror("nu s a putut scrie in fisier");
            exit(-1);   
        }

        sprintf(buffer, "Inode: %lu\n", fileStat.st_ino);
        if(write(fileD, buffer, strlen(buffer))<0)
        {
            perror("nu s a putut scrie in fisier");
            exit(-1);   
        }


        sprintf(buffer, "Dimensiune: %ld bytes\n", fileStat.st_size);
        if(write(fileD, buffer, strlen(buffer))<0)
        {
            perror("nu s a putut scrie in fisier");
            exit(-1);   
        }

        char mod_time[20];
        strftime(mod_time, 20, "%Y-%m-%d %H:%M:%S", localtime(&(fileStat.st_mtime)));
        sprintf(buffer, "Data ultimei modificari: %s\n", mod_time);
        if(write(fileD, buffer, strlen(buffer))<0)
        {
            perror("nu s a putut scrie in fisier");
            exit(-1);   
        }

        char *space = "\n";

        if(write(fileD, space, strlen(space))<0)
        {
            perror("nu s a putut scrie in fisier");
            exit(-1);   
        }


        if (S_ISDIR(fileStat.st_mode)) {
            list_directory(filePath); // recurse into subdirectory
        }
    }
    if(close(fileD) < 0){
    perror("nu s a putut inchide fisierul");
    exit(-1);
}


    closedir(dir);
}

//MAIN FUNCTION



int main(int argc, char **argv){

    if(argc > 12){
    fprintf(stderr, "we receive too much  arguments");   
    exit(-1);
    }

   
    //testare daca am primit chiar un fisier sau nu


    return 0;
}





