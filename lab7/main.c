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

// cerinta saptamana 7
// se actulizeaza functionalitatea prog a i sa primeasca oricate arg, dar nu mai mult de 10
// mentiune: niciun argument nu se va repeta
// programul proceseaza doar directoare, alte tipuri de argumente vor fi ignorate
// logica de captura a metadatele se va aplica la toate arg valide
// doar pe directoare
// in cazul care se dir se va modifica, utilizatorul va compara versiunea trecuta cu versiunea
// actuala
// daca exista diferenta, snapshotul vechi va fi modificat a i sa afiseze ultimele modificari
// functionalitatea cod extinsa ca sa primeasca un arg suplimentar, care va fo directorul de iesire
// in care vor fi stocate toate snapshoturile intrarile specificate in linia de comanda

// char out[1000] = "/mnt/d/Programare/SO/proiectSO/lab7/dirout";
// int cnt2=-1;

typedef struct
{
    char dir_name[289];
    char current_snapshot[289];
    char previous_snapshot[289];
} SnapshotInfo;

// SnapshotInfo snapshot_info[100]; // Array of SnapshotInfo for up to 100 directories

void list_directory(const char *path, int fileD)
{

    // char ioana[50] = "snapshot";

    // char fileName[289];

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
    // cnt2++;

    // sprintf(fileName, "%s/%s.%ld", dir_name, ioana, info.st_ino);

    // if((fileD = open(fileName, O_CREAT | O_WRONLY, S_IRUSR|S_IWUSR|S_IXUSR)) < 0)
    //     {
    //         perror("nu s a putut deschide fisierul");
    //         exit(-1);
    //     }

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

    // if(close(fileD) < 0){
    // perror("nu s a putut inchide fisierul");
    // exit(-1);
    // }

    if (closedir(d1) != 0)
    {
        perror("error at closing the directory");
        exit(-1);
    }
}

int compare_snapshots(const char *snapshot1, const char *snapshot2)
{
    // FILE *file1 = fopen(snapshot1, "r");
    // FILE *file2 = fopen(snapshot2, "r");  //deci sa deschid fisierele cu open

    // if(file1 == NULL || file2 == NULL)
    // {
    //     perror("Error opening file");
    //     return -1;
    // }

    int file1, file2;

    if ((file1 = open(snapshot1, O_RDONLY)) < 0)
    {
        perror("eroare deschidere fisiere");
        exit(-1);
    }

    if ((file2 = open(snapshot1, O_RDONLY)) < 0)
    {
        perror("eroare deschidere fisiere");
        exit(-1);
    }

    char line1[1024], line2[1024];

    while (fgets(line1, sizeof(line1), file1) != NULL && fgets(line2, sizeof(line2), file2) != NULL) // fac cu read
    {

        if (strcmp(line1, line2) != 0)
        {
            // The lines are different
            fclose(file1);
            fclose(file2); // close
            return 1;
        }
    }

    // Check if the files ended at the same time
    if (feof(file1) != feof(file2))
    {
        // One file ended before the other
        fclose(file1);
        fclose(file2);
        return 1;
    }

    fclose(file1);
    fclose(file2);
    return 0;
}

void save_snapshot_info(SnapshotInfo *snapshot_info, size_t size)
{
    FILE *file = fopen("snapshot_info.dat", "wb");
    if (file == NULL)
    {
        perror("Error opening file for writing");
        exit(-1);
    }
    if (fwrite(snapshot_info, sizeof(SnapshotInfo), size, file) != size)
    {
        perror("Error writing snapshot_info to file");
        exit(-1);
    } // trebuie schimbate si astea ca sa fie cu open si chestii

    fclose(file);
}

void load_snapshot_info(SnapshotInfo *snapshot_info, size_t size)
{
    FILE *file = fopen("snapshot_info.dat", "rb");
    if (file == NULL)
    {
        perror("Error opening file for reading");
        return;
    }
    if (fread(snapshot_info, sizeof(SnapshotInfo), size, file) != size)
    {
        perror("Error reading snapshot_info from file");
        exit(-1);
    }
    fclose(file);
}

int main(int argc, char **argv)
{

    SnapshotInfo snapshot_info[100];
    load_snapshot_info(snapshot_info, 100);

    // ./p dir0 dir1 .. dir9 -o dirout
    if (argc > 13)
    {
        perror("we have too much arguments\n");
        exit(-1);
    }

    // first, i have to find the out directory

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

        // aici creez fork ca sa mi faca mai multe procese

        // testari

        if (strcmp(argv[i], "-o") != 0)
        {
            // Find the SnapshotInfo for the current directory
            SnapshotInfo *info = NULL;
            for (int j = 0; j < 100; j++)
            {
                if (strcmp(snapshot_info[j].dir_name, argv[i]) == 0)
                {
                    info = &snapshot_info[j];
                    break;
                }
            }

            // If no SnapshotInfo was found for the current directory, create a new one
            if (info == NULL)
            {
                for (int j = 0; j < 100; j++)
                {
                    if (snapshot_info[j].dir_name[0] == '\0')
                    { // Empty dir_name means unused SnapshotInfo
                        info = &snapshot_info[j];
                        strcpy(info->dir_name, argv[i]);
                        break;
                    }
                }
            }

            // Save the name of the current snapshot
            strcpy(info->previous_snapshot, info->current_snapshot);

            // Generate the name of the new snapshot
            sprintf(info->current_snapshot, "%s/%s.%d", argv[out], "snapshot", i);

            int fileD = open(info->current_snapshot, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR | S_IXUSR);
            if (fileD < 0)
            {
                perror("nu s a putut deschide fisierul");
                exit(-1);
            }

            list_directory(argv[i], fileD); // pass the file descriptor to the function

            if (close(fileD) < 0)
            {
                perror("nu s a putut inchide fisierul");
                exit(-1);
            }

            // Compare the current snapshot with the previous one
            if (info->previous_snapshot[0] != '\0') // Don't compare if there is no previous snapshot
            {
                int comparison_result = compare_snapshots(info->previous_snapshot, info->current_snapshot);
                if (comparison_result == -1)
                {
                    printf("Error comparing snapshots.\n");
                }
                else if (comparison_result == 1)
                {
                    printf("The snapshots %s and %s are different.\n", info->previous_snapshot, info->current_snapshot);
                }
                else
                {
                    printf("The snapshots %s and %s are identical.\n", info->previous_snapshot, info->current_snapshot);
                }
            }
            else
            {
                printf("No previous snapshot to compare with for directory %s.\n", argv[i]);
            }
        }
        else
        {
            break;
        }
    }

    // aici o sa fac wait

    save_snapshot_info(snapshot_info, 100);

    return 0;
}