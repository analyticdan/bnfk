/* Data cell. */
struct cell {
	char value;
	struct cell *next;
	struct cell *prev;
};
typedef struct cell cell;

/* Labels for loops. */
struct label {
	long int offset;
	struct label *next;
	struct label *prev;
};
typedef struct label label;

/* BF operations. */
cell* gt (cell *data);
cell* lt (cell *data);
label* lbrack (label *data);
label* rbrack (cell *cdata, label *ldata);