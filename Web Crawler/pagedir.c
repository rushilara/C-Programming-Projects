#include "pagedir.h"
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>

bool pagedir_init(const char *pageDirectory) {
	// Fill in with your implementationi
	
	//Create struct stat inorder for holding information of directory
	struct stat statbuf;
	//Use stat function to see if page deirectory exists, if it doesn't return false
	if(stat(pageDirectory, &statbuf) != 0){
		return false;
	}else{
		//Else create file of size 1024
		char fullpath[1024];
		//Make file in pageDirectory named "any_name.crawler"
		snprintf(fullpath, sizeof(fullpath), "%s/%s", pageDirectory, "any_name.crawler");
		// Create file in order to check if it works properly
		FILE *file = fopen(fullpath, "w");
		if(!file){
			return false;
		}
		fclose(file);
		return true;
	}
}

void pagedir_save(const webpage_t *page, const char *pageDirectory, const int documentID) {
	// Fill in with your implementation
	
	//Create file of size 1024
	char fullpath[1024];
	//Create file in pagedirectory named the documentID
	snprintf(fullpath, sizeof(fullpath), "%s/%d", pageDirectory, documentID);
	//Open file to write, if there is error print error opening file
	FILE *file = fopen(fullpath, "w");
	if(!file){
		perror("Error opening file");
		return;
	}
	//Write to file url, depth, and the html and close the file, if it doens't work print error writing to file
	if(fprintf(file, "%s\n%d\n%s", page->url, page->depth, page->html) < 0){
		perror("Error writing to file");
		fclose(file);
		return;
	}
	fclose(file);
}
