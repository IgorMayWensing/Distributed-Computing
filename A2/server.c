#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr

char * toArray(int number) {
    int n = log10(number) + 1;
    int i;
    char *numberArray = calloc(n, sizeof(char));
    for (i = n-1; i >= 0; --i, number /= 10)
    {
        numberArray[i] = (number % 10) + '0';
    }
    return numberArray;
}

void func(int sockfd)
{
	char buff[MAX];
	int n;
	// infinite loop for func
	for (;;) {
		bzero(buff, MAX);

		// read the message from client and copy it in buffer
		read(sockfd, buff, sizeof(buff));
		// print buffer which contains the client contents
		printf("From client: %s\t", buff);

		//****************************************************
		char oper1[MAX];		
		char oper2[MAX];
		int op1, op2, result;
		int a = 0;
		int b = 0;
		bool isFirstOp = true;

		for( int i = 2; i < MAX-1; i++ ){
      		if (buff[i] == '#') {
				isFirstOp = false;
			}
			if (isFirstOp) {
				oper1[a++] = buff[i];
			} else if(buff[i] != '#'){
				oper2[b++] = buff[i];
			}
   		}
		   
		op1 = atoi(oper1);
		op2 = atoi(oper2);

		if (buff[0]=='/') {
			result = op1/op2;
		}else if (buff[0]=='*') {
			result = op1*op2;
		}else if (buff[0]=='+') {
			result = op1+op2;
		}else if (buff[0]=='-') {
			result = op1-op2;
		}
		printf("\nop1: %d \n", op1);
		printf("op2: %d \n", op2);
		printf("result: %d \n", result);
		//****************************************************
		bzero(buff, MAX);
		n = 0;

		
		write(sockfd, toArray(result), sizeof(buff));
	}
}



// Driver function
int main()
{
	int sockfd, connfd, len;
	struct sockaddr_in servaddr, cli;

	// socket create and verification
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		printf("socket creation failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully created..\n");
	bzero(&servaddr, sizeof(servaddr));

	// assign IP, PORT
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		printf("socket bind failed...\n");
		exit(0);
	}
	else
		printf("Socket successfully binded..\n");

	// Now server is ready to listen and verification
	if ((listen(sockfd, 5)) != 0) {
		printf("Listen failed...\n");
		exit(0);
	}
	else
		printf("Server listening..\n");
	len = sizeof(cli);

	// Accept the data packet from client and verification
	connfd = accept(sockfd, (SA*)&cli, &len);
	if (connfd < 0) {
		printf("server acccept failed...\n");
		exit(0);
	}
	else
		printf("server acccept the client...\n");

	func(connfd);

	close(sockfd);
}
