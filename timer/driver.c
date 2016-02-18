#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

/*
void starttimer() {

}

canceltimer() {

} */

int main(int argc,const char *argv[]) {

	int sock;
	struct sockaddr_in timer_add;
	struct hostent *hp;

	if (argc!=3){
        printf("Usage: %s <remote-IP> <remote-port> \n",argv[0]);
        exit(1);
    }

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    	perror("error opening tcp socket");
    	exit(1);
    }

    hp = gethostbyname(argv[1]);
  	if(hp == 0) {
    	fprintf(stderr, "%s: unknown host\n", argv[1]);
    	exit(2);
  	}

  	/* reading port argument */
  	int port = atoi(argv[2]);

  	bcopy((void *)hp->h_addr, (void *)&timer_add.sin_addr, hp->h_length);
  	timer_add.sin_family = AF_INET;
   	timer_add.sin_port = htons(port); /* fixed by adding htons() */

   	/* establish connection with timer */
  	if(connect(sock, (struct sockaddr *)&timer_add, sizeof(struct sockaddr_in)) < 0) {
	    close(sock);
	    perror("error connecting stream socket");
	    exit(1);
  	}

  	char msg[30] = "Sending timer informations...";

  	/* first send file size */
  	if(write(sock, msg, sizeof(char) * 30) < 0) {
    	perror("error writing on stream socket: error on sending packet");
    	exit(1);
   	}

	return 0;
}
