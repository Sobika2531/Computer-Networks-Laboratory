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
        int sockfd1,connfd1,sockfd2,connfd2,i=0;
        struct sockaddr_in serv_addr;
        char uname[100],pw[100],fname[100],buffer[buff_size],c;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(4444);
        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
        sockfd1 = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd1 < 0){
                printf("Socket creation failed!\n");
                return 1;
        }
        if(bind(sockfd1, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
                printf("Binding failed!!\n");
                return 1;
        }
        if(listen(sockfd1,5)<0){
                printf("Listening failed\n");
                return 1;
    }
        connfd1 = accept(sockfd1, (struct sockaddr*)NULL, NULL);
        if(connfd1)
                printf("Connection established with the client\n");
        while(1){
                recv(connfd1,uname,sizeof(uname),0);
                recv(connfd1,pw,sizeof(pw),0);
                if(((strncmp(uname,"zara",strlen("zara")))==0)&&((strncmp(pw,"1234",strlen("1234")))==
0))
                {
                        printf("valid client\n\n");
                        send(connfd1,"valid",strlen("valid")+1,0);
                        break;
                }
                else
                        send(connfd1,"invalid",strlen("invalid")+1,0);
        }
        serv_addr.sin_port = htons(5555);
        sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd2 < 0){
                printf("Socket creation failed!\n");
                return 1;
        }
        if(bind(sockfd2, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0){
                printf("Binding failed!!\n");
                return 1;
        }
  if(listen(sockfd2,5)<0){
                printf("Listening failed\n");
                return 1;
        }
        connfd2 = accept(sockfd2, (struct sockaddr*)NULL, NULL);
        if(connfd2)
                printf("Connection established with the client\n\n");
        while(1){
                recv(connfd2,fname,sizeof(fname),0);
                 if((strncmp(fname,"exit",strlen("exit")))==0)
                 {
                         printf("\nexiting...\n");
                        close(connfd2);
                        close(connfd1);
                        break;
                 }
                else
                {
                        printf("\n\nfrom client:%s\n",fname);
                        int fd=open(fname,O_RDONLY);
                        if(fd<0)
                        {
                        printf("file not found\n");
                        send(connfd2,"not found",strlen("not found")+1,0);
                        }
            else{
                        printf("file found\n");
                        send(connfd2,"found",strlen("found")+1,0);
                        recv(connfd2,buffer,sizeof(buffer),0);
                        if(strncmp(buffer,"yes",strlen("yes"))==0)
                        {
                                printf("sending file content to the client...\n");
                                read(fd,buffer,sizeof(buffer));
                                send(connfd2,buffer,strlen(buffer)+1,0);
                        }
                        }
                }
        }
        return 0;
}
