John Behman
CS 372 Project 1
7/28/2019

Instructions:
1) open up two instances of the terminal on server flip2 and unzip the folder. (the code was written and has been tested on flip2)    
2) compile the client by running gcc -o chatclient chatclient.c
3) on the other terminal instance, run the server using command "python chatserve.py [PORT]" (ex. python chatserve.py 30021)
4) run the client in the first terminal instance with command "./chatclient flip2 [PORT]" using the same port as the previous step (ex. ./chatclient flip2 30021)
5) select the user handle for both the client and the server
6) on the client, send a message between 1 and 500 characters
7) verify that the message was recieved on the server and then send a message back
8) type \quit on either the client or server to close the client and reset the server

Thanks
