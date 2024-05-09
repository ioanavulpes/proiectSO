#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>

#define BUFFER 1024

typedef struct
{
    char actualsnap[30];
    char previoussnap[30];
} SnapInfo;

// cum fac sa am comparare de snapshot

// pasii in fct de compare

// o fac int daca da 1 sunt diferite
// 0 identice

void copiere_snap(const char *file1, const char *file2)
{

    int fd1, fd2;
    ssize_t bytes_read1, bytes_write2;
    char buf1[BUFFER];
    char buf2[BUFFER];

    if ((fd1 = open(file1, O_RDONLY)) < 0)
    {
        perror("eroare deschidere fisier1");
        exit(-1);
    }
    if ((fd2 = open(file1, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) < 0)
    {
        perror("eroare deschidere fisier2");
        exit(-1);
    }

    while ((bytes_read1 = read(fd1, buf1, BUFFER)) > 0)
    {
        if ((bytes_write2 = write(fd2, buf1, BUFFER)) != bytes_read1)
        {
            perror("nu se poate realiza scrierea in fisierul 2");
            exit(-1);
        }
    }

    if (bytes_read1 < 0)
    {
        perror("Eroare la citirea fișierului1");
        exit(EXIT_FAILURE);
    }

    // se inchid fisierele
    if (close(fd1) < 0)
    {
        perror("nu se poate realiza inchiderea pentru fis1");
        exit(-1);
    }

    if (close(fd2) < 0)
    {
        perror("nu se poate realiza inchiderea pentru fis2");
        exit(-1);
    }
}

int compare_snaps(const char *snap1, const char *snap2)
{

    // strcpy(snap2, snap1);
    //  prima data trebuie deschise ambele
    //  le deschid doar pentru citire pentru ca deja le am create cu continut

    int fd1, fd2;

    if ((fd1 = open(snap1, O_RDONLY)) < 0)
    {
        perror("eroare deschide primu snap1");
        exit(EXIT_FAILURE);
    }

    if ((fd2 = open(snap2, O_RDONLY)) < 0)
    {
        perror("eroare deschide primu snap1");
        exit(EXIT_FAILURE);
    }

    ssize_t bytes_read1, bytes_read2;
    char buf1[BUFFER];
    char buf2[BUFFER];

    while ((bytes_read1 = read(fd1, buf1, BUFFER)) > 0 && (bytes_read2 = read(fd2, buf2, BUFFER)) > 0)
    {
        if ((bytes_read1 != bytes_read2) || memcmp(buf1, buf2, bytes_read1) != 0)
        {
            perror("nu au acelasi continut ...1");
            if (close(fd1) < 0)
            {
                perror("eroare inchidere fisier 1");
                // return 1; //diferite
            }
            if (close(fd2) < 0)
            {
                perror("eroare inchidere fisier 1");
                // return 1;
            }

            return 1;
        }

        // verificare daca am citit tot din fisier
        // read da return 0 daca s a ajuns la capat de fisier
    }

    if ((bytes_read1 = read(fd1, buf1, BUFFER)) != (bytes_read2 = read(fd2, buf2, BUFFER)))
    {
        return 1;
    }

    if (close(fd1) < 0)
    {
        perror("eroare inchidere fisier 1");
    }
    if (close(fd2) < 0)
    {
        perror("eroare inchidere fisier 2");
    }

    return 0;
}

void list_directory(const char *path, int fileD)
{

    // i have to open and test the directory
    struct stat info;

    // int fileD;

    if (lstat(path, &info) < 0)
    {
        perror("the function lstat doesn t work 1\n");
        exit(-1);
    }

    // S_ISDIR = 1 -> true, adica avem director

    if (S_ISDIR(info.st_mode))
    {
        fprintf(stdout, "we receive a directory\n");
    }
    else
    {
        if (S_ISREG(info.st_mode))
        {
            fprintf(stdout, "we don t receive a directory, but is a REGULAR FILE\n");
            exit(-1);
        }
    }

    // if is a directory, we have to open it

    DIR *d1;

    if ((d1 = opendir(path)) == NULL)
    {
        perror("we can t open the directory");
        exit(-1);
    }

    // if can open it, we read from it

    struct dirent *dir;

    char filePath[100];

    char buffer[1024];

    while ((dir = readdir(d1)) != NULL)
    {

        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
        {
            continue;
        }

        sprintf(filePath, "%s/%s", path, dir->d_name);

        // fac stat pe ce am primit

        if (lstat(filePath, &info) < 0)
        {
            perror("the function lstat doesn t work 2\n");
            exit(-1);
        }

        if (write(fileD, filePath, strlen(filePath)) < 0)
        {

            perror("error at writing the name of the file\n"); // we are writing the name of the directory or file
            exit(-1);
        }

        // try to write the inode number
        sprintf(buffer, "\nInode: %lu\n", info.st_ino);
        if (write(fileD, buffer, strlen(buffer)) < 0)
        {
            perror("error at writing the number of the inode\n");
            exit(-1);
        }

        // we have to write the dim in bytes

        sprintf(buffer, "Number of bytes: %lu\n", info.st_size);
        if (write(fileD, buffer, strlen(buffer)) < 0)
        {
            perror("error at writing of the number of the byte's file");
            exit(-1);
        }

        // the last date when you modified the file

        char mod_time[20];
        strftime(mod_time, 20, "%Y-%m-%d %H:%M:%S", localtime(&(info.st_mtime)));
        sprintf(buffer, "Data ultimei modificari: %s\n", mod_time);

        if (write(fileD, buffer, strlen(buffer)) < 0)
        {
            perror("error at last date when you modified the file\n");
            exit(-1);
        }

        char *space = "\n";

        if (write(fileD, space, strlen(space)) < 0)
        {
            perror("nu s a putut scrie in fisier");
            exit(-1);
        }

        if (S_ISDIR(info.st_mode))
            list_directory(filePath, fileD); // recurse into subdirectory
    }

    if (closedir(d1) != 0)
    {
        perror("error at closing the directory");
        exit(-1);
    }
}

int main(int argc, char **argv)
{
    SnapInfo snaps[30];

    for (int i = 0; i < argc; i++)
    {

        if (strcmp(argv[i], "-o") != 0)
        {
            strcpy(snaps[i].actualsnap, "");
            strcpy(snaps[i].previoussnap, "");
        }
    }

    // ./p dir0 dir1 .. dir9 -o dirout
    if (argc > 13)
    {
        perror("we have too much arguments\n");
        exit(-1);
    }

    //     //first, i have to find the out directory

    int out = -1;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-o") == 0)
        {
            fprintf(stdout, "we find the out directoty\n");
            out = i + 1;
        }
    }

    if (out > 0 && out < argc)
    {
        fprintf(stdout, "the directory that is our OUTPUT is named %s\n", argv[out]);
    }
    else
    {
        fprintf(stderr, "-o not found or does not have a value\n");
    }

    for (int i = 1; i < argc; i++)
    {
        for (int j = i + 1; j < argc; j++)
        {
            if (strcmp(argv[i], argv[j]) == 0)
            {
                perror("NU ai dat bine argumnetele, EXISTA argumente ce se repeta\n");
                exit(-1);
            }
            else
            {
                continue;
            }
        }

        if (strcmp(argv[i], "-o") != 0)
        {
            // if (snaps[i].actualsnap[0] != '\0')
            // {
            // printf("se face copierea\n");
            // strcpy(snaps[i].previoussnap, snaps[i].actualsnap);
            // copiere_snap(snaps[i].actualsnap, snaps[i].previoussnap);
            //}
            sprintf(snaps[i].actualsnap, "%s/%s.%d", argv[out], "snapshot", i); // aici pun argv[out] ca sa stie unde sa puna fisierele

            int fileD = open(snaps[i].actualsnap, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR | S_IXUSR);
            if (fileD < 0)
            {
                perror("nu s a putut deschide fisierul\n");
                exit(-1);
            }

            list_directory(argv[i], fileD); // pass the file descriptor to the function

            if (close(fileD) < 0)
            {
                perror("nu s a putut inchide fisierul\n");
                exit(-1);
            }

            if (snaps[i].previoussnap[0] != '\0')
            {
                copiere_snap(snaps[i].actualsnap, snaps[i].previoussnap);

                printf("aici se face compararea\n");
                if (compare_snaps(snaps[i].actualsnap, snaps[i].previoussnap) == 0)
                {
                    fprintf(stdout, "SNAPSHOT uri identice\n");
                }
                else
                {
                    fprintf(stdout, "snapshot urile NU sunt identice\n");
                }

                printf("se face copierea\n");
                strcpy(snaps[i].previoussnap, snaps[i].actualsnap);

                // }
            }
            else
            {
                break;
            }
        }
    }

        return 0;
}