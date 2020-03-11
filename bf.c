#include "bf.h"
#include <stdio.h>
#include <stdlib.h>

FILE *file;
cell *cells_head, *data;
label *labels_head, *loop;

/* Does not do any semantic checking.
   This program just interprets on the fly and if there are mismatched
   brackets, well, too bad. :) */

int main (int argc, char **argv) {
  /* Usage: bf file.bf */
  if (argc != 2) {
  	printf ("Usage: bf [filename]\n");
  	return -1;
  }

  /* Open file to interpret. */
  file = fopen (argv[1], "r");
  if (file == NULL) {
  	perror ("Error opening file");
  	return -1;
  }

  /* Initialize first data cell. */
  cells_head = malloc (sizeof (cell));
  if (cells_head == NULL) {
  	perror ("Error mallocing");
  	return -1;
  }
  data = cells_head;
  data->value = 0;
  data->next = NULL;
  data->prev = data;

  /* Initialize label stack (for jumping to beginning of loops). */
  labels_head = malloc (sizeof (label));
  if (labels_head == NULL) {
  	perror ("Error mallocing");
  	return -1;
  }
  loop = labels_head;
  loop->offset = 0;
  loop->next = NULL;
  loop->prev = NULL;

  /* Interpret. */
  int c;
  for (c = fgetc(file); c != -1; c = fgetc(file)) {
  	label *nextloop;
  	cell *nextcell;
  	switch (c) {
  	  case '>':
  	  	/* Create next cell if it doesn't exist. */
  	    if (data->next == NULL) {
  	      nextcell = malloc (sizeof (cell));
	      if (nextcell == NULL) {
	        perror ("Error mallocing");
	        return -1;
	      }
	      nextcell->value = 0;
  	      nextcell->next = NULL;
  	      nextcell->prev = data;
	      data->next = nextcell;
        }
        data = data->next;
  	  	break;
  	  case '<':
  	    data = data->prev;
  	  	break;
  	  case '+':
  	    data->value += 1;
  	  	break;
  	  case '-':
  	  	data->value -= 1;
  	  	break;
  	  case '.':
  	  	if (fputc (data->value, stdout) == -1) {
  	  	  perror ("Error writing to stdout");
  	  	  return -1;
  	  	}
  	  	break;
  	  case ',':
  	  	c = fgetc (stdin);
  	  	if (c == -1) {
  	  	  perror ("Error reading from stdin");
  	  	  return -1;
  	  	}
  	  	data->value = (char) c;
  	  	break;
  	  case '[':
  	    /* Initialize and push loop beginning onto label stack. */
  	    nextloop = malloc (sizeof (label));
		if (nextloop == NULL) {
		  perror ("Error mallocing");
		  return -1;
		}
		if ((nextloop->offset = ftell(file)) == -1) {
		  perror ("Error getting file position");
		  return -1;
		}
		nextloop->next = NULL;
		nextloop->prev = loop;
		loop->next = nextloop;
		loop = nextloop;
  	  	break;
  	  case ']':
  	  	if (loop->offset == 0) {
  	  	  perror("Mismatched loop end");
  	  	  return -1;
  	  	} else if (data->value == 0) {
  	  	  /* Continue if current cell has value 0. */
  	  	  loop = loop->prev;
  	  	  free(loop->next);
  	  	  loop->next = NULL;
  	  	} else if (fseek(file, loop->offset, 0) == -1) {
  	  	  /* Otherwise, jump to loop beginning. */
		  perror ("Error seeking");
		  return -1;
		}
  	  	break;
  	  default:
  	  	continue;
  	}
  }

  if (!feof (file)) {
    perror ("Error reading file");
    return -1;
  } else if (loop != labels_head) {
    perror ("Mismatched loop operators");
    return -1;
  } else {
    return 0;
  }
}