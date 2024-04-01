#include<stdio.h>
#include<stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<dirent.h>
#include<string.h>
#include <unistd.h>
#include <time.h>

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

    //if we have a directory, we can start the monitoring
    //we need to create a snapshot of the directory

    //we need to create a file where we will store the snapshot

    int fileDescriptor;
    char *fileName = "snapshot.txt";

    if((fileDescriptor = open(fileName, O_CREAT | O_WRONLY, S_IRUSR|S_IWUSR|S_IXUSR)) < 0){
        perror("nu s a putut deschide fisierul");
        exit(-1);
    }

    //i have to write information about the directory in the file

    //we need to write de name of the directory in the file

    char *nume = argv[1];

    if((write(fileDescriptor, nume, strlen(nume))) < 0){
        perror("nu s a putut scrie in fisier");
        exit(-1);
    }

char *buf = "\nnr inod\n";
if((write(fileDescriptor, buf, strlen(buf))) < 0){
    perror("nu s a putut scrie in fisier");
    exit(-1);
}

ino_t  nrInod = fileTest.st_ino;

// Buffer to hold the inode number as a string
char inoStr[20];

// Convert the inode number to a string
sprintf(inoStr, "%lu", nrInod);

// Write the inode number string to the file
if((write(fileDescriptor, inoStr, strlen(inoStr))) < 0){
    perror("nu s a putut scrie in fisier");
    exit(-1);
}
    char *buf1 = "\ndata ultimei modificari\n";
        if((write(fileDescriptor, buf1, strlen(buf1))) < 0){
            perror("nu s a putut scrie in fisier");
            exit(-1);
        }

    time_t data = fileTest.st_mtime;

   struct tm *timeinfo = localtime(&data);

// Buffer to hold the formatted time
char timebuf[80];

// Format the time
strftime(timebuf, 80, "%Y-%m-%d %H:%M:%S", timeinfo);

// Write the formatted time to the file
if((write(fileDescriptor, timebuf, strlen(timebuf))) < 0){
    perror("nu s a putut scrie in fisier");
    exit(-1);
}

char *buf2 = "\ndimensiune in bytes a fisierului\n";

if((write(fileDescriptor, buf2, strlen(buf2))) < 0){
    perror("nu s a putut scrie in fisier");
    exit(-1);
}

// Buffer to hold the size of the file as a string
char sizeStr[20];

// Convert the size of the file to a string

sprintf(sizeStr, "%lu", fileTest.st_size);

// Write the size of the file to the file

if((write(fileDescriptor, sizeStr, strlen(sizeStr))) < 0){
    perror("nu s a putut scrie in fisier");
    exit(-1);
}

if(close(fileDescriptor) < 0){
    perror("nu s a putut inchide fisierul");
    exit(-1);
}

//acuma trebuie sa inchidem directorul

if(closedir(directory) < 0){    
    perror("nu s a putut inchide directorul");
    exit(-1);
}


    return 0;
}


