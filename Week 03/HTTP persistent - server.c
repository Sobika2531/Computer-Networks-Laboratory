#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define buff_size 1024
void resp(char *buffer,char *filename,char *content,char* time){
        char *http = "HTTP/1.1 302 Found";
        char *cnxn = "Connection: Keep-alive";
        snprintf(buffer,buff_size,"%s\nDate: %s\n%s\n%s\n",http,time,cnxn,content);
}
void filename(char *a, char *b){
        int i=4,j=0;
        while(a[i] != '\n'){
                b[j] = a[i];
                i++;
                j++;
        }
        b[j] = '\0';
}
int main(){
        int sockfd,connfd,n=0,i=0;
        char c;
        struct sockaddr_in serv_addr;
        char fname[20],buffer1[buff_size],buffer2[buff_size];
        FILE *fp;
        time_t t;
        bzero(buffer1,buff_size);
        bzero(buffer2,buff_size);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(6500);
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
        while(1){
                n = read(connfd,buffer2,buff_size);
                buffer2[n] = '\0';
                filename(buffer2,fname);
          if((strncmp(fname,"exit",4))==0){
                        char *msg  = "Connection closed!!!\n";
                        printf("Connection ended !!!\n");
                        write(connfd,msg,strlen(msg));
                        return 0;
                }
                printf("%s",buffer2);
                fp = fopen(fname,"r");
                if(fp){
                        bzero(buffer1,buff_size);
                        while(fgets(buffer1+strlen(buffer1),buff_size-strlen(buffer1),fp));
                        time(&t);
                        resp(buffer2,fname,buffer1,ctime(&t));
                        write(connfd,buffer2,strlen(buffer2));
                }
                else
                        write(connfd,"File not found", strlen("File not found"));
        }
}
