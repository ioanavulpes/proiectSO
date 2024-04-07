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


void list_directory(const char *path){

    char out[1000] = "/mnt/d/Programare/SO/proiectSO/lab7/dirout";

    char ioana[100] = "snapshot";

    char fileName[289];

    //i have to open and test the directory
    struct stat info;

    int fileD;

    if(lstat(path, &info) < 0){
        perror("the function lstat doesn t work 1\n");
        exit(-1);
    }   

    //S_ISDIR = 1 -> true, adica avem director

    if(S_ISDIR(info.st_mode)){
        fprintf(stdout, "we receive a directory\n");
    }
    else
    {
        if(S_ISREG(info.st_mode))
        {
        fprintf(stdout,"we don t receive a directory, but is a REGULAR FILE\n");
        exit(-1);   
        }
    }

    //if is a directory, we have to open it

    DIR *d1;
    
    if((d1 = opendir(path)) == NULL){
        perror("we can t open the directory");
        exit(-1);   
    }

    //if can open it, we read from it

    struct dirent *dir;
    int cnt1=0;
    char filePath[100];

    char buffer[1024];
    int cnt2=0;
    sprintf(fileName, "%s/%s.%d", out, ioana, cnt2);

    if((fileD = open(fileName, O_CREAT | O_WRONLY, S_IRUSR|S_IWUSR|S_IXUSR)) < 0)
        {
            perror("nu s a putut deschide fisierul");
            exit(-1);
        }

    while((dir = readdir(d1)) != NULL){
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        } 

        sprintf(filePath, "%s/%s", path, dir->d_name);

        //fac stat pe ce am primit

        if(lstat(filePath, &info) < 0){
            perror("the function lstat doesn t work 2\n");
            exit(-1);
        }

        cnt2++;  

         if(write(fileD, filePath, strlen(filePath)) < 0){

            perror("error at writing the name of the file\n");    //we are writing the name of the directory or file
            exit(-1);  
         }

         //try to write the inode number
         sprintf(buffer, "\nInode: %lu\n", info.st_ino);
         if(write(fileD, buffer, strlen(buffer)) < 0){
            perror("error at writing the number of the inode\n");
            exit(-1);  
         }

         //we have to write the dim in bytes

         sprintf(buffer, "Number of bytes: %lu\n", info.st_size);
         if(write(fileD, buffer, strlen(buffer)) < 0){
            perror("error at writing of the number of the byte's file");
            exit(-1);  
         }

         //the last date when you modified the file

        char mod_time[20];
        strftime(mod_time, 20, "%Y-%m-%d %H:%M:%S", localtime(&(info.st_mtime)));
        sprintf(buffer, "Data ultimei modificari: %s\n", mod_time);

        if(write(fileD, buffer, strlen(buffer)) < 0){
            perror("error at last date when you modified the file\n");
            exit(-1);   
        }

        char *space = "\n";

        if(write(fileD, space, strlen(space))<0)
        {
            perror("nu s a putut scrie in fisier");
            exit(-1);   
        }


         if (S_ISDIR(info.st_mode)) 
            list_directory(filePath); // recurse into subdirectory
        
    }
    if(close(fileD) < 0){
    perror("nu s a putut inchide fisierul");
    exit(-1);
    }


    if(closedir(d1) != 0)
    {
        perror("error at closing the directory");
        exit(-1);   
    }

}


int main(int argc, char **argv){


// ./p dir0 dir1 .. dir9 -o dirout 
    if(argc > 13){
        perror("we have too much arguments\n");
        exit(-1);
    }   

   //first, i have to find the out directory

    int out=-1;
   for(int i=0; i<argc; i++){
    if(strcmp(argv[i], "-o") == 0){
        fprintf(stdout, "we find the out directoty\n");
        out = i+1;
    }
   }

    if(out > 0 && out < argc) {
        fprintf(stdout, "the directory that is our OUTPUT is named %s\n", argv[out]);
    } else {
        fprintf(stderr, "-o not found or does not have a value\n");
    }

    for(int i=1; i<argc; i++){
        if(strcmp(argv[i], "-o") != 0){
            list_directory(argv[i]);
        }   
        else
        {
            break;
        }
    }



    return 0;
}