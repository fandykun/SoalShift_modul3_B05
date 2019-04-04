#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

void *control (void *arg);
void *kowalski (void *arg);
void *irajSlep (void *arg);
void *agmalAwoke (void *arg);
int trigger = 0;
int agmalWakeUp_Status = 0, irajSpirit_Status = 100;
int iCount = 0, aCount = 0, finishStatus = 0;
time_t timeStop, timeNow;

int main()
{
    int iController, statController, iraj, agmal;
    pthread_t inputController, statusController;
    pthread_t tIraj, tAgmal;

    if(iController = pthread_create(&inputController, NULL, *control, NULL))
    {
        printf("Failed to create thread\n");
        exit(EXIT_FAILURE);
    }

    if(statController = pthread_create(&statusController, NULL, *kowalski, NULL))
    {
        printf("Failed to create thread\n");
        exit(EXIT_FAILURE);
    }

    if(iraj = pthread_create(&tIraj, NULL, *irajSlep, NULL))
    {
        printf("Creating thread \"Iraj\" failed\n");
        exit(EXIT_FAILURE);
    }

    if(agmal = pthread_create(&tAgmal, NULL, *agmalAwoke, NULL))
    {
        printf("Creating thread \"Agmal\" failed\n");
        exit(EXIT_FAILURE);
    }

    pthread_join(inputController, NULL);
    pthread_join(tIraj, NULL);
    pthread_join(tAgmal, NULL);
}

void *control (void *arg)
{
    char input[50];
    
    while (!finishStatus)
    {   
        gets(input);
        if (strcmp(input, "All Status") == 0 || strcmp(input, "all status") == 0)
        {
            printf("Agmal WakeUp_Status = %d\n", agmalWakeUp_Status);
            printf("Iraj Spirit_Status = %d\n\n", irajSpirit_Status);
            memset(input, 0, strlen(input));
        }

        else if (strcmp(input, "Agmal Ayo Bangun") == 0 || strcmp(input, "agmal ayo bangun") == 0)
        {
            trigger = 1;
            strcpy(input, "");
        }

        else if (strcmp(input, "Iraj Ayo Tidur") == 0 || strcmp(input, "iraj ayo tidur") == 0)
        {
            trigger = 2;
            strcpy(input, "");
        }
    }

    printf("Program completed\n");
}

void *kowalski (void *arg)
{
    while(!finishStatus)
    {   
        if (aCount == 3 || iCount == 3)
        {
            timeStop = time(NULL);
            while(1)
            {
                timeNow = time(NULL);
                if(timeNow - timeStop == 10)
                {
                    if(aCount > 2) {
                        aCount = 0;
                        printf("Fitur Agmal Ayo Bangun diaktifkan kembali\n");
                        break;
                    }

                    else if(iCount > 2) {
                        iCount = 0;
                        printf("Fitur Iraj Ayo Tidur diaktifkan kembali\n");
                        break;
                    } 
                }
                printf("> %lu detik lagi sebelum fungsi diaktifkan\n", timeNow - timeStop);
                sleep(1);
            }
        }
    }
}

void *agmalAwoke (void *arg)
{
    while(1)
    {
        if(finishStatus)
            break;

        /*while(trigger != 1)
        {
        }*/
        while (trigger == 1){
            if(aCount < 3)
            {
                agmalWakeUp_Status += 15;
                if(agmalWakeUp_Status < 100)
                printf("Agmal mulai terbangun dari tidurnya\nWakeUp_Status (+15) : %d\n\n", agmalWakeUp_Status);

                else if (agmalWakeUp_Status >= 100)
                { 
                    printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
                    finishStatus = 1;
                    break;
                }
                aCount++;
            }
            else
            {
                printf("Fitur Agmal Ayo Bangun sedang di-disabled\n");   
            }

            trigger = 0;
        }
    }
}

void *irajSlep (void *arg)
{
    while(1)
    {       
        if (finishStatus)
            break;

        while(trigger == 2){
            if(iCount < 3)
            {
                irajSpirit_Status -= 20;
                if(irajSpirit_Status > 0)
                    printf("Iraj mulai merasa mengantuk.\nSpirit_Status (-20) : %d\n\n", irajSpirit_Status);
                
                else if (irajSpirit_Status <= 0)
                {
                    printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
                    finishStatus = 1;
                    break;
                }

                iCount++;
            }
            else
            {
                printf("Fitur Iraj Ayo Tidur sedang di-disabled\n");
            }

            trigger = 0;
        }
    }
}