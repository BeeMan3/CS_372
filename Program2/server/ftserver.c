//John Behman
//ftserver.c
//Server side of project 2 that sends a file or directory listing to the client that connects to this port
//CS 372
//8/14/2019

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

//creates a buffer to store file names that are in the servers directory
char ** create_buffer(int len){      
	char ** buffer = malloc(len*sizeof(char *));
	int i;
	for(i = 0; i < len; i++){
		buffer[i] = malloc(100*sizeof(char));
		memset(buffer[i],0,sizeof(buffer[i]));
	}
	return buffer;
}

//deletes the buffer containing the file names in the directory
void delete_buffer(char ** buffer, int len){
	int i;
	for (i = 0; i < len; i++){
		free(buffer[i]);
	}
	free(buffer);
}

//This function refers to Beej's guide (Located on many pages including page 32)
int socket_create(struct addrinfo * res){              
	int sockfd;                                                     
	if ((sockfd = socket((struct addrinfo *)(res)->ai_family, res->ai_socktype, res->ai_protocol)) == -1){
		printf("ERROR! Socket could nto be created\n");
		exit(1);
	}
	return sockfd;
}

//This function is utilized the same as the one above for the connection of the client and the server
void socket_link(int sockfd, struct addrinfo * res){          
	int status;
	if ((status = connect(sockfd, res->ai_addr, res->ai_addrlen)) == -1){
		fprintf(stderr, "Error in connection.\n");
		exit(1);
	}
}

//This function creates the IP information using steps from Beej's Guide
struct addrinfo * ip_create(char* IP, char* port){     
	int status;                                                                                                 
	struct addrinfo hints;
	struct addrinfo * servinfo;
	
	memset(&hints, 0, sizeof hints);  //from page 17 to set up the struct
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;

	if((status = getaddrinfo(IP, port, &hints, &servinfo)) != 0){ //crate the linked list
		printf("ERROR! Incorrect port entered\n"); //seen on many pages of Beej's guide such as page 18
		exit(1);
	}
	
	return servinfo;
}

//This function finds all files in the servers directory.
int file_check(char** files){  //Referred to helpful link: https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
	DIR* d;                                 
	struct dirent * dir;
	d = opendir(".");
	int i = 0;
	if (d){
		while ((dir = readdir(d)) != NULL){    //loop through directory to find all files
			if (dir->d_type == DT_REG){		  
				strcpy(files[i], dir->d_name); //instead of listing like the reference does, copy the name to the stuct
				i++;
			}
		}
		closedir(d);
	}
	return i;
}

//Accepts new data connection. Most of this is referenced from Beej's Guide 
void accept_data_connect(int new_fd){
    char * good = "ok";
    char * bad = "bad";
    char data_port[100];
    char client_command[100];
    char IP[100];

    memset(data_port, 0, sizeof(data_port));
	memset(client_command, 0, sizeof(client_command));
	memset(IP, 0, sizeof(IP));
    
	recv(new_fd, data_port, 100, 0);      //recieve sata port value from client
    send(new_fd, good, strlen(good), 0); 

    recv(new_fd, client_command, 100, 0); //recieve command from client
    send(new_fd, good, strlen(good), 0);

    recv(new_fd, IP, 100, 0);			  //recieve IP from client

    printf("Connection obtained from client located at %s\n", IP);
    char c = client_command[0]; //take first char from command (the letter)

    switch(c){ //switch statement to know what to do based on the command sent
	//case for command -l
	case 'l':
		send(new_fd, good, strlen(good),0);
		printf("Client requested files within server directory \n");
		
		char** direc_file = create_buffer(500);
		
		int direc_file_count = file_check(direc_file);
		
		struct addrinfo * d = ip_create(IP, data_port); //save client IP information into struct
		int direc_socket = socket_create(d); 			//obtain socket file descriptor
		socket_link(direc_socket, d);					//link socket after creating socket from sockfd
	
		int i ;
		for (i = 0; i < direc_file_count; i++){                     
			send(direc_socket, direc_file[i], 100 ,0);  //send directory data to client
		}
	
		char* fin = "done";
		send(direc_socket, fin, strlen(fin), 0); 		//send complete string
	
		close(direc_socket); 							//close directory socket
		freeaddrinfo(d);	 
		delete_buffer(direc_file,500); 					//delete buffer
	break;
	//case for command -g
	case 'g':
		send(new_fd, good, strlen(good), 0);
		
		char file[100];
		memset(file, 0, sizeof(file));
		recv(new_fd, file, sizeof(file) - 1, 0);
		printf("Client requested file %s \n", file);
		
		char** file_buffer = create_buffer(500);
		int file_count = file_check(file_buffer);		//verify file exists in server directory
		int detect = 0;            
		int j;

		for (j = 0; j < file_count; j++){             	//check all files ot see if the file exists
			if(strcmp(file_buffer[j], file) == 0){		//assert detect if the file is found
				detect = 1;
			}
		}
	
		if(detect){ 									//if the file exists
			printf("File found, sending %s to client\n", file);
			char * detect_file = "Detected file";
			send(new_fd, detect_file, strlen(detect_file), 0); //Inform client that the file was detected
			
			char new_file[100];
			memset(new_file, 0, sizeof(new_file));		
			strcpy(new_file, "./");
			char * end = new_file + strlen(new_file);
			end += sprintf(end, "%s", file);
			
			int k = 500000;
			int l = 0;
			for(l; l < k; l++){ //wait for a short bit before calling socket functions
				l = l;
			}
			struct addrinfo * f = ip_create(IP, data_port);  //similar to previous case, save IP info to struct
			int file_socket = socket_create(f);              //again similar to previous case obtain the socket file drescriptor
			socket_link(file_socket, f);					 //then link the socket with obtained sockfd
	
			char buffer[2000];                               //buffer to transfer file	
			memset(buffer, 0, sizeof(buffer));
	
			int fd = open(file, O_RDONLY);                   //open the file (https://linux.die.net/man/3/open)
			while (1) {
				int B_count = read(fd, buffer, sizeof(buffer) - 1);  //helpful reference (https://stackoverflow.com/questions/19769542/reading-from-file-using-read-function)
				if (B_count == 0) 
				break;

				if (B_count < 0) {
					printf("Error. Cannot read file.\n");
					return;
				}

				void* stop = buffer;  
				while (B_count > 0) { //while the buffer has data, send it to the client
					int B_write = send(file_socket, stop, sizeof(buffer),0);
					if (B_write < 0) {
						printf("Error in writing\n");
						return;
					}
					B_count -= B_write;
					stop += B_write;
				}	
		
				memset(buffer, 0, sizeof(buffer));  //clear the buffer
			}
	
			memset(buffer, 0, sizeof(buffer));  	//clear the buffer again after the loop is complete
			strcpy(buffer, "COMPLETE");				
			send(file_socket, buffer, sizeof(buffer),0); //send with complete tag at end
	
			close(file_socket);          			//close file socket
			freeaddrinfo(f);

		}        
        	else{			
			printf("Could not find file. Sending error to client.\n");
			char * missing = "Unable to locate file";
			send(new_fd, missing, 100, 0);
		}
		delete_buffer(file_buffer, 500);
	
	break;

	default: //default case send bad tag to client meaning command recieved is incorrect (not -l or -g)
	send(new_fd, bad, strlen(bad), 0); 

    }
  
    printf("Server waiting for connection from client\n");

}

int main(int argc, char *argv[]){
    int rv, new_fd, port;
    int sockfd;
    struct addrinfo hints, *servinfo;
    struct sockaddr_storage their_addr;
    socklen_t addr_size;


    if(argc =! 2){ //if there are nto 2 arguments exit
	printf("Wrong number of arguments! Please use 2 arguments -> ./ftserver [port #] \n");
	exit(1);
    }
    else ;

    port = atoi(argv[1]);
    if(port > 65535 || port < 1024){ //verify the port is between the valid range
	printf("Port number is not withn a valid range");
	exit(1);
    }
    else ;

    memset(&hints, 0, sizeof hints); //next three lines are reffered from Beej's Guide (Page 32) for setting up connection
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0){ //reffered to from Beej's Guide (page 32)
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
	exit(1);
    }
    else ;

    if((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1){ //reffered to from Beej's Guide (page 32)
	fprintf(stderr, "ERROR! Socket not created \n");
	exit(1);
    }
    else ;

    if(bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1){ //reffered from Beej's Guide (page 20)
	close(sockfd);
	fprintf(stderr, "ERROR! Unable to bind socket \n");
	exit(1);
    } 
    else ;

    if(listen(sockfd, 5) == -1){ //reffered from Beej's Guide (page 22)
	close(sockfd);
	fprintf(stderr, "ERROR! Unable to listen \n");
	exit(1);
    }	
    else ;

    printf("Ready for client connection\n");    
    while(1){
	addr_size = sizeof their_addr;
	new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size); //accept client connection - Beej's Guide (page 22)
	if(new_fd == -1){
		continue;
	}
	accept_data_connect(new_fd); //call client acception function that deals with the new file descriptor
    }
    close(new_fd);			//close the new file descriptor 
	freeaddrinfo(servinfo); //free the address information. Reffered to from Beej's Guide (page 34)  
}

