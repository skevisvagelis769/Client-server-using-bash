
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NAME "socket" //we declare the name of the server

int main(int argc, char *argv[]) {
	int sock, msgsock, rval,sum=0; //we declare the variables to which we will assign the socket,
	//the message that'll be accepted by the server,the return value that we will read from the client,
	//and the sum to be used for the calculation of the average
	double avg;//the average
	char response[64];//we store the server response thatll be sent to the client
	struct sockaddr_un server;//we declare ther server socket adder struct
	int buf[1024];//we declare the buffer that will store the clien's request
	sock = socket(AF_UNIX, SOCK_STREAM, 0);//we create the socket
	if(sock<0){ //if the value returned by the socket function is less than 0 then we throw and error
		perror("opening stream socket");
		exit(1);
	}
	server.sun_family = AF_UNIX;
	strcpy(server.sun_path, NAME); //we set the server path to NAME (socket),with strcpy because
	//we cant implicitly set server.sun_path = NAME
	unlink(NAME);//we unlink the socket name so it can be used by another client
	if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) { //we bind the socket to the server struct,if this is 0 then ther was an error binding the socket to the server
   	 	perror("binding stream socket");
    	exit(1);
	}
	listen(sock, 5);// we listen for requests in the socket, we set the limit to the number of simultaneous requests to 5
	for (;;) {//we will loop over the following code infinitely so that we can keep processing clients' reqeusts
		msgsock = accept(sock, NULL, NULL);//we accept the traffic coming into the socket, and we set msgsock as its return value
		if (msgsock == -1)//if accept returned -1 then we have an error accepting the request
		perror("accept");
		else do{ //if the returned value of accept is not -1 then we can continue to processing the request
			sum=0;//we set the sum equal to 0 in each loop so that we can calculate the average of the specific client
			avg=0;//in a similar way we set the average to 0
			bzero(buf,sizeof(buf));//we clear the buffer
			if((rval=read(msgsock,buf,1024))<0) perror("reading stream message");//we read the client's message in msgsock and we store it to the buffer and then store the return value of read to rval.If this value is less than 0 then theres an error reading the message
				int size = rval/sizeof(int); //we calculate the size of the array that the client sent.This is done by deviding rval,which is the number of bytes that we have received ,by the size of an int,therefore getting how many numbers there are in the array
				for(int i = 0;i<size;i++){//we loop over all items in the array
					sum = sum+buf[i];//and we calculate the sum of all items
				}
				if(size !=0){//if the size of the array is not 0
					avg = (double)sum/size;//then we can calculate the average
				}
				if(avg >20){//if the average is greater than 20
					sprintf(response, "Sequence Ok: %.2f", avg);//then by using sprintf we can "print" into response the message Sequence Ok: avg
				}else{//if avg is equal or less than 20
					strcpy(response,"Check Failed");//we print into the response the message Check Failed
				}
				write(msgsock, response, strlen(response) + 1);//we write the response into the message socket ,with a length equal to the size of the response array + 1 to account for the terminating character.This way we return the response to the client
		}while(rval>0);//we repeat this process while rval>0, because when rval <= 0 then the client has terminated the connection
		close(msgsock); //when the connection has been terminated we close the message socket
	}	
	close(sock);//we close the socket
	unlink(NAME);//we unlink the name from the socket
}
    
