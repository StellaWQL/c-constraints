#include "stdarg.h"
#include "stdbool.h"

/* Note:
 *   We just give you a **DEMO**.
 *   You can modify all the defintions if you want.
 * */
struct item {
	char *name;
	int val;
};

struct collection {
	/* You can define any variables you want. */
};

typedef bool (*limit_fp_t)(int size, struct item *list);
typedef void (*travel_fp_t)(int size, struct item *list);

struct collection *collection_new(void);
int collection_add_variable(struct collection *c, char *var_name, int val0, ...);
int collection_add_limit(struct collection *c, limit_fp_t func);
int collection_travel_solution(struct collection *c, travel_fp_t func);
