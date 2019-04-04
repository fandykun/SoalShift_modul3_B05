#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>

#define STANDBY 'A'
#define BATTLE 'B'
#define SHOP 'C'

#define PLAYER 1
#define ENEMY 2

char getch();

char command;
char status_scene;
int status_bath;
//---Standby
int health_status;
int hunger_status;
int hygiene_status;
int food_stock;
//---Battle
int enemy_health_status;
int battle_status;
//---shop
int shop_food_stock;
int shop_counter = 0;
//---timer
int elapsedTime;
int hungry_time;
int hygiene_time;
int health_time;
int bath_time;
int cooldown_bath_time;

void *keypress(void *args)
{
    command = 1;
    while (command != 32)
    {
        command = getch();
        if(status_scene == STANDBY)
        {
            switch(command) {
                case '1':
                    if(food_stock > 0) {
                        hunger_status += 15;
                        food_stock--;
                    }
                    break;
                case '2': 
                    if(status_bath == 1) {
                        hygiene_status += 30;
                        cooldown_bath_time = 20;
                        status_bath = 0;
                        bath_time = elapsedTime;
                    }
                    break;
                case '3': 
                    status_scene = BATTLE;
                    break;
                case '4': status_scene = SHOP;
                    break;
                case '5':
                    command = 32;
                    break;
            }
        }
        else if(status_scene == BATTLE)
        {
            switch(command) {
                case '1':
                    if(battle_status == PLAYER) {
                        enemy_health_status -= 20;
                        battle_status = ENEMY;
                    }
                    else if(battle_status = ENEMY) {
                        health_status -= 20;
                        battle_status = PLAYER;
                    }
                    break;
                case '2':
                    status_scene = STANDBY;
                    enemy_health_status = 100;
                    break;
            }
        }
        else if(status_scene == SHOP)
        {
            switch(command) {
                case '1':  
                    if(shop_food_stock - shop_counter> 0) {
                        food_stock++;
                        shop_counter++;
                    }
                    break;
                case '2': status_scene = STANDBY;
                    break;
            }
        }
    }
}   

void *permainan(void *args)
{
    char monster[50];
    strcpy(monster, (void *)args);
    key_t key = 1234;
    int shmid = shmget(key, 4, IPC_CREAT | 0666);
    char *value = shmat(shmid, NULL, 0);
    
    while (command != 32)
    {
        memcpy(value + 2, &shop_counter, 2);
        memcpy(&shop_food_stock, value, 2);
        elapsedTime = (int)time(NULL);
        if(status_scene != BATTLE) {
            if(elapsedTime - hungry_time > 10) {
                hunger_status -= 5;
                hungry_time = elapsedTime;
            }  
            if(elapsedTime - hygiene_time > 30) {
                hygiene_status -= 10;
                hygiene_time = elapsedTime;
            }
            if(elapsedTime - health_time > 10) {
                health_status += 5;
                health_time = elapsedTime;
            }

            if(status_bath == 0)
                cooldown_bath_time = 20 - (elapsedTime - bath_time);
            if(cooldown_bath_time == 0)
                status_bath = 1;
        }
        else {
            health_time = elapsedTime;
            hungry_time = elapsedTime;
            health_time = elapsedTime;
            bath_time = elapsedTime - 20 + cooldown_bath_time;
        }
        switch(status_scene) {
            case STANDBY:
                printf("Standby Mode\n");
                printf("Health status\t: %d\n", health_status);
                printf("Hunger status\t: %d\n", hunger_status);
                printf("Hygiene status\t: %d\n", hygiene_status);
                printf("Food left\t: %d\n", food_stock);
                if(status_bath == 1)
                    printf("Bath is ready.\n");
                else
                    printf("Bath will be ready in %ds\n", cooldown_bath_time);
                printf("Choices\n");
                printf("1. Eat\n");
                printf("2. Bath\n");
                printf("3. Battle\n");
                printf("4. Shop\n");
                printf("5. Exit\n");
                system("clear");
                break;
            case BATTLE:
                printf("Battle Mode\n");
                printf("%s's Health : %d\n", monster, health_status);
                printf("Enemy's Health\t : %d\n", enemy_health_status);
                printf("Choices\n");
                printf("1. Attack\n");
                printf("2. Run\n");
                system("clear");
                break;
            case SHOP:
                printf("Shop Mode\n");
                printf("Shop food stock : %d\n", shop_food_stock - shop_counter);
                printf("Your food stock : %d\n", food_stock);
                printf("Choices\n");
                printf("1. Buy\n");
                printf("2. Back\n");
                system("clear");
                break;
            default:
                printf("SALAH PENCET BRO\n");
                system("clear");
                break;
        }
        if(enemy_health_status == 0) {
            printf("PLAYER WIN.\n");
            command = 32;
        }
        else if(health_status == 0) {
            printf("GAME OVER.\n");
            command = 32;
        }
    }
    shmdt(value);
    shmctl(shmid,IPC_RMID,NULL);
}

int main()
{
    //Inisialisasi
    health_status = 300;
    hunger_status = 200;
    hygiene_status = 100;
    food_stock = 0;
    shop_food_stock = 0;
    status_scene = STANDBY;
    hungry_time = (int)time(NULL);
    hygiene_time = (int)time(NULL);
    health_time = (int)time(NULL);
    bath_time= (int)time(NULL);
    status_bath = 1;
    enemy_health_status = 100;
    battle_status = PLAYER;
    //--------------------

    char nama_monster[50];
    printf("Masukkan nama monster: ");
    scanf("%s", nama_monster);

    pthread_t tid_input;
    pthread_t tid_state;

    pthread_create(&tid_input, NULL, &keypress, NULL);
    pthread_create(&tid_state, NULL, &permainan, (void *)nama_monster);
    
    pthread_join(tid_input, NULL);
    pthread_join(tid_state, NULL);
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