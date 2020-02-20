//John Behman
//chatclient.c
//Chat client that communicates back and forth with a server
//CS 372 Project 1
//7/28/2019

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char *argv[]){
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	int numbytes;
	int con;
	char handle[12];
	char server[12];

	//verify that there are three arguments entered on the command line
	if (argc != 3){ 
	    fprintf(stderr, "usage: ./chatclient server port\n");
	    exit(1);
	}

	printf("Please enter a user handle (maximum 12 characters):");
	    scanf("%s", handle); //user enters handle
	
	
	memset(&hints, 0, sizeof(hints)); //next three lines are reffered from Beej's Guide (Page 32) for setting up connection
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0){ //reffered to from Beej's Guide (page 32)
	    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	    exit(1);
	}

	else ;

	if((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1){ //reffered to from Beej's Guide (page 32)
	    fprintf(stderr, "ERROR! Socket not created \n");
	    exit(1);
	}

	else ;

	if((con = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen)) == -1){ //connection verification, reffered to from Beej's Guide (page 31)
	    fprintf(stderr, "Connection error! \n");
   	    exit(1);
	}

	else ;

	//data used for chat
	int sending = send(sockfd, handle, strlen(handle), 0);
	int recieve = recv(sockfd, server, 12, 0);
	char data_output[500]; 
	memset(data_output, 0, sizeof(data_output));
	fgets(data_output, 500, stdin);
	char data_in[500];
	memset(data_in, 0, sizeof(data_in));
	int comp;
	int input;
	int output;
	int check;

	while(1){
	    printf("%s>", handle); //print user field
	    fgets(data_output, 500, stdin); //take user input for sending to server
	    comp = strcmp(data_output, "\\quit\n"); //check if user wnats to quit, if so quit the program
	    if(comp == 0) exit(0);

	    data_output[strcspn(data_output, "\n")] = 0; //delete new line character to avoid sending it to server

	    output = send(sockfd, data_output, 500, 0); //assign output to the output data that is sent to the server, reffered to Beej's Guide (page 34)

	    if(output == -1){
		fprintf(stderr, "ERROR! Data was not sent\n");
		exit(1);
	    }

	    input = recv(sockfd, data_in, 500, 0); //input data return value assigned to variable. Also taken from Beej's guide (page 34)


	    if(input == 0){ 
		printf("program closed by server \n");
		exit(1);
	    }

	    if(input == -1){
		fprintf(stderr, "ERROR! Data was not recieved\n");
		exit(1);
	    }
	
	    else printf("%s>%s\n", server, data_in); //print servers user handle and data sent from the server

	    check = strcmp(data_in, "\\quit"); //if the server send "\quit" close the client
	    if(check == 0) exit(0);

	    else ;
	    
	    //reset input and output values
	    memset(data_in, 0, sizeof(data_in)); 
	    memset(data_output, 0, sizeof(data_output));

	}

	//free the address information and close sockfd. Reffered to from Beej's Guide (page 34)
	freeaddrinfo(servinfo);
	close(sockfd);
	printf("Client is now closed\n");
}
