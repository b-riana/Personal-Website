// hashset main application
#include "hashset.h"

int main(int argc, char *argv[]) {
  int echo = 0;                                     // controls echoing, 0: echo off, 1: echo on
  if (argc > 1 && strcmp("-echo", argv[1]) == 0) {  // turn echoing on via -echo command line option
    echo = 1;
  }

  printf("Hashset Application\n");
  printf("Commands:\n");
  printf("  hashcode <data>  : prints out the numeric hash code for the given key (does not change the hash set)\n");
  printf("  contains <data>  : prints FOUND if data is in the set NOT PRESENT otherwise\n");
  printf("  add <data>       : inserts the given data into the hash set, reports existing data\n");
  printf("  print            : prints all data in the hash set in the order they were addded\n");
  printf("  structure        : prints detailed structure of the hash set\n");
  printf("  clear            : reinitializes hash set to be empty with default size\n");
  printf("  save <file>      : writes the contents of the hash set to the given file\n");
  printf("  load <file>      : clears the current hash set and loads the one in the given file\n");
  printf("  next_prime <int> : if <int> is prime, prints it, otherwise finds the next prime and prints it\n");
  printf("  expand           : expands memory size of hash set to reduce its load factor\n");
  printf("  quit             : exit the program\n");

  char cmd[128];
  hashset_t set;
  int success;
  hashset_init(&set, HASHSET_DEFAULT_TABLE_SIZE);

  while (1) {
    printf("HS> ");                      // print prompt
    success = fscanf(stdin, "%s", cmd);  // read a command
    if (success == EOF) {                // check for end of input
      printf("\n");                      // found end of input
      break;                             // break from loop
    }

    if (strcmp("quit", cmd) == 0) {  // check for quit command
      if (echo) {
        printf("quit\n");
      }
      break;  // break from loop
    }

    else if (strcmp("add", cmd) == 0) {  // insertion
      fscanf(stdin, " %s", cmd);         // read string to add
      if (echo) {
        printf("add %s\n", cmd);
      }

      success = hashset_add(&set, cmd);  // call hashset add function
      if (!success) {                    // check for success
        printf("Data already present, no changes made\n");
      }
    }

    else if (strcmp("hashcode", cmd) == 0) {  // hashcode
      fscanf(stdin, "%s", cmd);               // read string to calculate hashcode
      if (echo) {
        printf("hashcode %s\n", cmd);
      }

      printf("%ld\n", (long)hashcode(cmd)); //prints long hashcode
    }

    else if (strcmp("save", cmd) == 0) {  // save to file
      fscanf(stdin, "%s", cmd);           // read string for file name
      if (echo) {
        printf("save %s\n", cmd);
      }

      hashset_save(&set, cmd);  // call hashset save function
    }

    else if (strcmp("load", cmd) == 0) {  // load from file
      fscanf(stdin, "%s", cmd);           // read string for filename
      if (echo) {
        printf("load %s\n", cmd);
      }

      success = hashset_load(&set, cmd);  // call hashset load function
      if (!success) {                     // check for success
        printf("load failed\n");
      }
    }

    else if (strcmp("next_prime", cmd) == 0) {  // next prime command
      fscanf(stdin, "%s", cmd);                 // reads number
      if (echo) {
        printf("next_prime %s\n", cmd);
      }

      printf("%d\n", next_prime(atoi(cmd))); //prints next prime number
    }

    else if (strcmp("expand", cmd) == 0) {  // expand command
      if (echo) {
        printf("expand\n");
      }
      hashset_expand(&set); //calls hashset expand function
    }

    else if (strcmp("clear", cmd) == 0) {  // clear command
      if (echo) {
        printf("clear\n");
      }

      //resets hashset fields
      hashset_free_fields(&set);
      hashset_init(&set, HASHSET_DEFAULT_TABLE_SIZE);
    }

    else if (strcmp("structure", cmd) == 0) {  // structure command
      if (echo) {
        printf("structure\n");
      }
      hashset_show_structure(&set); //prints hashset elems in table format
    }

    else if (strcmp("print", cmd) == 0) {  // print command
      if (echo) {
        printf("print\n");
      }
      hashset_write_data_ordered(&set, stdout); //prints hashset elems in added order
    }

    else if (strcmp("contains", cmd) == 0) {  // contains command
      fscanf(stdin, "%s", cmd);               // read string to check for
      if (echo) {
        printf("contains %s\n", cmd);
      }
      success = hashset_contains(&set, cmd);  // call contains function
      if (success) {                          // check for success
        printf("FOUND: %s\n", cmd);
      } else {
        printf("NOT PRESENT\n");
      }
    }

    else {  // unknown command
      if (echo) {
        printf("%s\n", cmd);
      }
      printf("unknown command %s\n", cmd);
    }
  }

  // end main while loop
  hashset_free_fields(&set);  // clean up the hashset
  return 0;
}
