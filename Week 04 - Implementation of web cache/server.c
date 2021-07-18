#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define size 1024
char buffer[size];

void extract(char* buffer, char* name, char* date){
        int i = 0,j=0,k=0;
        while(buffer[i] != ' ')i++;
        while(buffer[++i] != '\n'){
                name[j] = buffer[i];
                j++;
        }
        name[j] = '\0';
        if(buffer[++i] != '\0'){
                while(buffer[i] != ' ')i++;
                while(buffer[++i] != '\n'){
                        date[k] = buffer[i];
                        k++;
                }
        }
        date[k] = '\0';
}

int main(){
        int sockfd,connfd,n=0;
        struct sockaddr_in serv_addr;
        char url[100];
        char pd[100];
        char d[100];

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
        if(connfd){
                printf("Proxy server got connected!!!\n");
        }
        FILE* fp;
        while(1){
                bzero(buffer,size);
                n = read(connfd,buffer,size);
                if(n>0){
                        bzero(d,100);
                        buffer[n] = '\0';
                        printf("Request from the proxy server: \n%s\n", buffer);
                        extract(buffer,url,pd);
                        fp = fopen("origin.txt", "r");
                        while(fgets(buffer,size,fp) != NULL ){
                                if(strncmp(url,buffer,strlen(url))==0){
                                                fseek(fp,-(strlen(buffer)-strlen(url)-1), SEEK_CUR);
                                                fgets(d,100,fp);
                                                break;
                                }
                        }
                        if(strlen(d)==0)
                                snprintf(buffer,size,"%s\n", "Not found");
                        else if(strncmp(pd,d,strlen(d)-1)==0)
                                snprintf(buffer,size,"%s\n", "NULL");
                        else{
                                snprintf(buffer,size,"Last-modified: %s\n",d);
                        }
                        sleep(2);
                        write(connfd,buffer,strlen(buffer));
                }
        }
}
