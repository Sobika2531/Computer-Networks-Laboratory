#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define size 1024

void req(char* buffer, char* url, char* date){
        if(strlen(date)==0)
                snprintf(buffer,size,"GET %s\n", url);
        else
                snprintf(buffer,size,"GET %s\nIF-MODIFIED-SINCE %s\n",url,date);
}

int update(char* filename, char* buffer){
        FILE* fp = fopen("proxy.txt","r+");
        char ch;
        char line[50],date[20];
        int i = 0,j=0;
        while(buffer[i] != ' ')i++;
        while(buffer[++i] != '\n'){
                date[j] = buffer[i];
                j++;
        }
        date[j] = '\0';
        while((fgets(line,50,fp)) != NULL){
                if((strncmp(filename,line,strlen(filename)))==0){
                        fseek(fp,-(strlen(line)-strlen(filename)-1), SEEK_CUR);
                        fputs(date,fp);
                        goto EXIT;
                }
        }
        snprintf(buffer,size,"%s,%s\n",filename,date);
        fputs(buffer,fp);
EXIT:
        fclose(fp);
        printf("Proxy Server updated\n");
        return 0;;
}


int main(){
        int sockfd,connfd,sockfd2,connfd2=0,n=0;
        struct sockaddr_in serv_addr;
        struct sockaddr_in serv_addr2;
        char buffer[size];
        char url[100];
        char d[100];
        FILE* fp;

        bzero(buffer,size);

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(6500);
        serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

        serv_addr2.sin_family = AF_INET;
        serv_addr2.sin_port = htons(7500);
        serv_addr2.sin_addr.s_addr = htonl(INADDR_ANY);

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd < 0){
                printf("Socket creation failed!\n");
                return 1;
        }
        sockfd2 = socket(AF_INET, SOCK_STREAM, 0);
        if(sockfd2 < 0){
                printf("Socket2 creation failed\n");
        }

        connfd = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
        if(bind(sockfd2, (struct sockaddr*)&serv_addr2, sizeof(serv_addr2))<0){
                printf("Binding failed \n");
                return 1;
        }
        if(listen(sockfd2,3)<0){
                printf("Listening failed\n");
                return 1;
        }
        connfd2 = accept(sockfd2, (struct sockaddr*)NULL, NULL);
        if(connfd2){
                printf("Client got connected\n");
        }
        while(1){
                n = read(connfd2,url,100);
                if(n>0){
                        bzero(d,100);
                        bzero(buffer,size);
                        url[n] = '\0';
                        fp = fopen("proxy.txt", "r");
                        while(fgets(buffer,size,fp)){
                                if(strncmp(url,buffer,strlen(url))==0){
                                        fseek(fp,-(strlen(buffer)-strlen(url)-1), SEEK_CUR);
                                        fgets(d,100,fp);
                                        break;
                                }
                                strcpy(d,"");
                        }
                        fclose(fp);
                req(buffer,url,d);
                write(sockfd,buffer,strlen(buffer));
R:
                n = read(sockfd,buffer,size);
                if(n>0){
                buffer[n] = '\0';
                printf("Response from origin server: \n%s\n", buffer);

                }
                else goto R;

                if(strncmp(buffer,"NULL",4)==0)
                        write(connfd2,"Proxy Server",strlen("Proxy Server"));
                else if (strncmp(buffer,"Last",4)==0){
                        update(url,buffer);
                        write(connfd2,"Origin Server",strlen("Origin Server"));
                }
                else
                        write(connfd2,"Not Found",strlen("Not found"));
                bzero(buffer,size);
                }

        }
}
