#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#define LOCALDNS 4044

int main() {
        int socketfd = 0, sentbytes, recvbytes;
        struct sockaddr_in host_addr;
        char input[20], buffer[20];

        socketfd = socket(AF_INET, SOCK_DGRAM, 0);

        if(socketfd < 0) {
                fprintf(stderr, "Error in socket creation.\n");
                return -1;
        }

        host_addr.sin_family = AF_INET;
        host_addr.sin_port = htons(LOCALDNS);
        inet_pton(AF_INET, "127.0.0.1", &host_addr.sin_addr);

        while(1) {
                fprintf(stdout, ">> Enter the HostName: ");
                scanf("%s", input);
                sentbytes = sendto(socketfd, input, strlen(input) + 1, 0, (struct sockaddr*)&host_addr, sizeof(host_addr));

                if(strncmp(input, "exit", sizeof("exit")) == 0)
                        break;

                recvbytes = recvfrom(socketfd, buffer, sizeof(buffer), 0, NULL, NULL);

                printf("SERVER IP OF %s : %s\n", input, buffer);
        }

        close(socketfd);
        return 0;
}
