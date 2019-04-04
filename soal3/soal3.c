#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

void *irajTidur (void *spirit);
void *agmalBangun (void *wakeup);
int iCount = 0, aCount = 0, closeProgram = 0, trigger = 0;
char input[40];
int agmalWakeUp_Status = 0, irajSpirit_Status = 100;

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

   pthread_join(tIraj, NULL);
   pthread_join(tAgmal, NULL);
}

void *agmalBangun (void *wakeup)
{
    while(1)
    {
        if(closeProgram)
            break;

        if(aCount > 2)
        {
            printf("Fitur Agmal Ayo Bangun disabled 10 s\n");
            sleep(10);
            aCount = 0;
            printf("Fitur Agmal Ayo Bangun enabled\n");
        }

        while(trigger == 0)
        {
            // Berikan input
            printf("Input : ");
            gets(input);
            printf("%d\n", trigger);
  
            if(strcmp(input, "All Status") == 0 || strcmp(input, "all status") == 0)
            {
                printf("Agmal WakeUp_Status = %d\n", agmalWakeUp_Status);
                printf("Iraj Spirit_Status = %d\n", irajSpirit_Status);
                // memset(input, 0, strlen(input));
                strcpy(input, "");
                break;
            }

            if (strcmp(input, "Agmal Ayo Bangun") == 0 || strcmp(input, "agmal ayo bangun") == 0)
            {
                if(aCount > 2)
                {
                    printf("Fitur Agmal Ayo Bangun sedang di-disabled\n");
                    memset(input, 0, strlen(input));
                }
                else
                {
                    // Tambahkan agmalWakeUp_Status sebanyak 15
                    // tambah counter untuk mengecek berapa kali
                    // perintah telah dilaksanakan
                    agmalWakeUp_Status += 15;
                    printf("Agmal mulai terbangun dari tidurnya\nWakeUp_Status (+15) : %d\n", agmalWakeUp_Status);
                    aCount++;

                    if (agmalWakeUp_Status >= 100)
                    {
                        printf("Agmal Terbangun, mereka bangun pagi dan berolahraga\n");
                        closeProgram = 1;
                        break;
                    }
                    memset(input, 0, strlen(input));
                }
            }

            if (strcmp(input, "Iraj Ayo Tidur") == 0 || strcmp(input, "iraj ayo tidur") == 0)
            {
                trigger = 1;
                if(closeProgram)
                    break;   
                break;
            }

            else
            {
                memset(input, 0, strlen(input));
                break;
            }
        }
    }

    printf("Agmal Out with trigger %d\n", trigger);
}


void *irajTidur (void *spirit)
{
    while(!closeProgram)
    {
        // Jika perintah "Iraj Ayo Tidur" digunakan lebih dari 3 kali
        // disable fungsi tersebut selama 10 detik
        if(iCount > 2)
        {
            printf("Fitur Iraj Ayo Tidur disabled 10 s\n");
            memset(input, 0, strlen(input));
            sleep(10);
            iCount = 0;
            printf("Fitur Iraj Ayo Tidur enabled\n");
        }
        
        while(trigger)
        {
            if(iCount > 2)
            {
                printf("Fitur Iraj Ayo Tidur sedang di-disabled\n");
                memset(input, 0, strlen(input));
            }
            else
            {
                // Kurangi irajSpirit_Status sebanyak 20,
                // tambah counter untuk mengecek seberapa kali
                // perintah telah dilaksanakan
                irajSpirit_Status -= 20;
                printf("Iraj mulai merasa mengantuk.\nSpirit_Status (-20) : %d\n", irajSpirit_Status);
                iCount++;
                
                // Jika irajSpirit_Status berada tepat atau dibawah nol,
                // maka program akan berhenti.
                if(irajSpirit_Status <= 0)
                {
                    printf("Iraj ikut tidur, dan bangun kesiangan bersama Agmal\n");
                    closeProgram = 1;
                    break;
                }
                memset(input, 0, strlen(input));
            }
            trigger = 0;
            printf("Trigger Happy %d\n", trigger);
            break;
        }
    }
    printf("Iraj Out with trigger %d\n", trigger);
}

