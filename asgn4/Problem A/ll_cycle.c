#include <stddef.h>
#include "ll_cycle.h"
#include <stdlib.h>

int ll_has_cycle(node *head) {
	/* TODO: Implement ll_has_cycle */
	// Implement a fast and slow node that we will be moving throughout the linked list
	struct node *fast = head;
	struct node *slow = head;
	// While loop that makes sure the fast node isn't NULL nor points to NULL
	while(fast != NULL && fast->next != NULL){
		// Move fast node two nodes and slow node one node
		fast = fast->next->next;
		slow = slow->next;
		// If fast node is equal to slow node than return 1
		if(fast == slow){
			return 1;
		}
		

	}
	// If the fast node is NULL or points to NULL return 0
	return 0;
	}
