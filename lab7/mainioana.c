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

    //we consider that we receive an argument from command line and
    //we don t know if is a file or a directory
    //we have to check that

    struct stat info;  // one structure of the type stat char *fileN = "snapshot.txt";
    char *fileN = "snapshot.txt";
    int fileD;
    

    //we will use the function lstat that is used for any type of file that it is not a link

    //the lstat function will write into a structure the type stat that it has to be allocated
    //and to be existed

    //lstat is a call system that gives us the power of knowing information about the file that are the argument

    if(lstat(path, &info) < 0) //if the returning value of lstat is -1, we have an error
    {
            fprintf(stderr, "the lstat function does not function\n");
            exit(-1);
    }
    
    //here, we have access at information 
    //st_mode -> give us the file type

    if(S_ISDIR(info.st_mode)){
        fprintf(stdin, "we receive a directory\n");
    }
    else
    {
        fprintf(stdin, "we receive a regular file\n");
        exit(-1);   
    }

    //if we find the directory, we will open it
    //in error case , the function opendir will return NULL
    //The function returns a pointer to the DIR type
    
    struct dirent *dir;
    DIR *d1;

    //we try to open the directory that we received it as an argument at our function
    //retuning value is a pointer of type DIR


    if((d1 = opendir(path)) == NULL){
        fprintf(stderr, "we can t open the directory\n");   
        exit(-1);
    }

    //readdir is used for reading from a directory
    //returning value of readdir is a pointer to a dirent structure
    //it takes as argument a pointer to a DIR structure that is obtained from the using of the function opendir

    //returning value in an error case is NULL

    while((dir = readdir(d1)) !=  NULL){
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)  //we have to test this because
                                                                            //in the current directory always we will
                                                                            //have these 2 directories . and ..
        continue; //that means that we have to get over this files

        //if we don t receive . or .. we have check another files or directories

        //we have to create the path of the file to know where is from


        //we have to open the snapshot

        //open the file that we used as a snapshot
        //  char *fileN = "snapshot.txt";
        //  int fileD;
        if((fileD = open(fileN, O_CREAT | O_WRONLY, S_IRUSR|S_IWUSR|S_IXUSR)) < 0){
            fprintf(stderr, "error when we try to open the snapshot");
            exit(-1);   
        }

        // char filePath[1024];

        // sprintf(filePath, "%s/%s", path, dir->d_name); //to creat the path of the file
        char filePath[1018];
        sprintf(filePath, "nume: %s/%s", path, dir->d_name);

        //we call the lstat to find details about each file

        if(lstat(filePath, &info)){
            perror("the lstat function doesn't function");
            exit(-1);
        }

        char buffer[1024];
       // sprintf(buffer, "nume %s\n", filePath);
        snprintf(buffer, sizeof(buffer), "nume %s\n", filePath);
        //scriem numele in snapshot
        //write returns -1 in the error case
        if(write(fileD, buffer, strlen(buffer)) < 0){
            perror("error at the function write");
            exit(-1);   
        }


            


    }






    


    //close the snapshot
    //returning -1 in the error case
    //retuning 0 in the succes case

    if(close(fileD)<0){
        fprintf(stderr, "error at the closing of the snapshot");
        exit(-1);
    }

    //close the directory
    //on succes, the returning value is 0

    if(closedir(d1)){
        perror("error at the closing of directory");
        exit(-1);   
    }
}


    

//MAIN FUNCTION



int main(int argc, char **argv){

    if(argc != 2){
    fprintf(stderr, "we receive too much  arguments");   
    exit(-1);
    }


    list_directory(argv[1]);


   
    //testare daca am primit chiar un fisier sau nu


    return 0;
}





