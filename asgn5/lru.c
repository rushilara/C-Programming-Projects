#include <stdio.h>
#include <stdlib.h>
#include "lru.h"
#include "set.h"
#include <string.h>
#include <stdbool.h>

//Helper function to find the least used key
const char *least_used(lru_t *lru){
	//Return NULL if lru is empty or equal to NULL
	if(lru == NULL){
		return NULL;	
	}
	if(lru->curr == 0){
		return NULL;
	}
	//Create temporary set for iteration and intialize max num and i
	set_t *temp = lru->num;
	int max = 0;
	int num = 0;
	int i = 0;
	//Iterate through the set and change max variable if it is greater than max
	while(temp!=NULL){
		if(*((int*)temp->item)> max){
			//Change max variable and log num as i for iteration purposes
			max = *((int*)temp->item);
			num = i;
		}
		i++;
		temp = temp->next;

	}
	//Create temporary set for iteration and return key in the right node
	set_t *temp1 = lru->num;
	for(int i = 0; i < num; i++){
		temp1 = temp1->next;
	}
	return temp1->key;
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
		lru->curr = 0;
	}
	//Return lru
	return lru;
}

//Add new item to lru
bool lru_insert(lru_t *ht, const char *key, void *item){
	//If any parameter is false return NULL
	if(ht == NULL || key == NULL || item == NULL){
		return false;
	}
	//Create temporary set for iteration
	set_t *temp5 = ht->set;
	//Check if key exists already, if it does return false
	while(temp5 != NULL){
		if(strcmp(temp5->key, key) == 0){
			return false;
		}
		temp5 = temp5->next;
	}
	//If current number of items is equal to max then change the least used node to the new node
	if(ht->curr == ht->max){
		//Find the least used key using helper function
		const char *least = least_used(ht);
		//Create temp for iteration
		set_t *temp = ht->set;
		//Create i to find the right index
		int i = 0;
		//Check where the least used key is
		while(strcmp(temp->key, least) != 0){
			temp = temp->next;
			i++;
		}
		//Change the least used key to the new key and item
		temp->key = key;
		temp->item = item;
		//Change the corresponding item in num set to the new key and 0
		set_t *temp1 = ht->num;
		for(int j = 0; j < i; j++){
			temp1 = temp1->next;
		}
		temp1->key = key;
		temp1->item = 0;
	//Else if number of items is not equal to max
	}else{
		//Add new key and item normally
		bool success1 = set_insert(ht->set, key, item);
		//Check for errors
		if(success1 == false){
			return false;
		}
		//Add new key and num 0 normally to num set
		bool success2 = set_insert(ht->set, key, 0);
		//Check for errors
		if(success2 == false){
			return false;
		}
		//Increase the current number of items by 1 
		ht->curr++;
	}
	//Check that if the key is not equal to the added key the number in num set increases by 1
	set_t *temp2 = ht->num;
	while(temp2 != NULL){
		if(strcmp(temp2->key, key) != 0){
			int* itemPtr = (int*)temp2->item;
			(*itemPtr)++;
		}
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
