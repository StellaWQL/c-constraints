#include "stdarg.h"
#include "stdbool.h"
#include <stdlib.h>
/* Note:
 *   We just give you a **DEMO**.
 *   You can modify all the defintions if you want.
 * */
struct item {
	char *name;
    int *values;  // store values
    int numberOfValues;    // store the number of vals
};

typedef bool (*limit_fp_t)(int size, struct item *list);
typedef void (*travel_fp_t)(int size, struct item *list);

struct collection {
    struct item *items;
    int size;
    int capacity;
    limit_fp_t *limits;   // 限制函数的数组
    int num_limits;       // 限制函数的数量
};

void reset_output_buffer();
void collection_free(struct collection* c);
struct collection *collection_new(void);
int collection_add_variable(struct collection *c, char *var_name, int val0, ...);
int collection_add_limit(struct collection *c, limit_fp_t func);
int collection_travel_solution(struct collection *c, travel_fp_t func);
