#ifndef CRAWLER_H
#define CRAWLER_H
#include "set.h"
#include "hashtable.h"
#include <stddef.h>

typedef struct {
	char *url;
	char *html;
	size_t length;
	int depth;
} webpage_t;

typedef struct {
	//Create set within bag data structure for storing the webpages
	set_t *set;
} bag_t;

/*typedef struct {
	// Fill in with your implementation
} hashtable_t;*/

#endif
