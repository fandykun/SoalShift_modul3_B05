#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid_save[2];
pthread_t tid_zip[2];
pthread_t tid_unzip[2];
int status = 4;

void *save(void *args)
{
    pthread_t id = pthread_self();
    if (pthread_equal(id, tid_save[0]))
        system("mkdir -p /home/fandykun/Documents/FolderProses1 && ps -aux | tail -10 > /home/fandykun/Documents/FolderProses1/SimpanProses1.txt");
    else
        system("mkdir -p /home/fandykun/Documents/FolderProses2 && ps -aux | tail -10 > /home/fandykun/Documents/FolderProses2/SimpanProses2.txt");
    status--;
}

void *zip(void *args)
{
    while(status != 2) ;
    pthread_t id = pthread_self();
    if (pthread_equal(id, tid_zip[0])) {
        system("zip -j /home/fandykun/Documents/FolderProses1/KompresProses1.zip /home/fandykun/Documents/FolderProses1/SimpanProses1.txt");
        system("rm -f /home/fandykun/Documents/FolderProses1/SimpanProses1.txt");
    } else {
        system("zip -j /home/fandykun/Documents/FolderProses2/KompresProses2.zip /home/fandykun/Documents/FolderProses2/SimpanProses2.txt");
        system("rm -f /home/fandykun/Documents/FolderProses2/SimpanProses2.txt");
    }
    status--;
}

void *unzip(void *args)
{
    pthread_t id = pthread_self();
    if (pthread_equal(id, tid_unzip[0]))
        system("unzip /home/fandykun/Documents/FolderProses1/KompresProses1.zip -d /home/fandykun/Documents/FolderProses1/");
    else 
        system("unzip /home/fandykun/Documents/FolderProses2/KompresProses2.zip -d /home/fandykun/Documents/FolderProses2/");
}

int main(int argc, char **argv)
{
    for(int i = 0;i < 2; i++) {
        pthread_create(&tid_save[i], NULL, &save, NULL);
        pthread_create(&tid_zip[i], NULL, &zip, NULL);
    }
    for(int i = 0;i < 2; i++) {
        pthread_join(tid_save[i], NULL);
        pthread_join(tid_zip[i], NULL);
    }

    while(status > 0) ;
    printf("Menunggu 15 detik untuk mengekstrak kembali\n");
    sleep(15);

    for(int i = 0;i < 2; i++)
        pthread_create(&tid_unzip[i], NULL, &unzip, NULL);
    for(int i = 0;i < 2; i++)
        pthread_join(tid_unzip[i], NULL);

    return 0;
}