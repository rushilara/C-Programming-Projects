#include "growing_arrays.h" 
// Testing code. You can modify this and check your own test cases.
struct Table table;
int main(int argc, char **argv) {
    table.nval = 0;
    table.max = INIT_SIZE;
    table.record = malloc(sizeof(Record) * table.max);

    // test add_record
    Record r1 = {"Alice", 1};
    Record r2 = {"Bob", 2};
    Record r3 = {"Charlie", 3};
    int i1 = add_record(r1);
    int i2 = add_record(r2);
    int i3 = add_record(r3);
    printf("added records at indices %d, %d, %d\n", i1, i2, i3);

    // test get_id and get_record
    int id = get_id("Bob");
    Record r = get_record(i1);
    printf("record at index %d: name=%s, id=%d\n", i1, r.name, r.id);
    printf("record with name 'Bob': id=%d\n", id);

    // test del_record
    int success = del_record("Bob");
    printf("deleted record with name 'Bob': %d\n", success);
    id = get_id("Bob");
    printf("record with name 'Bob': id=%d\n", id);

    free(table.record);
    return 0;
}

/*
Output for the above testing code:

added records at indices 0, 1, 2
record at index 0: name=Alice, id=1
record with name 'Bob': id=2
deleted record with name 'Bob': 1
record with name 'Bob': id=0


*/
