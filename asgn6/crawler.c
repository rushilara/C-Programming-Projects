#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "crawler.h"
#include "curl.h"
#include "set.h"
#include "hashtable.h"
#include "url.h"
#include "pagedir.h"

/**
 * Parses command-line arguments, placing the corresponding values into the pointer arguments seedURL,
 * pageDirectory and maxDepth. argc and argv should be passed in from the main function.
 */

// For bag key will be the url and item will be the webpage
// For hashtable, both the key and the item will be the webpage

//Create new bag
bag_t* bag_new(void){
	// allocate memory for bag pointer
    	bag_t *temp_bag = (bag_t*) malloc(sizeof(bag_t));
    	// if memory allocation failed return NULL
    	if(temp_bag == NULL){
        	return NULL;
    	}
    	//initialize set in bag to new set if fails return NULL
    	temp_bag->set = set_new();
	if(temp_bag->set == NULL){
		return NULL;
	}
    	return temp_bag;
}

//Insert into bag
bool bag_insert(bag_t *bag, const char *key, void *item){
	//If bag argument is NULL return false
	if(bag == NULL){
		return false;
	}
	//If insertion into bag set fails, return false
	if(set_insert(bag->set, key, item) == false){
		return false;
	}
	return true;
}

//Create new webpage
webpage_t* webpage_new(void){
	//Allocate memory for temp_webpage, if it fails return NULL
	webpage_t *temp_webpage = (webpage_t*) malloc(sizeof(webpage_t));
	if(temp_webpage == NULL){
		return NULL;
	}
	//Initialize all data in webpage to NULL
	temp_webpage->url = NULL;
	temp_webpage->html = NULL;
	return temp_webpage;
}

//Function to check if bag is empty
bool bag_empty(bag_t *bag){
	if(bag == NULL){
		return false;
	}
	//If set in bag is NULL then return true, othewrise return false
	if(bag->set == NULL){
		return true;
	}else{
		return false;
	}
}

//Function to pull an item from a bag
void *bag_pull(bag_t *bag){
	//If bag is null or set in bag is null return NULL
	if(bag == NULL){
		return NULL;
	}
	set_t *temp_set = bag->set;
	if(temp_set == NULL){
		return NULL;
	}
	//Store webpage in webpage pointer
	void *item = temp_set->item;
	//Make set in bag equal to next node
	bag->set = temp_set->next;
	//free everything and return the webpage
	free(temp_set);
	return item;
	
	
}

//function to delete webpage
void delete_webpage(webpage_t *webpage){
	//Ignore if webpage is null
	if(webpage != NULL){
		//check if data in webpage is null, if not free it
		if(webpage->url != NULL){
			free(webpage->url);
		}
		if(webpage->html != NULL){
			free(webpage->html);
		}
		//free webpage
		free(webpage);
}

//function to delete bag
void delete_bag(bag_t *bag){
	//if bag is not NULL check if set isn't NULL, delete set then free bag
	if(bag != NULL){
		if(bag->set != NULL){
			set_delete(bag->set, NULL);
		}
		free(bag);
	}

}

//Function to extractURLs from html into a bag
void extractURLs(char *htmlContent, char *baseUrl, bag_t *bag) {
	char *anchorPtr = htmlContent;
    	char *hrefStart;
    	char *hrefEnd;
    	char urlBuffer[2048]; // Buffer for URLs

    	while ((anchorPtr = strstr(anchorPtr, "<a ")) != NULL) {
        	if ((hrefStart = strstr(anchorPtr, "href=\"")) != NULL) {
            		hrefStart += 6; // Skip past 'href="'
        	} else if ((hrefStart = strstr(anchorPtr, "href='")) != NULL) {
            		hrefStart += 6; // Skip past "href='"
        	} else {
            		anchorPtr++; // Move past this <a tag
            		continue;
        	}

        	hrefEnd = strchr(hrefStart, hrefStart[-1] == '\"' ? '\"' : '\'');
        	if (hrefEnd == NULL) {
            		anchorPtr++; // Skip malformed href and continue
            		continue;
        	}

        	// Check for URL length to prevent buffer overflow
        	size_t urlLength = hrefEnd - hrefStart;
        	if (urlLength >= sizeof(urlBuffer)) {
            		// URL is too long; skip this URL
            		anchorPtr = hrefEnd;
            		continue;
        	}

        	// Copy the URL to the buffer
        	strncpy(urlBuffer, hrefStart, urlLength);
        	urlBuffer[urlLength] = '\0'; // Ensure null-termination

        	// Check if the URL is an anchor (starts with '#')
        	if (urlBuffer[0] == '#') {
            		anchorPtr = hrefEnd; // Skip this URL
            		continue;
        	}

        	// Normalize URL
        	char* normalizedUrl = normalizeURL(baseUrl, urlBuffer);

        	// Add the URL to the bag
        	bag_insert(bag, normalizedUrl, normalizedUrl);

        	// Free the normalized URL if needed
        	free(normalizedUrl);

        	// Move past this URL
        	anchorPtr = hrefEnd;
    	}
}



static void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth) {
	//If incorrect amount of arguments print error and exit with failure
	if (argc != 4) {
		fprintf(stderr, "Usage: %s <seedURL> <pageDirectory> <maxDepth>\n", argv[0]);
        	exit(EXIT_FAILURE);
    	}
	//Assign seedURL
    	*seedURL = argv[1];
	//If failure to create page directory, print error and exit with failure
    	if (!pagedir_init(argv[2])) {
        	fprintf(stderr, "Failed to initialize page directory\n");
        	exit(EXIT_FAILURE);
    	}
	//Assign pageDirectory name
    	*pageDirectory = argv[2];
	//Use strtol to properly assign number to the third argument
    	char *endptr;
    	int number = (int) strtol(argv[3], &endptr, 10);
	//If strtol fails or if number is out of range print error and exit with failure
    	if (*endptr != '\0' || number < 0 || number > 10) {
        	fprintf(stderr, "maxDepth must be an integer in the range [0 ... 10]\n");
        	exit(EXIT_FAILURE);
    	}
	//assign maxdepth
    	*maxDepth = number;
}

/**
 * Crawls webpages given a seed URL, a page directory and a max depth.
 */
static void crawl(char *seedURL, char *pageDirectory, const int maxDepth) {
	//Create new hashtable with the number of slots being maxDepth
	hashtable_t *pagesSeen = hashtable_new(maxDepth);
	//If creation fails return error
	if(pagesSeen == NULL){
		fprintf(stderr, "issue creating hashtable\n");
		exit(EXIT_FAILURE);
	}
	//Insert seedURL into hashtable, if it fails return error
	if(hashtable_insert(pagesSeen, seedURL, seedURL) == false){
		fprintf(stderr, "issue inserting seedURL to hashtable\n");
		exit(EXIT_FAILURE);
	}
	//Create new bag pagesToCrawl, if creation fails return error
	bag_t *pagesToCrawl = bag_new();
	if(pagesToCrawl == NULL){
		fprintf(stderr, "issue creating bag\n");
		exit(EXIT_FAILURE);
	}
	//Create new webpage, if creation fails return error
	webpage_t *seed_webpage = webpage_new();
	if(seed_webpage == NULL){
		fprintf(stderr, "issue creating seed_webpage\n");
		exit(EXIT_FAILURE);
	}
	//Pass url and depth into seed_webpage
	seed_webpage->url = seedURL;
	seed_webpage->depth = 0;
	//Insert webpage into bag, if this fails return error
	if(bag_insert(pagesToCrawl, seedURL, seed_webpage) == false){
		fprintf(stderr, "issue inserting seedURL to bag\n");
		exit(EXIT_FAILURE);
	}
	//Initialize document_id to start with 1
	int document_id = 1;
	//While bag is not empty
	while(!bag_empty(pagesToCrawl)){
		//Pull webpage from bag, if webpage is null return NULL
		webpage_t *webpage = bag_pull(pagesToCrawl);
		if(webpage == NULL){
			fprintf(stderr, "issue pulling webpage from bag\n");
			exit(EXIT_FAILURE);
		}
		//Download html into webpage
		size_t *length;
		webpage->html = download(webpage->url, length);
		webpage->length = *length;
		//If webpage is not NULL save the webpage into directory, icrement document ID
		if(webpage->html != NULL){
			pagedir_save(webpage, pageDirectory, document_id);
			document_id++;
			//If depth isn't at maxDepth scan more pages into bag and hashtable
			if(webpage->depth < maxDepth){
				pageScan(webpage, pagesToCrawl, pagesSeen);
			}	
		}
		//Delete the webpage
		delete_webpage(webpage);
		


	}
	//Delette the hashtable and the bag
	hashtable_delete(pagesSeen, NULL);
	delete_bag(pagesToCrawl);

}

/**
 * Scans a webpage for URLs.
 */
static void pageScan(webpage_t *page, bag_t *pagesToCrawl, hashtable_t *pagesSeen) {
	// Create new bag to store all the urls within the html of the page
	bag_t *urls = bag_new();
	//Check memory was allocated properly, if not return error
	if(urls == NULL){
		fprintf(stderr, "error creating bag\n");
		exit(EXIT_FAILURE);
	}
	//Extract URLs into bag
	extractURLs(page->html, page->url, urls);
	//Iterate through bag
	while(!bag_empty(urls)){
		//Pull url from bag
		char *url = bag_pull(urls);
		//If Url is internal, try to insert url into hashtable
		if(isInternalURL(page->url, url)){
			char *new_url = strdup(url);
			if(new_url == NULL){
				free(url);
				fprintf(stderr, "Error copying url\n");
				exit(EXIT_FAILURE);
			}
			if(hashtable_insert(pagesSeen, new_url, new_url)){
				//If insertion was successful create new webpage
				webpage_t *new_webpage = webpage_new();
				//Error checking
				if(new_webpage == NULL){
					free(url);
					fprintf(stderr, "Error creating webpage\n");
					exit(EXIT_FAILURE);
				}
				//Load new url and depth into webpage and insert into bag
				new_webpage->url = new_url;
				new_webpage->depth = page->depth + 1;
				bag_insert(pagesToCrawl, new_url, new_webpage);
			//Free memory
			}else{
				free(new_url);
			}
		}
		free(url);
	}
	delete_bag(urls);
}

int main(const int argc, char *argv[]) {
	//Create variables to store seedURL, pageDirectory, and maxDepth
	char *seedURL;
	char *pageDirectory;
	int *maxDepth;
	//Parse arguments
	parseArgs(argc, argv, &seedURL, &pageDirectory, maxDepth);
	//Crawl
	crawl(seedURL, pageDirectory, *maxDepth);
	return 0;

}
