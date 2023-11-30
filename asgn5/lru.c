#include <stdio.h>
#include <stdlib.h>
#include "lru.h"
#include "set.h"
#include <string.h>
#include <stdbool.h>

//Helper function to find the least used key
const char *least_used(lru_t *lru) {
    // Return NULL if lru is empty or equal to NULL
    if (lru == NULL || lru->curr == 0) {
        return NULL;
    }

    // Create temporary set for iteration and initialize max, num, and i
    set_t *temp = lru->num;
    int max = 0;
    int num = 0;
    int i = 0;

    // Iterate through the set and update max if a higher value is found
    while (temp != NULL) {
        if (*((int*)temp->item) > max) {
            max = *((int*)temp->item);  // Update max
            num = i;  // Record the index of this item
        }
        i++;
        temp = temp->next;
    }

    // Create another temporary set for iteration to find the key at the recorded index
    set_t *temp1 = lru->num;
    for (int j = 0; j < num && temp1 != NULL; j++) {
        temp1 = temp1->next;
    }

    // Return the key if temp1 is not NULL
    return (temp1 != NULL) ? temp1->key : NULL;
}


//Create empty lru, return NULL if error
lru_t *lru_new(const int capacity){
	//Allocate memory if failure, return NULL
	lru_t *lru = (lru_t*) malloc(sizeof(lru_t));
	if(lru == NULL){
		return NULL;
	}else{
		//Set max to capacity, create new set and set curr to 0
		lru->max = capacity;
		lru->set = set_new();
		lru->num = set_new();
		if(lru->set == NULL || lru->num == NULL){
			return NULL;
		}
		lru->curr = 0;
	}
	//Return lru
	return lru;
}

//Add new item to lru
bool lru_insert(lru_t *ht, const char *key, void *item) {
    if (ht == NULL || key == NULL || item == NULL) {
        return false;
    }

    // Check if the key already exists
    set_t *temp = ht->set;
    while (temp != NULL) {
        if (temp->key != NULL && strcmp(temp->key, key) == 0) {
            return false; // Key already exists
        }
        temp = temp->next;
    }

    // Allocate memory for the usage count
    int *usageCount = malloc(sizeof(int));
    if (usageCount == NULL) {
        return false; // Memory allocation failed
    }
    *usageCount = 0; // Initialize usage count

    // Check if the LRU cache is full
    //For testing printf("%d = %d\n", ht->curr, ht->max);
    if (ht->curr == ht->max) {
        const char *least = least_used(ht);
        if (least == NULL) {
            free(usageCount);
            return false;
        }
	//For testing printf("%s\n", least);

        set_t *new_node = (set_t *) malloc(sizeof(set_t));
        if (new_node == NULL) {
            free(usageCount);
	    //For testing printf("bleh");
            return false; // Memory allocation failed
        }
        new_node->key = key; // Assume key is not dynamically allocated here
        new_node->item = item;

        // Replace the least used item
if (strcmp(ht->set->key, least) == 0) {
    set_t *old_node = ht->set;
    ht->set = new_node; // Update the head of the list
    new_node->next = old_node->next;

    // Free the old node's contents if they were dynamically allocated
    if (old_node->key != key) {
        //free((void*)old_node->key); // Free only if dynamically allocated
    }
    if (old_node->item != item) {
        //free(old_node->item); // Free only if dynamically allocated
    }
    free(old_node);
} else {
    temp = ht->set;
    while (temp->next != NULL && strcmp(temp->next->key, least) != 0) {
        temp = temp->next;
    }
    if (temp->next != NULL) {
        set_t *old_node = temp->next;
        new_node->next = old_node->next;
        temp->next = new_node;

        // Free the old node
        free((void*)old_node->key); // Only if dynamically allocated
        free(old_node->item);       // Only if dynamically allocated
        free(old_node);
    } else {
        free(new_node);
        free(usageCount);
        return false;
    }
}


        set_t *temp1 = ht->num;
        while (temp1 != NULL && strcmp(temp1->key, least) != 0) {
            temp1 = temp1->next;
        }

        if (temp1 != NULL) {
            free(temp1->item); // Free the old usage count
            temp1->item = usageCount; // Update to the new usage count
        } else {
            free(usageCount);
            return false;
        }
    } else {
        // Cache is not full, insert normally
        if (!set_insert(ht->set, key, item) || !set_insert(ht->num, key, usageCount)) {
            free(usageCount); // Free the allocated memory if insertion fails
            return false;
        }
        ht->curr++;
    }

    // Increment the counters for all other keys
    temp = ht->num;
    while (temp != NULL) {
        if (temp->key != NULL && strcmp(temp->key, key) != 0) {
            int* itemPtr = (int*)temp->item;
            (*itemPtr)++;
        }
        temp = temp->next;
    }

    return true;
}




//Find item using key in lru
void *lru_find(lru_t *ht, const char *key){
	//Find the value of the item using set_find function in set.h
	void *item = set_find(ht->set, key);
	//If error or parameter is NULL, return NULL
	if(item == NULL || ht == NULL || key == NULL){
		return NULL;
	}
	//Iterate through num list, if key is not equal to parameter key than add the item by 1, else equal the item to 0
	set_t *temp = ht->num;
	while(temp != NULL){
		if(strcmp(temp->key, key) != 0){
			int* itemPtr = (int*)temp->item;
                        (*itemPtr)++;
		}else{
			temp->item = 0;
		}
		temp = temp->next;
	}
	//Return the item
	return item;
}

//Print the whole lru
void lru_print(lru_t *ht, FILE *fp,
                     void (*itemprint)(FILE *fp, const char *key, void *item)){
	//If file pointer or itemprint is NULL ignore
	if(fp != NULL && itemprint != NULL){
		// If ht is null print (null)
		if(ht != NULL){
			set_print(ht->set, fp, itemprint);
		}else{
			printf("(null)");
		}
	}
}

//Iterate over lru and call function on each item
void lru_iterate(lru_t *ht, void *arg,
               void (*itemfunc)(void *arg, const char *key, void *item) ){
	//Ignore if ht is NULL or itemfunc is NULL
	if(ht != NULL && itemfunc != NULL){
		//Use set_iterate function from set.h to iterate over the set
		set_iterate(ht->set, arg, itemfunc);
	}
}

//Delete the whole lru
void lru_delete(lru_t *ht, void (*itemdelete)(void *item) ){
	if(ht != NULL){
		set_delete(ht->set, itemdelete);
		set_delete(ht->num, NULL);
		free(ht);
	}
}
