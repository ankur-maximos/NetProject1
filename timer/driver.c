#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

voif starttimer() {

}

canceltimer() {

}

int main(int argc,const char *argv[]) {

	int socket;
	struct sockaddr_in timer_add;

	if (argc!=3){
        printf("Usage: %s <remote-IP> <remote-port> \n",argv[0]);
        exit(1);
    }

    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
    	perror("error openting datagram socket");
    	exit(1);
    }

    hp = gethostbyname(argv[1]);
  	if(hp == 0) {
    	fprintf(stderr, "%s: unknown host\n", argv[1]);
    	exit(2);
  	}

  	/* reading port argument */
  	int port = atoi(argv[2]);

  	bcopy((void *)hp->h_addr, (void *)&sin_addr.sin_addr, hp->h_length);
  	sin_addr.sin_family = AF_INET;
   	sin_addr.sin_port = htons(port); /* fixed by adding htons() */

   	/* establish connection with timer */
  	if(connect(sock, (struct sockaddr *)&sin_addr, sizeof(struct sockaddr_in)) < 0) {
	    close(sock);
	    perror("error connecting stream socket");
	    exit(1);
  	}

  	/* first send file size */
  	if(write(sock, &network_byte_order, sizeof(uint32_t)) < 0) {
    	perror("error writing on stream socket: error on sending file size");
    	exit(1);
   	}



	return 0;
}
