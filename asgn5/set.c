#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"

// set_new function to create a new set
set_t* set_new(void){
	// allocate memory for set pointer
	set_t *temp_set = (set_t*) malloc(sizeof(set_t));
	// if memory allocation failed return NULL
	if(temp_set == NULL){
		return NULL;
	}
	//initialize data to NULL
	temp_set->key = NULL;
	temp_set->item = NULL;
	temp_set->next = NULL;
	return temp_set;
}

bool set_insert(set_t *set, const char *key, void *item){
	// Return false if any of the arguments are NULL
	if(set == NULL || key == NULL || item == NULL){
		return false;
	}
	// Create temporary set equal to argument set for iteration
	set_t *temp_set = set;
	//Iterate through set
	while(temp_set != NULL){
		/*If key exists return false or if whatever erason the key is equal to NULL
		
		  
		  
		  if(temp_set->key == NULL){
			return false;
		}



		*/
		if (temp_set->key != NULL && strcmp(temp_set->key, key) == 0) {
    			return false;
		}

		temp_set = temp_set->next;
	}
	//Create new set in order to add to the end with arguments key and item
	set_t *new_node = set_new();
	//If creation fails return false
	if(new_node == NULL){
		return false;
	}
	//Load data into node
	new_node->key = key;
	new_node->item = item;
	new_node->next = NULL;
	// Iterate through set to add new_node to the end
	set_t *temp1 = set;
	while(temp1->next != NULL){
		temp1 = temp1->next;
	}
	temp1->next = new_node;
	//return true if successful
	return true;
}

void *set_find(set_t *set, const char *key){
	//Create temp set for iteration
	set_t *temp = set;
	// if arguments are NULL return NULL
	if(set == NULL || key == NULL){
		return NULL;
	}
	//iterate through set and exit loop if node key is equal to argument key or if temp reaches end of list
	while(temp != NULL && strcmp(temp->key, key) != 0){
		temp = temp->next;
	}
	// If temp is NULL return NULL
	if(temp == NULL){
		return NULL;
	}
	//Otherwise, return the item in the node
	return temp->item;
}

void set_print(set_t *set, FILE *fp,
               void (*itemprint)(FILE *fp, const char *key, void *item) ){
	//If file is NULL or the function is NULL don't do anything
	if(fp != NULL && itemprint != NULL){
		//If set is null print (null)
		if(set == NULL){
			printf("(null)");
		}
		//create temporary set equal to argument set and iterate through it
		set_t *temp = set;
		while(temp != NULL){
			//print each key and item
			itemprint(fp, temp->key, temp->item);
			temp = temp->next;
		}
	}

}

void set_iterate(set_t *set, void *arg,
                 void (*itemfunc)(void *arg, const char *key, void *item) ){
	//If set is NULL or itemfunc is NULL do nothing
	if(set != NULL && itemfunc != NULL){
		//Create temporary set to iterate through
		set_t *temp = set;
		//Call function on each item in set
		while(temp != NULL){
			itemfunc(arg, temp->key, temp->item);
			temp = temp->next;
		}
	}
}

void set_delete(set_t *set, void (*itemdelete)(void *item) ){
	//If Set is equal to NULL ignore
	if(set!=NULL){
		//Create temporary set to iterate through
		set_t *temp = set;
		//Delete each set node
		while(temp != NULL){
			free((void*)temp->key);
			itemdelete(temp->item);
			temp = temp->next;
		}
		//Free temp and set
		free(temp);
		free(set);
	}
}
