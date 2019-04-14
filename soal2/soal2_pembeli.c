#include <stdio.h>
#include <sys/socket.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#define PORT 7777

void *connection (void *arg);
int *stock;

int main(int argc, char const *argv[]) {
    key_t key = 12;
    pthread_t connector;

    int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
    stock = shmat(shmid, NULL, 0);

    int t1;
    if((t1 = pthread_create(&connector, NULL, *connection, NULL)))
    {
        printf("Creating thread 1 failed\n");
    }

    pthread_join(connector, NULL);

    shmdt(stock);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}

void *connection (void *arg)
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
      
    // Creating a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    while (1){
        valread = read(new_socket ,buffer, 1024);
        printf("Server menerima perintah %s\n", buffer);

        char hello[100];
        if(strcmp(buffer, "beli") == 0)
        {
            if(*stock > 0)
            {
                *stock = *stock - 1;
                strcpy(hello, "Transaksi berhasil\n");
                send(new_socket , hello , strlen(hello) , 1);
                printf("Transaksi berhasil\n");
            }
            else
            {
                strcpy(hello, "Transaksi gagal\n");
                send(new_socket, hello, strlen(hello), 1);
                printf("Transaksi gagal.\nSilahkan tambah stok anda\n");
            }
        }
    }
}