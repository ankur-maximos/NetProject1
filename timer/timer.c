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
	int sock;
	int msgsock;
	struct sockaddr_in sin_addr;

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
  	sin_addr.sin_family = AF_INET;
  	sin_addr.sin_addr.s_addr = INADDR_ANY;
  	sin_addr.sin_port = htons(port);

  	/* bind socket name to socket */
  	if(bind(sock, (struct sockaddr *)&sin_addr, sizeof(struct sockaddr_in)) < 0) {
    	perror("error binding stream socket");
    	exit(1);
  	}

  	/* accept a (1) connection in socket msgsocket */ 
  	if((msgsock = accept(sock, (struct sockaddr *)NULL, (int *)NULL)) == -1) { 
    	perror("error connecting stream socket");
    	exit(1);
  	}	 

  	/* first read "hello" from the client */
  	if(recv(msgsock, &network_byte_order, sizeof(uint32_t), MSG_WAITALL) < 0){
    	perror("error reading on stream socket: error on reading file size");
    	exit(1);
  	}

	return 0;
}