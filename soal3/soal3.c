#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

void *irajTidur (void *arg);
void *agmalBangun (void *arg);
int aCount = 0, iCount = 0, flagF = 0, flagI = 0, flagA = 0;
int agmalWakeUp_Status = 0, irajSpirit_Status = 100;
char input[50];

int main()
{
    int iraj, agmal;
    pthread_t tIraj, tAgmal;
   
   if((iraj = pthread_create(&tIraj, NULL, *irajTidur, NULL)))
   {
       printf("Gagal membuat thread \"Iraj\"\n");
   }
   
   if((agmal = pthread_create(&tAgmal, NULL, *agmalBangun, NULL)))
   {
       printf("Gagal membuat thread \"Agmal\"\n");
   }

   while(1)
   {
        if(flagF)
            break;
        else
        {
            gets(input);
            if(strcmp(input, "All Status") == 0 || strcmp(input, "all status") == 0)
            {
                    printf("Agmal WakeUp_Status = %d\n", agmalWakeUp_Status);
                    printf("Iraj Spirit_Status = %d\n", irajSpirit_Status);
            }
        
            if(strcmp(input, "Iraj Ayo Tidur") == 0 || strcmp(input, "iraj ayo bangun") == 0)
            {
                    if(iCount < 3)
                        flagI = 1;
                    else 
                        printf("Fitur Iraj Ayo Tidur sedang dimatikan, harap tunggu\n");        
            }

            if(strcmp(input, "Agmal Ayo Bangun") == 0 || strcmp(input, "agmal ayo bangun") == 0)
            {
                    if(aCount < 3)
                        flagA = 1;
                    else
                        printf("Fitur Agmal Ayo Bangun sedang dimatikan, harap tunggu\n");
            }

            memset(input, 0, strlen(input));
        }
   }

   pthread_join(tIraj, NULL);
   pthread_join(tAgmal, NULL);
}

void *irajTidur (void *arg)
{
    while(1)
    {
        if(flagF) break;

        if(iCount > 2)
        {
            printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
            sleep(10);
            iCount = 0;
            printf("Fitur Iraj Ayo Tidur enabled\n");
        }

        if(flagI)
        {
            irajSpirit_Status -= 20;
            printf("Iraj mulai merasa mengantuk.\nSpirit_Status (-20) : %d\n", irajSpirit_Status);
            aCount++;
            
            // Jika irajSpirit_Status berada tepat atau dibawah nol,
            // maka program akan berhenti.
            if(irajSpirit_Status <= 0)
            {
                printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
                exit(0);
            }

            flagI = 0;
        }
    }
}

void *agmalBangun (void *arg)
{
    while(1)
    {
        if(flagF) break;

        if(aCount > 2)
        {
            printf("Fitur Agmal Ayo Bangun disabled 10 s\n");
            sleep(10);
            aCount = 0;
            printf("Fitur Agmal Ayo Bangun enabled\n");
        }

        if(flagA)
        {
            agmalWakeUp_Status += 15;
            printf("Agmal mulai terbangun dari tidurnya\nWakeUp_Status (+15) : %d\n", agmalWakeUp_Status);
            iCount++;

            if (agmalWakeUp_Status >= 100)
            {
                printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
                exit(0);
            }

            flagA = 0;
        }
    }
}
