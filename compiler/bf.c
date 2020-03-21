#include <stdio.h>

static int indent_level = 0;
static FILE *c_file;

/* Helper functions for compiling to C. */
static void emit_indent (void);
static void emit_prologue (void);
static void emit_epilogue (void);
static void emit_next_cell (void);

int main(int argc, char const *argv[])
{
  /* Open BF file to compile from. */
  FILE *bf_file = fopen (argv[1], "r");
  if (bf_file == NULL)
    bf_file = stdin;

  /* Open C file to compile to. */
  c_file = fopen ("a.c", "w");
  if (c_file == NULL) {
    perror ("Error opening output file");
    return -1;
  }

  emit_prologue();

  int c;
  for (c = fgetc(bf_file); c != -1; c = fgetc(bf_file)) {
    switch (c) {
    case '>':
      emit_indent ();
      fputs ("next_cell ();\n", c_file);
      emit_indent ();
      fputs ("head = head->next;\n", c_file);
      break;
    case '<':
      emit_indent ();
      fputs ("head = head->prev;\n", c_file);
      break;
    case '+':
      emit_indent ();
      fputs ("head->value += 1;\n", c_file);
      break;
    case '-':
      emit_indent ();
      fputs ("head->value -= 1;\n", c_file);
      break;
    case '.':
      emit_indent ();
      fputs ("putchar (head->value);\n", c_file);
      break;
    case ',':
      emit_indent ();
      fputs ("head->value = (char) getchar ();\n", c_file);
      break;
    case '[':
      emit_indent ();
      fputs ("while (head->value > 0)\n", c_file);
      emit_indent ();
      fputs ("{\n", c_file);
      indent_level += 1;
      break;
    case ']':
      indent_level -= 1;
      emit_indent ();
      fputs ("}\n", c_file);
      break;
    default:
      continue;
    }
  }

  emit_epilogue();
  return 0;
}

/* Writes 2 * INDENT spaces to C_FILE. */
void emit_indent (void)
{
  for (int i = 0; i < indent_level; i++)
    fputs("  ", c_file);
}

/* Writes the BF-C program prologue to C_FILE.*/
void emit_prologue (void)
{
  // Emit includes.
  fputs ("#include <stdio.h>\n", c_file);
  fputs ("#include <stdlib.h>\n\n", c_file);

  // Emit struct defs.
  fputs ("typedef struct cell {\n", c_file);
  fputs ("  char value;\n", c_file);
  fputs ("  struct cell *next;\n", c_file);
  fputs ("  struct cell *prev;\n", c_file);
  fputs ("} cell;\n\n", c_file);

  // Emit memory list.
  fputs ("cell *head;\n", c_file);

  // Emit a helper function for ">".
  emit_next_cell ();

  // Begin to emit main.
  fputs ("int main (void)\n", c_file);
  fputs ("{\n", c_file);
  indent_level += 1;

  // Emit memory list initialization.
  emit_indent ();
  fputs ("head = malloc (sizeof (cell));\n", c_file);
  emit_indent ();
  fputs ("head->value = 0;\n", c_file);
  emit_indent ();
  fputs ("head->next = NULL;\n", c_file);
  emit_indent();
  fputs ("head->prev = head;\n", c_file);
}

/* Writes the closing brace of the C program to C_FILE. */
void emit_epilogue (void)
{
  fputs("}\n", c_file);
}

/* Writes the function that ensures the next cell exists to C_FILE. */
void emit_next_cell (void)
{
  fputs("void next_cell (void)\n", c_file);
  fputs("{\n", c_file);
  indent_level += 1;

  emit_indent();
  fputs("cell *tmp;\n\n", c_file);

  emit_indent();
  fputs("if (head->next == NULL)\n", c_file);
  emit_indent();
  fputs("{\n", c_file);
  indent_level += 1;

  emit_indent();
  fputs("tmp = malloc (sizeof (cell));\n", c_file);
  emit_indent();
  fputs("tmp->value = 0;\n", c_file);
  emit_indent();
  fputs("tmp->next = NULL;\n", c_file);
  emit_indent();
  fputs("tmp->prev = head;\n", c_file);
  emit_indent();
  fputs("head->next = tmp;\n", c_file);

  indent_level -= 1;
  emit_indent();
  fputs("}\n", c_file);

  indent_level -= 1;
  emit_indent();
  fputs("}\n\n", c_file);
}



