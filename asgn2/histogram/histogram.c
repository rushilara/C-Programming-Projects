#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MIN 0

//Implementing an int stack data strucutre with an integer data and a pointer that points to the next node
struct node{
	int data;
	struct node *next;
};

//Push function to load data into the stack
void push(int data, struct node **head){
	//Create a temp node that loads data in and acts as the next head
	struct node *temp = malloc(sizeof(struct node));
	temp->data = data;
	//Point next in temp to the previous head and move head to the new head, temp
	temp->next = *head;
	*head = temp;
}

//Pop function to return int data from stack
int pop(struct node **head){
	//Create return function incase stack is empty
	if(*head == NULL){
		return -1;
	}	
	//Create temporary head and make it equal to the head
	struct node *temp = malloc(sizeof(struct node));
	temp = *head;
	//Store data from head
	int data = temp->data;
	//Move head to the next node
	*head = temp->next;
	//Delete previous head using temp and return stored data
	free(temp);
	temp = NULL;
	return data;
}

int main(){
	//Stack to hold int values
	struct node *head = NULL;
	
	//variables to help me with gathering the input
	
	//Variables that help relating to converting the string input into a number
	int count = 0;
	char *input = (char*) malloc(sizeof(char));
	char character;

	//Variables to load correct numbers into stack
	int num;
	int size = 1;
	int min = MIN;
	int max = 16;
	
	//variables to help with output
	int array[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	printf("16 bins of size 1 for range [0,16)\n");
	//While loop to gather input
	while(scanf("%c", &character) != EOF){
		/* Use the following if doesn't work, use in order to implement copy paste method

		 if(character == '\004'){
			break;
		}
		*/
		//Push data into stack if character is whitespace
		if(character == ' ' || character == '\t' || character == '\n'){
			num = atoi(input);
			//ignore negative numbers
			if(num >= min){
				push(num, &head);
				//while the number is greater than the max increase size and max and print new bins
				while(num >= max){
					size *= 2;
				       	max *= 2;	
					printf("16 bins of size %d for range [%d,%d)\n", size, min, max);
				}	
			}
			//Reset count and input string
			count = 0;
		       	*input = '\0';	
		}
		else{
		//If character isn't blank space, allocate new memory to temp
		char *temp = (char *)realloc(input,(count + 2) * sizeof(char));
		if (!temp) {
   		 	// handle memory allocation failure
   		 	free(input);  // important if you don't want to continue using `input` after this point
			printf("Failed to reallocate memory");
			return 1;
		} else {
   		 	input = temp;
		}
		//Put new character into input string, increase count and add null character at the end of the string
		*(input + count) = character;
		count++ ;
		*(input + count) = '\0';
	
		}
	}

	//Code to generate Output
	
	//Load data from stack into array
	while(head != NULL){
		//Create int variable num from popped data
		int nums = pop(&head);
		//Find where in the array to increase count by dividing size by number in nums
		int loc = nums / size;
		//Increase the count in the correct spot in the array
		array[loc]++;
	}
	

	//Printing Output
	for(int i = 0; i <= 15; i++){
		//Intialize min in line
		int mins = i * size;
		//Create string pointer for loading asterisks
		char *str = (char*) malloc(sizeof(char));
		*str = '\0';
		//For loop to load asterisks
		for(int j = 0; j < array[i]; j++){
			//Reallocate memory into string for loading additional asterisks
			char *temp = (char *)realloc(str,(j + 2) * sizeof(char));
                	if (!temp) {
                 		// handle memory allocation failure
                 		free(str);  // important if you don't want to continue using `input` after this point
                		printf("Failed to reallocate memory");
                		return 1;
                	} else {
                 		str = temp;
                	}
			//Load asterisk and null character into string
			*(str + j) = '*';
			*(str + j + 1) = '\0';	
		}
		//If the string isn't empty than we must add a space before the asterisks
		if(*str != '\0'){
			//Realloc memory in order for space and null terminator
			char *temp = (char *)realloc(str,strlen(str) + 2);
                        if (!temp) {
                                // handle memory allocation failure
                                free(str);  // important if you don't want to continue using `input` after this point
                                printf("Failed to reallocate memory");
                                return 1;
			}
			else{
				str = temp;
			}
			//Move the memory located in str by 1
			memmove(str + 1, str, strlen(str) + 1);
			//Make first character of spring into a space
			str[0] = ' ';
		}
		//Print statement using minimum, size and asterisk string
		printf("[%3d:%3d]%s\n", mins, (mins + (size - 1)), str);
	}



	//Free memory from input string and make input equal to NULL
	free(input);
	input = NULL;
	return 0;
}
