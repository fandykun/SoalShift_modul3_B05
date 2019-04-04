#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <termios.h>

char command;
int shop_food_stock;
int food_stock;
char getch();

void *keypress(void *args)
{
    command = 1;
    while (command != 32)
    {
        command = getch();
        if(command == '1') shop_food_stock++;
        else if(command == '2') command = 32;
    }
}

void *permainan(void *arg)
{
    key_t key = 1234;
    char *value;
    int shop_counter  = 0;
    int shmid;
    while (command != 32)
    {
        shmid = shmget(key, 4, IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);
        memcpy(value, &shop_food_stock, 2);
        memcpy(&shop_counter, value + 2, 2);
        food_stock = shop_food_stock - shop_counter;

        printf("Shop\n");
        printf("Food stock\t: %d\n", food_stock);
        printf("Choices\n");
        printf("1. Restock\n");
        printf("2. Exit\n");
        system("clear");

        shmdt(value);
    }
    shmctl(shmid,IPC_RMID,NULL);
}

void main()
{
    pthread_t t1, t2;

    pthread_create(&t1, NULL, &keypress, NULL);
    pthread_create(&t2, NULL, &permainan, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
}

//---Referensi implementasi getch pada C
//https://stackoverflow.com/questions/421860/capture-characters-from-standard-input-without-waiting-for-enter-to-be-pressed/912796#912796
//----------------------------------------
char getch()
{
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0)
        perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
        perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    return (buf);
}