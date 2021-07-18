#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include<math.h>

char* generateCRC(char *s, char *b) {
	int l = 0, r = 9, i, count;
	int tmp = r;
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
	for(i = strlen(a) - (tmp - 1); i < strlen(a); i++) 
		a[t++] = a[i];
	a[t] = '\0';
	return a;
}
int main(){
	int listenfd = 0, connfd = 0,i, val, flag = 0;
	struct sockaddr_in serv_addr;
	char buff[1024], div[10], data[100];
	if((listenfd = socket(AF_INET, SOCK_STREAM, 0))<0){
		printf("Error in socket creation\n");
		return 1;

	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if((bind(listenfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))==-1){
		printf("Error in Binding\n");
		return 1;
	}
	if(listen(listenfd, 3) == -1){
		printf("Failed to listen");
		return 1;
	}
	connfd = accept(listenfd, (struct sockaddr*)NULL, NULL);
	val = read(connfd, buff, sizeof(buff));
	buff[val] = '\0';
	strcpy(div, buff);	
	write(connfd, "ACK", 3);
	val = read(connfd, buff, sizeof(buff));
	buff[val] = '\0';
	strcpy(data, buff);
	printf("Data received: %s\n",buff);
	strcpy(div, generateCRC(data, div));
	printf("Remainder - %s\n", div);
	for(i = 0; i < strlen(div); i++) {
		if(div[i] != '0') {
			printf("Incorrect Data\n");
			flag = 1;
			break;
		}
	}
	if(flag == 0) 
		printf("Correct Data\n");
	return 0;
}
