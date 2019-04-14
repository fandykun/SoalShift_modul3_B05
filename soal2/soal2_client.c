#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
  
int main(int argc, char const *argv[]) {
    unsigned short int PORT;
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));

    int choice;
    printf("Input sesuai angka yang muncul\n");
    printf("Anda adalah (1)pembeli atau (2)penjual? : ");
    scanf("%d", &choice);

    if(choice == 1)
    {
        PORT = 7777;
        printf("Tersambung di Port : %hu\n", PORT);
    }
    else if (choice == 2)
    {
        PORT = 8080;
    }
    else
    {
        printf("Input salah\n");
    }
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    /* This function converts the character string "127.0.0.1" into a network
       address structure in the AF address family (in this case AF_INET), 
       then copies the network address structure to &serv_addr.sin_addr.
       &serv_addr.sin_addr is written in network byte order. 
    */

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
    }
    
    /*  Connect a socket to a server address as stated by serv_addr struct
        with the size of the serv_addr struct
    */
    
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    /*  Sample command here
    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer ); 
    */

    while(1) {
        
        char input[15];
        printf("input command: ");
        scanf("%s", input);

        if (choice == 1 && strcmp(input, "beli") == 0)
        {   
            send(sock, input, strlen(input), 0);
            valread = read(sock, buffer, 1024);
            printf("%s\n", buffer);
        }    
        else if (choice == 2 && strcmp(input, "tambah") == 0)
        {
            send(sock, input, strlen(input), 0);
            valread = read(sock, buffer, 1024);
            printf("%s\n", buffer);
        }
        else
        {
            printf("Input salah...\n");
        }

        memset(input, 0, strlen(input));
    }
    return 0;
}
