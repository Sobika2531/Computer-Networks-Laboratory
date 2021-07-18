#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#define size 1024
void request(char *buffer, char* filename, char* time){
        char *host = "Host: www.something.com";
        char *cnxn = "Connection Keep-alive";
        snprintf(buffer,size,"GET %s\n%s\n%s\nDate: %s\n", filename,host,cnxn,time);
}
int main(){
        int sockfd,connfd=0,n=0;
        struct sockaddr_in serv_addr;
        char buffer[size];
        char fname[100];
        time_t t;
        time(&t);
        bzero(buffer,size);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(6500);
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd<0){
                printf("Socket creation failed\n");
                return 1;
        }
        connfd = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        printf("\nEnter the Filename: ");
        scanf("%s", fname);
        time(&t);
        request(buffer, fname, ctime(&t));
        write(sockfd,buffer,strlen(buffer));
        if((strncmp(fname,"exit",4))==0)
                return 0;
        n=read(sockfd,buffer,size);
        buffer[n] = '\0';
        printf("%s", buffer);
}
