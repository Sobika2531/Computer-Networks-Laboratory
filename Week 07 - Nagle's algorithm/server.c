#include <stdio.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define buff_size 1024

int main()
{
        int sockfd,connfd;
        struct sockaddr_in serv_addr;

        char ack[buff_size]="ACK",buffer[buff_size];

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(4444);
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if(sockfd < 0){
                printf("Socket creation failed!\n");
                return 1;
        }

        if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
                printf("Binding failed!!\n");
                return 1;
        }

        if(listen(sockfd,5)<0){
                printf("Listening failed\n");
                return 1;
        }

        connfd = accept(sockfd, (struct sockaddr*)NULL, NULL);

        if(connfd)
                printf("Connection established with the client\n");
  for(;;){
                recv(connfd,buffer,sizeof(buffer),0);

                if(strncmp(buffer,"exit",strlen("exit"))==0){
                        printf("\n\nexiting.....\n");
                        break;
                }

                printf("\nreceived packets: %s",buffer);
                printf("\nsending acknowledgement..\n");
                send(connfd,ack,strlen(ack)+1,0);
        }

        close(connfd);
        close(sockfd);

        return 0;
}
