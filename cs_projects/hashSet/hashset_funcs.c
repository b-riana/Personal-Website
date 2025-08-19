// hashset_funcs.c: utility functions for operating on hash sets. Most
// functions are used in the hashset_main.c which provides an
// application to work with the functions.

#include "hashset.h"

// PROVIDED: Compute a simple hash code for the given character
// string. Uses a "polyomial code" which uses all characters of the
// string to form the hash code. This is the same approach that many
// standard libraries like Java's String.hashcode() function
// use. ADVANTAGE: Gives good distribution for all strings with even
// single character variations having different hash
// codes. DISADVANTAGE: computing the hash code is linear in time on
// the length of the string.
int hashcode(char key[]) {
  int hc = 0;
  for (int i = 0; key[i] != '\0'; i++) {
    hc = hc * 31 + key[i];
  }
  return hc;
}

void hashset_init(hashset_t *hs, int table_size) {
  // initializes all hashset fields to either NULL or the proper int
  hs->order_first = NULL;
  hs->order_last = NULL;
  hs->table_size = table_size;
  hs->data_count = 0;

  hsnode_t **arr = malloc(sizeof(hsnode_t) * table_size);  // allocates space for hashset table
  for (int i = 0; i < table_size; i++) {                   // sets each node to null
    arr[i] = NULL;
  }
  hs->table = arr;
}
// Initialize the hash set 'hs' to have given size and data_count
// 0. Ensures that the 'table' field is initialized to an array of
// size 'table_size' and is filled with NULLs. Also ensures that the
// first/last pointers are initialized to NULL

int hashset_contains(hashset_t *hs, char data[]) {
  int hc = hashcode(data);
  hc = hc > -hc ? hc : -hc;          // makes the hashcode positive
  int index = hc % hs->table_size;   // limits hashcode size
  hsnode_t *cur = hs->table[index];  // gets first node at index
  while (cur != NULL) {              // loops through nodes at index and checks if data matches
    hsnode_t *next = cur->table_next;
    if (strcmp(data, cur->data) == 0) {
      return 1;  // node found
    }
    cur = next;
  }
  return 0;  // node was not found
}
// Returns 1 if the parameter `data` is in the hash set and 0
// otherwise. Uses hashcode() and field `table_size` to determine
// which index in table to search.  Iterates through the list at that
// table index using strcmp() to check for `data`. NOTE: The
// `hashcode()` function may return positive or negative
// values. Negative values are negated to make them positive. The
// "bucket" (index in hs->table) for `data` is determined by with
// 'hashcode(key) modulo table_size'.

int hashset_add(hashset_t *hs, char data[]) {
  if (hashset_contains(hs, data)) {  // checks if data is already in hashset
    return 0;
  }

  int hc = hashcode(data);          // gets hashcode
  hc = hc > -hc ? hc : -hc;         // makes the hashcode positive
  int index = hc % hs->table_size;  // limits hashcode size

  // initializes new node, assigns proper fields
  hsnode_t *add = malloc(sizeof(hsnode_t));
  strcpy(add->data, data);
  add->order_next = NULL;
  add->table_next = hs->table[index];  // saves pointer to pre-existing elems

  // updates hashset fields
  hs->data_count++;           // increments elem count
  hs->table[index] = add;     // points list to start at new node
  if (hs->data_count == 1) {  // checks if node is first elem to be added
    hs->order_first = add;
  } else {
    hs->order_last->order_next = add;
  }
  hs->order_last = add;

  return 1;
}
// If the data is already present in the hash set, makes no changes
// to the hash set and returns 0. hashset_contains() may be used for
// this. Otherwise determines the bucket to add `data` at via the same
// process as in hashset_contains() and adds it to the FRONT of the
// list at that table index. Adjusts the `hs->order_last` pointer to
// append the new data to the ordered list of data. If this is the
// first data added, also adjsuts the `hs->first` pointer. Updates the
// `data_count` field and returns 1 to indicate a successful addition.
//
// NOTE: Adding data at the front of each bucket list allows much
// simplified logic that does not need any looping/iteration.

void hashset_free_fields(hashset_t *hs) {
  hsnode_t *cur = hs->order_first;
  while (cur != NULL) {  // loops through nodes and frees them
    hsnode_t *next = cur->order_next;
    free(cur);
    cur = next;
  }
  free(hs->table);
  // resets hs fields
  hs->order_first = NULL;
  hs->order_last = NULL;
  hs->table_size = 0;
  hs->data_count = 0;
}
// De-allocates nodes/table for `hs`. Iterates through the ordered
// list of the hash set starting at the `order_first` field and
// de-allocates all nodes in the list. Also free's the `table`
// field. Sets all relevant fields to 0 or NULL as appropriate to
// indicate that the hash set has no more usable space. Does NOT
// attempt to de-allocate the `hs` itself as it may not be
// heap-allocated (e.g. in the stack or a global).

void hashset_show_structure(hashset_t *hs) {
  // prints basic info ab hashset (elem count, table size, first & last nodes, load factor)
  printf("data_count: %d\n", hs->data_count);
  printf("table_size: %d\n", hs->table_size);
  if (hs->data_count >= 1) {  // checks if hashset has elems
    printf("order_first: %s\n", hs->order_first->data);
    printf("order_last : %s\n", hs->order_last->data);
  } else {  // hashset is empty
    printf("order_first: NULL\n");
    printf("order_last : NULL\n");
  }
  printf("load_factor: %.4f\n", hs->data_count * 1.00 / hs->table_size);

  for (int row = 0; row < hs->table_size; row++) {  // loops through nodes in table order
    printf("[%2d] : ", row);

    hsnode_t *cur = hs->table[row];  // gets first node at index
    while (cur != NULL) {
      char *next_data = cur->order_next != NULL ? cur->order_next->data : "NULL";
      printf("{%ld %s >>%s} ", (long)hashcode(cur->data), cur->data, next_data);  // prints node info
      cur = cur->table_next;                                                      // gets next node at index
    }

    printf("\n");
  }
}

// Displays detailed structure of the hash set. Shows stats for the
// hash set as below including the load factor (data count divided
// by table_size) to 4 digits of accuracy.  Then shows each table
// array index ("bucket") on its own line with the linked list of
// data in the bucket on the same line.
//
// EXAMPLE:
// data_count: 4
// table_size: 5
// order_first: Rick
// order_last : Tinyrick
// load_factor: 0.8000
// [ 0] : {7738144525137111380 Tinyrick >>NULL}
// [ 1] :
// [ 2] :
// [ 3] : {125779953153363 Summer >>Tinyrick} {1801677138 Rick >>Morty}
// [ 4] : {521644699469 Morty >>Summer}
//
// NOTES:
// - Uses format specifier "[%2d] : " to print the table indices
// - Nodes in buckets have the following format:
//   {1415930697 IceT >>Goldenfold}
//    |          |       |
//    |          |       +-> order_next->data OR NULL if last node
//    |          +->`data` string
//    +-> hashcode("IceT"), print using format "%ld" for 64-bit longs
//

void hashset_write_data_ordered(hashset_t *hs, FILE *out) {
  hsnode_t *cur = hs->order_first;  // gets first node
  int count = 1;

  while (cur != NULL) {                            // loops through all nodes
    fprintf(out, " %d %s\n", count++, cur->data);  // prints count and node data
    cur = cur->order_next;
  }
}
// Outputs all data in the hash set according to the order they
// were added. Starts at the `order_first` field and iterates through
// the list defined there. Each data is printed on its own line
// preceded by its add position with 1 for the first data, 2 for the
// second, etc. Prints output to `FILE *out` which should be an open
// handle. NOTE: the output can be printed to the terminal screen by
// passing in the `stdout` file handle for `out`.

void hashset_save(hashset_t *hs, char *filename) {
  FILE *f = fopen(filename, "w");  // opens file to write in
  if (f == NULL) {
    printf("ERROR: could not open file '%s'\n", filename);
    return;
  }
  fprintf(f, "%d %d\n", hs->table_size, hs->data_count);  // saves table size and data count
  hashset_write_data_ordered(hs, f);                      // prints to file
  fclose(f);
}
// Writes the given hash set to the given `filename` so that it can be
// loaded later.  Opens the file and writes its 'table_size' and
// 'data_count' to the file. Then uses the hashset_write_data_ordered()
// function to output all data in the hash set into the file.
// EXAMPLE FILE:
//
// 5 6
//   1 Rick
//   2 Morty
//   3 Summer
//   4 Jerry
//   5 Beth
//   6 Tinyrick
//
// First two numbers are the 'table_size' and 'data_count' field and
// remaining text is the output of hashset_write_data_ordered();
// e.g. insertion position and data.
//
// If the `filename` cannot be openened and fopen() returns NULL,
// prints the message
//
//  ERROR: could not open file '<FILENAME>'
//
// and returns immediately.

int hashset_load(hashset_t *hs, char *filename) {
  FILE *f = fopen(filename, "r");  // opens file to read
  if (f == NULL) {
    printf("ERROR: could not open file '%s'\n", filename);
    return 0;
  }

  int size;
  int elems;
  fscanf(f, "%d %d\n", &size, &elems);  // gets table size and data count from file

  // clears hashset
  hashset_free_fields(hs);
  hashset_init(hs, size);

  char data[128];
  int count;
  for (int i = 0; i < elems; i++) {        // loops through rest of lines in file
    fscanf(f, "  %d %s\n", &count, data);  // reads elem data
    hashset_add(hs, data);                 // adds node
  }

  fclose(f);
  return 1;  // success
}
// Loads a hash set file created with hashset_save(). If the file
// cannot be opened, prints the message
//
// ERROR: could not open file 'somefile.hs'
//
// and returns 0 without changing anything. Otherwise clears out the
// current hash set `hs`, initializes a new one based on the size
// present in the file, and adds all data from the file into the new
// hash set. Ignores the indices at the start of each line and uses
// hashset_add() to insert data in the order they appear in the
// file. Returns 1 on successful loading (FIXED: previously indicated
// a different return value on success) . This function does no error
// checking of the contents of the file so if they are corrupted, it
// may cause an application to crash or loop infinitely.

int next_prime(int num) {
  int prime = 0;
  while (!prime) {  // keeps looping till prime num is found
    prime = 1;
    for (int i = 2; i < num / 2; i++) {  // searches for possible factor of num
      if (num % i == 0) {
        prime = 0;
      }
    }

    if (!prime) {  // if num is not prime, adds 1 if even, 2 if odd
      num += (num % 2 == 0) ? 1 : 2;
    }
  }

  return num;
}
// If 'num' is a prime number, returns 'num'. Otherwise, returns the
// first prime that is larger than 'num'. Uses a simple algorithm to
// calculate primeness: check if any number between 2 and (num/2)
// divide num. If none do, it is prime. If not, tries next odd number
// above num. Loops this approach until a prime number is located and
// returns this. Used to ensure that hash table_size stays prime which
// theoretically distributes data better among the array indices
// of the table.

void hashset_expand(hashset_t *hs) {
  int new_size = next_prime(2 * hs->table_size + 1);  // gets new size of table

  // makes new hashset & initializes its fields
  hashset_t new_hs;
  hashset_init(&new_hs, new_size);

  hsnode_t *cur = hs->order_first;  // gets first node of old hashset
  while (cur != NULL) {             // loops and adds each node to new hashset
    hashset_add(&new_hs, cur->data);
    cur = cur->order_next;
  }

  hashset_free_fields(hs);  // frees old hashset fields
  *hs = new_hs;             // points to new hashset
}
// Allocates a new, larger area of memory for the `table` field and
// re-adds all current data to it. The size of the new table is
// next_prime(2*table_size+1) which keeps the size prime.  After
// allocating the new table, all table entries are initialized to NULL
// then the old table is iterated through and all data are added to
// the new table according to their hash code. The memory for the old
// table is de-allocated and the new table assigned to the hash set
// fields "table" and "table_size".  This function increases
// "table_size" while keeping "data_count" the same thereby reducing
// the load of the hash table. Ensures that the memory associated with
// the old table is free()'d. Makes NO special effort to preserve old
// nodes: re-adds everything into the new table and then frees the old
// one along with its nodes. Uses functions such as hashset_init(),
// hashset_add(), hashset_free_fields() to accomplish the transfer.