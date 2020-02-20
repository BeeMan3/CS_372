#John Behman
#ftclient.py
#This program connects to a server and recieves a designated text file or the directory contents from the server in question
#CS 372
#8/14/2019

import sys
from socket import *
import time

def makeSocket():
    sock = socket(AF_INET, SOCK_STREAM);
    if (sys.argv[3] == "-l"):
	sock.bind(('', int(sys.argv[4]))) #bind socket to the adsress (arg 4 for -l) https://docs.python.org/3/library/socket.html

    elif (sys.argv[3] == "-g"):
	sock.bind(('', int(sys.argv[5]))) #bind socket to the address (arg 5 for -l) https://docs.python.org/3/library/socket.html

    #again, socket connection functions referred to from https://docs.python.org/3/library/socket.html 
    sock.listen(1)
    client_data, address = sock.accept()
    return client_data

	

if __name__ == "__main__":
   
    #if statements that verify that the arguments entered are correct
    if (len(sys.argv) != 5 and len(sys.argv) != 6):
	print "The application must recieve 5 or 6 arguments! (5 using -l, 6 using -g)"
	exit(1)

    if (sys.argv[3] != "-l" and sys.argv[3] != "-g"):
	print "Must use -g or -l for the command argument"
	exit(1)

    if (sys.argv[3] == "-l" and (int(sys.argv[4]) < 1024 or int(sys.argv[4]) > 65535)):
	print "The data port number is incorrect"
	exit(1)

    if (sys.argv[3] == "-g" and (int(sys.argv[5]) < 1024 or int(sys.argv[5]) > 65535)):
	print "The data port number is incorrect"
	exit(1)

    if (int(sys.argv[2]) < 1024 or int(sys.argv[2]) > 65535):
	print "The server port number is incorrect"
	exit(1)

    if (sys.argv[1] != "flip1" and sys.argv[1] != "flip2" and sys.argv[1] != "flip3"):
	print "Must enter flip1, flip2, or flip 3 for the host"
	exit(1)

    #establishes connection with the server using the socket function (https://docs.python.org/3/library/socket.html)
    client_server = socket(AF_INET, SOCK_STREAM) 
    server = sys.argv[1]
    portnum = int(sys.argv[2])
    client_server.connect((server, portnum))

    if (sys.argv[3] == "-l"): #if command is -l, sends l to server
    	client_server.send(sys.argv[4])
	client_server.recv(1024)
	client_server.send("l")
	client_server.recv(1024)

    if (sys.argv[3] == "-g"): #if command is -g sends g to server
    	client_server.send(sys.argv[5])
	client_server.recv(1024)
	client_server.send("g")
	client_server.recv(1024)

    #obtain host name then send to server
    host = gethostname()
    IP = gethostbyname(host) 
    client_server.send(IP)
    
    hit = client_server.recv(1024) 

    if hit == "bad":  #if response is bad, server had issues with clients connection
    	print "The server recieved an invalid command"
	exit(1);

    if sys.argv[3] == "-g":
    	client_server.send(sys.argv[4])
	hit = client_server.recv(1024)
	if hit != "Detected file": 
	    print "Server was unable to locate the file"
	    

    client_data = makeSocket() #call socket create

    if(sys.argv[3] == "-l"): #if command is -l, recieve the directory information
	direc = client_data.recv(100)

	while direc != "done":
	    print direc
	    direc = client_data.recv(100)

    if(sys.argv[3] == "-g"): #if the command is -g, write the data recieved to a text file created witht he same name
	doc = open(sys.argv[4], "w")
	transfer = client_data.recv(1000)
	
	while "COMPLETE" not in transfer: #continue while the COMPLETE flag does not appear in the buffer being recieved
	    doc.write(transfer)
	    transfer = client_data.recv(1000)

    if(sys.argv[3] == "-g"): #close the document once completed if the command was -g
	doc.close()

	lines = open(sys.argv[4]).readlines()		#used to delete the last line of the text file recieved as
    	open(sys.argv[4], 'w').writelines(lines[:-1])   #there en extra line being generated filled with garbage 

    client_data.close(); 				#close the socket https://docs.python.org/3/library/socket.html 
