/* 
   client.c 
 */ 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <sys/un.h> 
#include <unistd.h> 

int main(int argc, char * argv[])
{ 
	int sockfd; 
	int len; 
	struct sockaddr_un address; 
	int result; 
	char sockfile[32];

	if (argc < 2)
	{
		printf("usage: ./client [1-9]\nFor example: ./client 1\n");
		exit(0);
	}

	/*建立客户端的套接口,采用AF_UNIX的unix域协议*/ 
	sockfd=socket(AF_UNIX,SOCK_STREAM,0); 

	/*创建服务器套接口的地址,其中包括套接口类型,名称*/ 
	address.sun_family=AF_UNIX; 


	strcpy(sockfile, "/tmp/.codepark_sock");
	strcat(sockfile, argv[1]);
	printf("socket file name: %s\n", sockfile);
	
	strcpy(address.sun_path, sockfile); 

	len=sizeof(address);     /*与服务器套接口建立连接*/ 

	printf("Try to connect broadcast server...\n");
	result=connect(sockfd,(struct sockaddr *)&address,len); 
	if(result==-1){ 
		perror("oops : client"); 
		exit(1); 
	} 

	printf("Server is connected!\n");
	while (1)
	{
		int n = 0;
		char ch = 'A'; 

		n = read(sockfd, &ch, 1);
		//printf("n = %d, char from server=%c\n", n, ch); 

		if (n == 0)
		{	
			printf("Server is closed!\n");
			break;
		}
		else
			write(1, &ch, 1);
	}

	printf("Thanks for watching demo\n");
	close(sockfd); 

	return 0;
}
