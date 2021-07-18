#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define buff_size 1024
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
        while(1){
                connfd = accept(sockfd, (struct sockaddr*)NULL, NULL);
                if(connfd){
                        printf("Connection established with the client\n\n");
                        n = read(connfd,buffer2,buff_size);
                        buffer2[n] = '\0';
                        filename(buffer2,fname);
                        printf("%s",buffer2);
                        fp = fopen(fname,"r");
                        time(&t);
                        if(fp){
                                bzero(buffer1,buff_size);
                                while(fgets(buffer1+strlen(buffer1),buff_size-strlen(buffer1),fp));
                                char* http = "HTTP/1.0 302 Found";
                                snprintf(buffer2,buff_size,"%s\nDate: %s\n%s\n", http,ctime(&t),buffer1);
                        }
                        else{
                                char* http = "HTTP/1.0 404 Not Found";
               snprintf(buffer2,buff_size,"%s\nDate: %s\n", http,ctime(&t));
                        }
                        write(connfd,buffer2,strlen(buffer2));
                        printf("Connection closed with the client\n\n");
                }
        }
}
