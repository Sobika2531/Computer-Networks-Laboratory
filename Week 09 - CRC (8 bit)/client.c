#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<fcntl.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

char* generateCRC(char *s, char *b) {
	int l = 0, r = 9, i, count;
	char* a = (char*)malloc(sizeof(100));
	strcpy(a,s);
	int t = strlen(a);
	for(i = 1; i <r; i++) 
		a[t++] = '0';
	a[t] = '\0';
	while(r <= strlen(a)) {
		count = 0;
		t = 0;
		for(i = l; i < r; i++) {
			if(a[i] == b[t++]) a[i] = '0';
			else a[i] = '1';
		}
		for(i = l; ; i++) {
			if(a[i] != '0') break;
			count++;
		}
		l = l + count;
		r = r + count;
	}
	t = 0;
	for(i = strlen(a) - 8; i < strlen(a); i++) 
		a[t++] = a[i];
	a[t] = '\0';
	return a;
}
int main(){
	int sockfd = 0, connfd = 0,i;
	char buff[1024]={0}, data[100], *crc=(char*)malloc(10), div[10];
	struct sockaddr_in serv_addr;
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("Error in socket creation\n");
		return 1;
	}
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_port=htons(5000);
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if((connfd = connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))<0){
		printf("Error in connecting\n");
	}
	else printf("Successfully connected\n");
	printf("CRC Generator = ");
	scanf("%s", div);
	write(sockfd, div, strlen(div));
	read(sockfd, buff, sizeof(buff));
	printf("Data = ");
	scanf("%s", data);
	crc = generateCRC(data, div);
	strcat(data,crc);
	printf("CRC = %s\nData + CRC = %s\n",crc,data);
	printf("Enter y to send data, CTRL+D to cause problem in transmission");
	char choice;
	scanf("\n%c",&choice);
	if(choice == 0) {
		srand(time(0));
		int random = rand() % strlen(data);
		data[random] = (data[random] == '0') ? '1' : '0';
	}
	write(sockfd, data, strlen(data));
	printf("Data + CSV sent\n");
	close(connfd);
	return 0;
}
