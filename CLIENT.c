
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
int sock;//we declare the socket variable
int N;//we declare the size of the array to be sent
struct sockaddr_un server;//we create a server struct
char buffer[1024];//we create a buffer to store the server's response into


sock = socket(AF_UNIX, SOCK_STREAM, 0);//we create the server socket
if (sock < 0) {//if the value returned by the socket creation function is less than 0 then we have an error in establishing a server
	perror("opening stream socket");
	exit(1);
}
server.sun_family = AF_UNIX;
strcpy(server.sun_path,"socket");//we set the server path to the path we set on the server file.In this case we set the NAME=socket
if (connect(sock, (struct sockaddr *) &server,
sizeof(struct sockaddr_un)) < 0) { //we connect to  the server using the socket.If connect returns a value less than 0 then we have an error establishing a connection to the server
	close(sock);
	perror("connecting stream socket");
	exit(1);
}

while(1){//infinite loop to continuously send new requests
	printf("Please give the size of the array youd like to enter!");
	scanf("%d",&N);
	int arr[N];
	for(int i = 0 ; i<N;i++){
		printf("Please give the %d int:",i);
		scanf("%d",&arr[i]);
	}

	if (write(sock, arr, sizeof(arr)) < 0){ //we write the array of numbers to the socket so that it gets sent to the server.If write returns a value less than 0 then we have an error writing to the server
		perror("writing on stream socket");
		close(sock);
	}
	if(read(sock,buffer,sizeof(buffer)-1)>0){ //we read the server's response from the socket and store it to the buffer.The size of the data received is the size of the buffer minus the terminating character (-1).If the value returned by read is greater than 0 then we have succesfuly read from the server and can print the result.
		printf("Received from server: %s\n", buffer);		
	
	}else{//else if its <= 0 we have an error reading from the socket
		perror("reading from stream socket");
	}
	printf("Would you like to continue?(1 for yes 0 for No");
	scanf("%d",&N);
	while(N!=1 && N!=0){
		printf("Would you like to continue?(1 for yes 0 for No)");
		scanf("%d",&N);
	}

	if(N==0){
		close(sock);//we close the socket before exiting so that it doesnt create issues with new clients
		exit(0);

	}
}

}



