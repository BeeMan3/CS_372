John Behman
CS 372
Project 2

This project opens a server and allows the client that was written to connect to the server. 
Via command line the user can designated if they would like to recieve a text file from the 
servers directory, or the contents within the servers directory. Below are the steps for operation:

Note: This project was developed and tested entirely on flip 2. The client and server must be in
seperate directories for correct operation.

1) Unpack the zip file
2) Verify that the client is in one directory, and the server and Makefile are in another
3) Compile the server by typing "make"
4) run the server by typing ./ftserver [PORT #]
5) Then in another instance on flip 2, run the client. The options you have are:
	-l: python ftclient.py [SERVER NAME] [SERVER PORT] -l [DATA PORT]
	ex: python ftclient.oy flip2 30021 -l 30100

	-g: python ftclient.py [SERVER NAME] [SERVER PORT] -g [FILE] [DATA PORT]
	ex: python ftclient.py flip2 30021 -g test.txt 30100

6) The task should then be completed based on what was designated and should be seen in the client instance

Note: Sometimes when connecting and performing the -l command the server throws an error. Just repeat the process
using a new port # and it should work. I have also supplied a sample test file I used for testing.
Thanks!
