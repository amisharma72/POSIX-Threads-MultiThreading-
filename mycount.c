# include<stdio.h>
# include<stdlib.h>
# include<pthread.h>
# include<ctype.h>

int counter=0;               /* Global variable */
typedef struct              /* Declaring a struct type with typedef */
{
 char *filename;
 char b;
}outfile;
pthread_mutex_t count=PTHREAD_MUTEX_INITIALIZER;    /* Initializing a mutex */

void *occurrences(void *arg)
{
    char buff;
    FILE *fp;
    outfile *data;
    data=(outfile*)arg;
    
    if((fp = fopen(data->filename, "r"))!=NULL)     /* Opens a file for reading */
    {
        while(fscanf(fp,"%c",&buff)!=EOF)
        {
            if(buff==data->b)
            {
                pthread_mutex_lock(&count);          /* Locks a mutex object */
                counter++;
                pthread_mutex_unlock(&count);       /* Releases a mutex object */
            }
        }
    fclose(fp);
    pthread_exit(NULL);
    }
    else
    {
        perror("Couldn't open file!\n");
        exit(1);
    }
}

int main(int argc, const char *argv[])
{
    int j=argc-2;
    outfile data;
    pthread_t t[j];        /* Declare threads */
    if (argc<=2)
    {
        perror("Give an appropriate command format!\n");
        exit(1);
    }
    for (int i=0;i<j;i++)      /* Runs all threads in parallel */
    {
        data.b = argv[1][0];
        data.filename = (char *)argv[i + 2];
        pthread_create(& t[i],NULL,occurrences,(void*)&data); /* Create a new thread for every argument */
        pthread_join(t[i],NULL);                      /* Waits for termination of a thread */
    }
    printf("%d occurrences of the letter %c found in %d threads\n",counter,argv[1][0],j);
    return 0;
}


