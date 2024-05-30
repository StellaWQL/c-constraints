#include "stdarg.h"
#include "stdbool.h"
#include <stdlib.h>

/* Note:
 *   This is a **DEMO**.
 *   You can modify all the definitions if you want.
 * */

struct item {
    char *name;
    int *values;           /* store values */
    int numberOfValues;    /* store the number of values */
    int values_capacity;   /* capacity of the values array */
};

typedef bool (*limit_fp_t)(int size, struct item *list);
typedef void (*travel_fp_t)(int size, struct item *list);

struct collection {
    struct item *items;
    int size;
    int capacity;
    limit_fp_t *limits;    /* array of limit functions */
    int num_limits;        /* number of limit functions */
};

void reset_output_buffer();
void collection_free(struct collection *c);
struct collection *collection_new(void);
int collection_add_variable(struct collection *c, char *var_name, int val0, ...);
int collection_add_limit(struct collection *c, limit_fp_t func);
int collection_travel_solution(struct collection *c, travel_fp_t func);
