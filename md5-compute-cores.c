#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include "openssl/md5.h"
#include <dirent.h>
#include <time.h>
#include <string.h>
#include "md5-compute-cores.h"
#include <pthread.h>
#define MAX_CORES 32
char * directoryName;
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

void md5Thread(void* args)
{
    threadargs *thread = (threadargs*)args;

    unsigned char c[MD5_DIGEST_LENGTH];
    int k = 0,i;
    FILE *outFile;
    char temp[1000] = { '\0'};
    

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(thread->cpuCore, &cpuset);

    pthread_t current_thread = pthread_self();    
    //printf("In Pthrread core is %d  id %d\n",thread->cpuCore,current_thread);
    pthread_setaffinity_np(current_thread, sizeof(cpu_set_t), &cpuset);
    char filename[100] = {'\0'};

    strcat(filename,directoryName);
    sprintf(temp, "%d", thread->cpuCore);
    strcat(filename,temp);
    strcat(filename,"_out.md5");

    outFile = fopen (filename, "w");

    for(k = thread->startFileNo ; k<= thread->EndFileNo; k++)
    {
        char temp2[100] = {'\0'};
        char file[100] = {'\0'};
        strcat(file,directoryName);
        sprintf(temp2, "%d", k);
        strcat(file,temp2);
        strcat(file,".txt");
        compute_md5Sum(file,&c[0]);

        //printf ("%s: ", file);
        fprintf(outFile,"%s: ", file);
        for(i = 0; i < MD5_DIGEST_LENGTH; i++)
        {
            //printf("%02x", c[i]);
            fprintf(outFile,"%02x", c[i]);
        }
        //printf("File is %s\n",file);
        fprintf(outFile,"\n");
    }
    fclose(outFile);
    return;

}
int main(int argc, char **argv)
{
    char* cpuCores;
    DIR *directory;
    struct dirent *dir;
    int i;
    FILE *outFile,*outfile2;
    char filename[100] = {'\0'};
    unsigned char c[MD5_DIGEST_LENGTH];
    int coreArray[MAX_CORES] = { -1};
    char* core = NULL;
    int j = 0; 
    int numFiles = 10002;


    struct timeval start, end;

    gettimeofday(&start, NULL);
    if(argc != 3)
    {
        printf("illegal parameters expected only 2");
        return 0;
    }

    directoryName = argv[1];
    cpuCores = argv[2];
    printf("directory Name is %s\n",directoryName);
    printf("CPU cores is %s\n",cpuCores);

    core = strtok (cpuCores,"-");
    while (core != NULL)
    {
        coreArray[j] = atoi(core);
        printf ("%d\n",coreArray[j]);
        j++;
        core = strtok (NULL, "-");
    }


    printf("j is %d\n",j);
    printf("Need to create thread and parrellise it\n");

    int k;
    int startFiles = 1;
    int scaleFactor = numFiles/j;
    pthread_t thread[j];
    for (k = 0 ; k < j ; k++)
    {
        threadargs *args = (threadargs*)malloc(sizeof(threadargs));
        args->cpuCore = coreArray[k];
        args->startFileNo = startFiles;

        if(k == (j-1))
        {
            args->EndFileNo = numFiles;
        }
        else
        {
            args->EndFileNo = (startFiles - 1) + scaleFactor;
        }
        startFiles = startFiles + scaleFactor;

        //printf("Start Files %d End Files %d",args->startFileNo,args->EndFileNo);
        pthread_create(&thread[k],NULL,md5Thread,(void*)args);

    }

    for (k = 0; k < j; k++) {
        pthread_join(thread[k], NULL);
    }

    strcat(filename,directoryName);
    strcat(filename,"out.md5");
    outFile = fopen (filename, "w");

    for( k = 0 ; k < j ; k++)
    {
        char buf[1000] = { '\0'};
        char file[100] = {'\0'};
        char temp2[100] = {'\0'};
        strcat(file,directoryName);
        sprintf(temp2, "%d", coreArray[k]);
        strcat(file,temp2);
        strcat(file,"_out.md5");

        //printf("filename is %s\n",file);
        outfile2 = fopen (file, "r");

        while (fgets(buf, sizeof(buf), outfile2) != NULL) {
            //printf("content is %s\n",buf);
            fprintf(outFile, "%s", buf);
        }
    }


    gettimeofday(&end, NULL);
 
    long seconds = (end.tv_sec - start.tv_sec);
    long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
 
    printf("The elapsed time is %d seconds and %d micros\n", seconds, micros);
    return 1;
}
