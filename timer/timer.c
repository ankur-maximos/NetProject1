#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

// Timer node structure
struct node {
	int key;
	double timeval;
	struct node *next;
};

typedef struct node Node;

// search for node already present returns true if present else false
int searchNode (Node **head, int key) {
	Node *temp = *head;
	int flag = 0;
	while(temp) {
		if(temp->key == key) {
			flag = 1;
			break;
		}		
	}
	return flag;
}

// update list
void updateList(Node *temp, double val,int flag) {
	while(temp && flag==1) {
		if(temp->timeval >= val)
			temp->timeval -= val;
		else break;
		temp = temp->next;
	}
	while(temp && flag==0) {
		temp->timeval += val;
		temp = temp->next;
	}
}

// insert into node 
void insertNode(Node **head, Node *node) {
	if(!(*head)) {
		*head = node;
	} else {
		Node *temp = *head;
		if(temp->timeval > node->timeval) {
			node->next = temp;
			*head = node;
			updateList(node->next,node->timeval,1);
		} else {
			Node *prev = temp;
			while(temp && node->timeval > temp->timeval) {
				node->timeval -= temp->timeval;
				prev = temp;
				temp = temp->next;
			} 
			node->next = prev->next;
			prev->next = temp;
			if(node) 
				updateList(node->next, node->timeval, 1);
		}
	}
}

// delete timer node
int deleteNode(Node **head,int key) {
	if(!(*head)) {
		return 0;
	} else {
		Node *temp = *head;
		if(temp->key == key) {
			*head = (*head)->next;
			updateList(*head,temp->timeval,0);
		} else {
			Node *temp1 = *head;
			while(temp && temp->key != key) {
				temp = temp->next;
			}
			while(temp1->next != temp) {
				temp1 = temp1->next;
			}
			temp1->next = temp->next;
			temp->next = NULL;
			updateList(temp1->next,temp->timeval,0);
		}
		free(temp);
	}
}

/* Printing out linked list */
void printList(Node **head) {
	Node *t = *head;
	while (t) {
		printf("Key->%d Time->%lf\n", t->key,t->timeval);
		t = t->next;
	}
}

/* Creating new node */
Node* getNode(int key,double timeval) {
	Node* newNode = (Node*)malloc(sizeof(Node));
	newNode->key = key;
	newNode->timeval = timeval;
	newNode->next = NULL;
	return newNode;
}


// main function to test calls
int main(int argc,const char *argv[]) { 
	// testing 
	Node *head = NULL; 

	// insert call
	int key = 1;
	double timeval = 10.4;
	insertNode(&head,getNode(key,timeval));

	//printList(&head);

	key = 1;
	timeval = 7.4;
	insertNode(&head,getNode(key,timeval));

	printList(&head);

	// starting server
	int sock;				// host listening socket
	int msgsock;			// 
	struct sockaddr_in timer_add;
	struct timeval *tv = NULL;
	fd_set readSockets;
	FD_ZERO(&readSockets);

	char *msg;

	if(argc<2) {
      printf("usage: %s <local-port> \n",argv[0]);
      exit(1);
  	}   

  	 /* reading port */
  	int port = atoi(argv[1]);

  	printf("TCP server waiting for remote connection from clients ...\n");

  	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
  		perror("error opening tcp socket");
  		exit(1);
  	}

  	/* construct name of socket to send to */
  	timer_add.sin_family = AF_INET;
  	timer_add.sin_addr.s_addr = INADDR_ANY;
  	timer_add.sin_port = htons(port);

  	/* bind socket name to socket */
  	if(bind(sock, (struct sockaddr *)&timer_add, sizeof(struct sockaddr_in)) < 0) {
    	perror("error binding stream socket");
    	exit(1);
  	}

  	listen(sock, 5);

  	/* accept a (1) connection in socket msgsocket */ 
  	if((msgsock = accept(sock, (struct sockaddr *)NULL, (int *)NULL)) == -1) { 
    	perror("error connecting stream socket");
    	exit(1);
  	}	 

  	msg = (char*)malloc(30);
  	/* first read "hello" from the client */
  	if(recv(msgsock, msg, 30, MSG_WAITALL) < 0){
    	perror("error reading on stream socket: error on reading file size");
    	exit(1);
  	}

  	printf("%s\n", msg);

  	FD_SET(msgsock,&readSockets);

  	for(;;) {

  		int temp = select(msgsock+1,&readSockets,NULL,NULL,tv);
  		if(temp == -1) {
  			perror("select");
  			exit(4);
  		}

  		if(temp == 0) {
  			//timeout
  			if(!head) {
  				printf("Timer list is empty\n");
  				exit(2);
  			} else {
  				Node *temp = head;
  				head = head->next;
  				free(temp);
  			}
  		} else {
  			//new request came
  			Node* node = (Node*)malloc(sizeof(Node));
  			if(recv(msgsock, msg, sizeof(Node), MSG_WAITALL) < 0){
    			perror("error reading on stream socket: error on reading file size");
    			exit(1);
  			}

  			printf("received key->%d timeval->%lf\n",node->key,node->timeval);
  			int found = searchNode(&head,node->key);
  			if(node->timeval == 0.0) {
  				//cancel timer
  				if(found == 0) {
					printf("Key not found \n");					
  				} else {
  					deleteNode(&head,node->key);
  				}
  			} else {
  				//insert timer
  				if(found == 0) { 			
  					insertNode(&head,node);
  					// updating timeout value
  					long msec = (long)node->timeval;
  					double usec = ((int)msec - node->timeval) * 1000;
  					if(tv) {
  						tv->tv_sec = (long)msec * 1000;
  						tv->tv_usec = (long)usec;
  					} else {

  					}
  				} else {
  					printf("duplicate key..Cannot be added\n");
  				}
  			}
  		}

  	}

	return 0;
}