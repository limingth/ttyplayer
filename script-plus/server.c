/*
 * server.c
 */
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>

#include <pthread.h>

#define printf(args...)		fprintf(stderr, ##args)

int clients = 0;
int fd[64];

void *thr_cast(void *arg)
{
	printf("server casting now...\n");

	while (1)
	{
		char ch;
		int i;
		int n = 0;

		/* get a char from stdin */
		n = read(0, &ch, 1);
		printf("n = %d, ch = %c(0x%x)\n", n, ch, ch);

		if (n <= 0)
		{
			printf("broadcast is over, thanks for using.\n");
			exit(0);
		//	break;
		}

		// write to stdout
		write(1, &ch, 1);

		for (i = 0; i < clients; i++)
		{
			// write the char to every socket
			//printf("client fd[%d] is %d\n", i, fd[i]);
			//printf("begin to write to client%d\n", i+1);
			if (fd[i] != 0)
				n = write(fd[i], &ch, 1);
			//printf("write = %d\n", n);

			// if student socktet is broken, return -1
			if (n < 0)
			{
				fd[i] = 0;
				printf("client %d is disconnected\n", i+1);
			}

			printf("boardcasts to client %d ok!\n", i+1);
		}
	}

	return;
}

#include <unistd.h>
#include <signal.h>
#include <stdio.h>

void sig_alrm(int signo)
{
	/* nothing to do */
	printf("sig %d handled!\n", signo);

	return;
}

void install_sig_handler()
{
	struct sigaction newact, oldact;
	unsigned int unslept;

	newact.sa_handler = sig_alrm;
	sigemptyset(&newact.sa_mask);
	newact.sa_flags = 0;
	sigaction(SIGPIPE, &newact, &oldact);

}

int main(int argc, char * argv[])
{
	int server_sockfd,client_sockfd;
	int server_len,client_len;
	struct sockaddr_un server_address;
	struct sockaddr_un client_address;
	pthread_t ntid;
	char sockfile[32];

	if (argc < 2)
	{
		printf("Usage: ./server [1-9]\nFor example: ./server 1\n");
		printf("the number must be different with others!\n");
		exit(0);
	}

	/* remove the existing socket */
	strcpy(sockfile, "/tmp/.codepark_sock");
	strcat(sockfile, argv[1]);
	printf("socket file name: %s\n", sockfile);
	unlink(sockfile);

	/* create host internal socket */
	server_sockfd=socket(AF_UNIX,SOCK_STREAM,0);
	server_address.sun_family=AF_UNIX;
	strcpy(server_address.sun_path,sockfile);
	server_len=sizeof(server_address);

	bind(server_sockfd,(struct sockaddr *)&server_address,server_len);
	listen(server_sockfd,5);

	int n = 0;
	n = pthread_create(&ntid, NULL, thr_cast, "new thread");

	install_sig_handler();

	/* wait for students' connection */
	while(1)
	{
		printf("server is waiting for connection now...\n");
		client_sockfd = accept (server_sockfd, (struct sockaddr *)&client_address, &client_len);

		/* add this sockfd to broadcast fd[] */
		fd[clients] = client_sockfd;
		clients++;
		printf("client %d connected!\n", clients);
	}
}
