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

    //we have a directory, we can start the monitoring

    //we need to create a snapshot of the directory

    //we need to create a file where we will store the snapshot

   FILE *file = fopen("snapshot.txt", "wr");

   if(file == NULL){
       perror("nu s a putut deschide fisierul snapshot");
       exit(-1);
   }

    //we need to write the name of the directory in the snapshot

    // if(write(fileSnapshot, argv[1], strlen(argv[1])) < 0){
    //     perror("nu s a putut scrie in fisierul snapshot");
    //     exit(-1);
    // }

    fprintf(file, "we have the MAIN DIRECTORY ");
    fprintf(file, "%s", argv[1]);

    fprintf(file , "\n");



    //code for writing the inode number of the directory in the snapshot

    // if(write(fileSnapshot, &fileTest.st_ino, sizeof(fileTest.st_ino)) < 0){
    //     perror("nu s a putut scrie in fisierul snapshot");
    //     exit(-1);
    // }

    fprintf(file, "we have the inode number of the directory ");
    fprintf(file, "%lu", (unsigned long)fileTest.st_ino);
    fprintf(file , "\n");
    //code for writing the last modification date of the directory in the snapshot


    // if(write(fileSnapshot, &fileTest.st_mtime, sizeof(fileTest.st_mtime)) < 0){
    //     perror("nu s a putut scrie in fisierul snapshot");
    //     exit(-1);
    // }

    // fprintf(file, "we have the last modification date of the directory ");
    // fprintf(file, "%lu", fileTest.st_mtime);
    // fprintf(file , "\n");

     time_t modification_time = fileTest.st_mtime;

    // Convert modification time to a human-readable format
    char time_buffer[80];
    strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", localtime(&modification_time));

    // Print the modification time using fprintf
    fprintf(file, "Modification time of %s\n", time_buffer);
    fprintf(file , "\n");

    //code for writing the size of the directory in the snapshot

    // if(write(fileSnapshot, &fileTest.st_size, sizeof(fileTest.st_size)) < 0){
    //     perror("nu s a putut scrie in fisierul snapshot");
    //     exit(-1);
    // }
    
    fprintf(file, "we have the size of the directory ");
    fprintf(file, "%lu", fileTest.st_size);
    fprintf(file , "\n");

    // if(fclose(file) < 0){
    //     perror("nu s a putut inchide fisierul snapshot");
    //     exit(-1);
    // }
    //we need to close the file

    // Open the file

// if((fileSnapshot = open(file, O_RDWR)) < 0){
//         perror("nu s a putut deschide fisierul snapshot");
//         exit(-1);
//     }
// // Variables to store the data
// char name[100];
// ino_t inode;
// time_t mtime;
// off_t size;

// // Read and print the name
// read(fileSnapshot, name, sizeof(name));
// printf("Directory: %s\n", name);

// // Read and print the inode number
// read(fileSnapshot, &inode, sizeof(inode));
// printf("Inode number: %lu\n", inode);

// // Read and print the last modification date
// read(fileSnapshot, &mtime, sizeof(mtime));
// char timebuf[26];
// ctime_r(&mtime, timebuf);
// printf("Last modification date: %s", timebuf);

// // Read and print the size
// read(fileSnapshot, &size, sizeof(size));
// printf("Size: %ld bytes\n", size);

// Close the file

if(fclose(file) < 0){ 

    perror("nu s a putut inchide fisierul snapshot");
    exit(-1);
}


    if(closedir(directory) < 0){
        perror("nu s a putut inchide directorul");
        exit(-1);
    }











    return 0;
}