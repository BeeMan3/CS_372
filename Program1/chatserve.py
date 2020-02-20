#John Behman
#chatserve.py
#This program is a chat server that allows a client to connect to it and communicate back and forth with the client
#CS 372 Project 1
#7/28/2019


from socket import *
import sys

if __name__ == "__main__":
    
    if len(sys.argv) != 2: #verify that the correct # of arguments were entered
	print "usage: python chatserve.py port"
	exit(1)

    port = sys.argv[1] #assign a value to designated port from command line
    sock = socket(AF_INET, SOCK_STREAM) #this was implemented from here: www.tutorialspoint.com/unix_sockets/socket_core_functions

    sock.bind(('', int(port))) #this function was also reffered to from the link above

    sock.listen(1) #again reffered to the link above

    handle = ""
    while len(handle) == 0 or len(handle) > 12: #if the user handle entered is of the correct size
	handle = raw_input("Enter a username that is 12 characters or less:")
	print "server is enabled"

    while 1:
	link, addr = sock.accept()  
	print "connected to address {}".format(addr) #print address of client connecting

	client = link.recv(1024) #recieve clients name from the client
	link.send(handle) #send servers handle to the client

	message = ""
	while 1:
	    recieve = link.recv(501)[0:-1] #data sent from client is recieved

	    if recieve == "": #if blank, conneection was lost
		print "end of connection, waiting for new user"
		break

	    print "{}>{}".format(client, recieve) #otherwise, print the data sent from the client user
	    data_out = ""
	    if len(data_out) <= 0 or len(handle) > 500: #if the output data is in the neccesarry size range
	    	sending = raw_input("{}>".format(handle)) #input data to send to client

	    if sending == "\quit": #if sending quit, close server sonnection and wait for new user
		link.send(sending)
		print"end of connection, waiting for new user"
		break
	    link.send(sending) #send data from server to client

    link.close(); #close the connection



