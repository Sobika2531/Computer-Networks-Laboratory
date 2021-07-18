#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

int main()
{

        int sockfd, connfd, len,op1,op2,result;
        char operator;
        struct sockaddr_in servaddr, cli;

        sockfd = socket(AF_INET, SOCK_STREAM, 0);

        if (sockfd == -1) {
                printf("socket creation failed...\n");
                exit(0);
        }

        else
                printf("Socket successfully created..\n");

        bzero(&servaddr, sizeof(servaddr));

        servaddr.sin_family = AF_INET;
        servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        servaddr.sin_port = htons(PORT);

        if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
                printf("socket bind failed...\n");
                exit(0);
        }

        else
                printf("Socket successfully binded..\n");

        if ((listen(sockfd, 5)) != 0) {
                printf("Listen failed...\n");
                exit(0);
        }

        else
                printf("Server listening..\n");

        len = sizeof(cli);

        connfd = accept(sockfd, (SA*)&cli, &len);

        if (connfd < 0) {
                printf("server acccept failed...\n");
                exit(0);
        }

        else
                printf("server acccept the client...\n");

        read(connfd, &operator, 10);
        read(connfd, &op1, sizeof(op1));
        read(connfd, &op2,sizeof(op2));

        switch(operator)
        {
                case '+': {result=op1+op2;
                                  printf("result : %d + %d =%d\n",op1,op2,result);
                                  break;
                          }

                case '-': {result=op1-op2;
                                  printf("result : %d - %d =%d\n",op1,op2,result);
                                  break;
                          }
                case '*': {result=op1*op2;
                                  printf("result : %d * %d =%d\n",op1,op2,result);
                                 break;
                          }
                case '/': {result=op1/op2;
                                  printf("result : %d / %d =%d\n",op1,op2,result);
                                  break;
                          }
                case '%': {result=op1%op2;
                                  printf("result : %d % %d =%d\n",op1,op2,result);
                                  break;
                          }
                default:
                          printf("ERROR: unsupported operation\n");
        }

        write(connfd,&result,sizeof(result));

        close(sockfd);

}
