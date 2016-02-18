#include<stdio.h>

// Timer node structure
struct node {
	int key;
	double timeval;
	struct node *next;
}

// search for node already present returns true if present else false
bool searchNode (struct node **head, int key) {
	struct node *temp = *head;
	bool flag = false;
	while(temp) {
		if(temp->key == key) {
			flag = true;
			break;
		}		
	}
	return flag;
}

// update list


// insert into node 
void insertNode(struct node **head, struct node *temp) {
	if(!(*head)) {
		*head = *temp;
	} else {
		int val = temp->timeval;
		struct node *temp1 = *head;
		if(temp1->timeval > val) {
			temp->next = temp1;
			*head = temp;
			updateList((*head)->next,val);
		} else {
			while(val > temp1->timeval) {
				val -= temp1->timeval;
				temp1 = temp1->next;
			}
			
		}
		while(temp) {
			if()
		}
	}
}

// main function to test calls
void main() {

}