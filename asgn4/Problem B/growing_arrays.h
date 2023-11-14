#ifndef GROWING_ARRAYS_H
#define GROWING_ARRAYS_H

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct Record {
    char *name;
    int id;
} Record;

extern struct Table {
    int nval;   // current number of records
    int max;    // allocated number of records
    Record *record; // array of records, i.e., name-id pairs
} table;

enum { INIT_SIZE = 1, GROW_SIZE = 2 };

int add_record(Record record);

int del_record(const char* name);

int get_id(const char *name);

Record get_record(int index);

#endif
