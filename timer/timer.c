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
void updateList(struct node *temp, int val,bool flag) {
	while(temp && flag) {
		temp->timeval -= val;
	}
	while(temp && !flag) {
		temp->timeval += val;
	}
}

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
			updateList((*head)->next,val,true);
		} else {
			struct node *prev = temp1;
			do{
				val -= temp1->timeval;
				temp1 = temp1->next;
			} while(temp1 && val >= temp1->timeval);
			while(prev->next != temp1) prev = prev->next;
			temp->next = prev->next;
			prev->next = temp;
			if(temp) 
				updateList(temp1, val, true);
		}
	}
}

//delete a node
bool deleteNode(struct node **head,int key) {
	if(!(*head)) {
		return false;
	} else {
		struct node *temp = *head;
		if((*temp)->key == key) {
			*head = (*head)->next;
			updateList(*head,temp->timeval,false);
			free(temp);
		} else {
			struct node *temp1 = *head;
			while(temp && temp->key != key) {
				temp = temp->next;
			}
			while(temp1->next != temp) {
				temp1 = temp1->next;
			}
			temp1->next = temp->next;
			temp->next = NULL;
			updateList(temp1->next,temp->timeval,false);
			free(temp);
		}
	}
}

// main function to test calls
void main() {

}