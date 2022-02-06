#include <stdio.h>
#include "openssl/md5.h"
#include <dirent.h>
#include <string.h>

int compute_md5Sum( char* file,char *c)
{
    char *filename=file;
    FILE *inFile = fopen (filename, "rb");
    MD5_CTX mdContext;
    int bytes;
    unsigned char data[1024];

    if (inFile == NULL) {
        printf ("%s can't be opened.\n", filename);
        return 0;
    }

    MD5_Init (&mdContext);
    while ((bytes = fread (data, 1, 1024, inFile)) != 0)
        MD5_Update (&mdContext, data, bytes);
    MD5_Final (c,&mdContext);
    fclose (inFile);
    return 0;
}

int main(int argc, char **argv)
{
    char * directoryName;
    DIR *directory;
    struct dirent *dir;
    int i;
    FILE *outFile;
    char filename[100] = {'\0'};
    unsigned char c[MD5_DIGEST_LENGTH];
    if(argc != 2)
    {
        printf("illegal parameters expected only 2");
        return 0;
    }

    directoryName = argv[1];
    printf("directory Name is %s\n",directoryName);
   
    directory = opendir(directoryName);
    if (directory)
    {
        strcat(filename,directoryName);
        strcat(filename,"out.md5");
        outFile = fopen (filename, "w");
        while ((dir = readdir(directory)) != NULL)
        {
            printf("%s\n", dir->d_name);
            if(strstr(dir->d_name,"txt"))
            {
                char file[100] = {'\0'};
                strcat(file,directoryName);
                strcat(file,dir->d_name);
                compute_md5Sum(file,&c[0]);

                //printf ("%s: ", file);
                fprintf(outFile,"%s: ", file);
                for(i = 0; i < MD5_DIGEST_LENGTH; i++)
                {
                    printf("%02x", c[i]);
                    fprintf(outFile,"%02x", c[i]);
                }
                fprintf(outFile,"\n");
            }
        }
        closedir(directory);
        fclose(outFile);
    }
    else
    {
        printf("no directpry present\n");
    }

    return 1;
}
