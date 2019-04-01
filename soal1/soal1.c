#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

unsigned long long faktorial[60];

void *cetak(void *args)
{
    int *nilai = (int *)args;
    printf("Faktorial %d!\t= %llu\n", *nilai, faktorial[*nilai]);
}

void sorting(int bilangan[], int n)
{
    for (int i = 0; i < n - 1; i++)
    {
        for(int j = i + 1; j < n; j++)
        {
            if (bilangan[i] > bilangan[j])
            {
                int temp = bilangan[i];
                bilangan[i] = bilangan[j];
                bilangan[j] = temp;
            }
        }
    }
}

int main(int argc, char **argv)
{
    faktorial[1] = 1;
    for (int i = 2; i < 60; i++)
        faktorial[i] = i * faktorial[i - 1];

    pthread_t tid[argc];
    int bilangan[argc];
    int n = argc - 1;

    for (int i = 1; i < argc; i++)
        bilangan[i - 1] = atoi(argv[i]);

    sorting(bilangan, n);

    for (int i = 0; i < n; i++)
    {
        pthread_create(&tid[i], NULL, &cetak, (void *)&bilangan[i]);
        pthread_join(tid[i], NULL);
    }

    return 0;
}