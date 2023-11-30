#include <stdio.h>
#include <stdlib.h>
#include "hashtable.h"
#include "set.h"
#include <stdbool.h>
#include <string.h>

// Helper hash function
int hashfunction(const char *key, const int num_slots){
        //If key is NULL or num_slots is greater than 0 return -1
        if(key != NULL && num_slots > 0){
                //Find added ascii value of all characters in key
                int hash = 0;
                while(*key){
                        hash += (int)(*key);
                        key++;
                }
                //Return remainder of hash divided by num_slots
                return hash % num_slots;
        }else{
		printf("invalid inputs");
                return -1;
        }
}

// Helper function to find the num_slots in a hashtable
int find_num_slots(hashtable_t *ht){
        // Create temp variable for iteration
        hashtable_t *temp = ht;
        // Num_slots to increase
        int num_slots = 0;
        //Iterate through node till it reaches NULL and add num_slots by 1 each time
        while(temp != NULL){
                num_slots++;
                temp = temp->next;
        }

        // Return Num_slots
        return num_slots;
}



hashtable_t *hashtable_new(const int num_slots) {
    // Allocate memory for the head node of the hashtable
    hashtable_t *hashtable = (hashtable_t*) malloc(sizeof(hashtable_t));

    // Check for memory allocation failure for the head node
    if (hashtable == NULL) {
        // Memory allocation failed, return NULL
        return NULL;
    }

    // Initialize the 'set' and 'next' pointers of the head node to NULL
    hashtable->set = set_new();
    hashtable->next = NULL;

    // Create a temporary pointer to iterate and add additional nodes
    hashtable_t *temp = hashtable;
    for (int i = 1; i < num_slots; i++) {
        // Allocate memory for a new hashtable node
        hashtable_t *new_node = (hashtable_t*) malloc(sizeof(hashtable_t));

        // Check for memory allocation failure for the new node
        if (new_node == NULL) {
            // On failure, free all previously allocated nodes to prevent memory leak
            hashtable_t *curr = hashtable;
            while (curr != NULL) {
                hashtable_t *next = curr->next;
                free(curr); // Free current node
                curr = next; // Move to the next node
            }
            // Return NULL after cleaning up
            return NULL;
        }

        // Initialize the new node's 'set' and 'next' pointers
        new_node->set = set_new();
        new_node->next = NULL;

        // Link the new node to the list
        temp->next = new_node;
        // Move the temporary pointer to the new node
        temp = temp->next;
    }

    // Return the head node of the hashtable
    return hashtable;
}


//Insert item into hash table, if unsuccessful return false else return true
bool hashtable_insert(hashtable_t *ht, const char *key, void *item){
	// If any parameter is NULL, return false
	if(ht == NULL || key == NULL || item == NULL){
		return false;
	}
	// Create variable num_slots using helper function in hashtable.h
	const int num_slots = find_num_slots(ht);
	// Find hash using helper function in hashtable.h
	int hash = hashfunction(key, num_slots);
	//Create temporary hashtable for iteration
	hashtable_t *temp = ht;
	//Go to the right node using the hash
	for(int i = 0; i < hash; i++){
		temp = temp->next;
	}
	//Insert item into set within hashtable node and store as variable success
	bool success = set_insert(temp->set, key, item);
	// If set insertion failed for whatever reason, this function will return false otherwise true
	return success;	
	
}

//Return the item with the key
void *hashtable_find(hashtable_t *ht, const char *key){
	//If either parameter is NULL return NULL
	if(ht == NULL || key == NULL){
		return NULL;
	}
	// Find both num_slots and hash using helper functions in hashtable.h
	const int num_slots = find_num_slots(ht);
	int hash = hashfunction(key, num_slots);
	//Create temporary node for iteration
	hashtable_t *temp = ht;
	// Iterate through hashtable using hash and make temp hashtable node equal to the right node
	for(int i = 0; i < hash; i ++){
		temp = temp->next;
	}
	//Return the item using set_find function from set.h and set located in the temporary node
	return set_find(temp->set, key);
}

//Print out whole hashtable
void hashtable_print(hashtable_t *ht, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item)){
	//If hashtable is NULL print (null)
	if(ht != NULL){	
		// If file is NULL or function is NULL, ignore
		if(fp != NULL && itemprint != NULL){
			//Create hashtable for iteration
			hashtable_t *temp = ht;
			while(temp != NULL){
				//Print out the whole set using set_print from set.h
				set_print(temp->set, fp, itemprint);
				temp = temp->next;
			}
		}
	}else{
		printf("(null)");
	}
}

//Iterate through all items in hashtable and call function on it
void hashtable_iterate(hashtable_t *ht, void *arg,
               void (*itemfunc)(void *arg, const char *key, void *item) ){
	//Ignore if hashtable is NULL or function is NULL
	if(ht != NULL && itemfunc != NULL){
		//Create temporary hashtable for iteration
		hashtable_t *temp = ht;
		while(temp != NULL){
			//Using set_iterate function iterate through set and call function
			set_iterate(temp->set, arg, itemfunc);
			temp = temp->next;
		}
	}
}

//Delete the whole hashtable
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) ){
	//Ignore if hashtable is alr NULL
	if(ht != NULL){
		//create temporary hashtable for iteration
		hashtable_t *temp;
		while(ht != NULL){
			//Make temp = to hashtable make hashtable equal to next
			temp = ht;
			ht = ht->next;
			//Delete everything in temp
			set_delete(temp->set, itemdelete);
			free(temp);
		}	
	}	
}
