#include "collection.h"
#include <string.h>
#include <stdio.h>

struct collection *collection_new(void)
{
    struct collection *c = malloc(sizeof(struct collection));
    if (c == NULL)
        return NULL;

    c->items = malloc(sizeof(struct item) * 10); /* Allocate memory for items, size 10 */
    if (c->items == NULL) {
        free(c);
        return NULL;
    }

    c->size = 0;
    c->capacity = 10;
    c->limits = malloc(sizeof(limit_fp_t) * 4); /* Allocate memory for limits, size 4 */
    if (c->limits == NULL) {
        free(c->items);
        free(c);
        return NULL;
    }
    c->num_limits = 0;

    return c;
}

void collection_free(struct collection *c)
{
    if (c != NULL) {
        for (int i = 0; i < c->size; i++) {
            free(c->items[i].values);
            free(c->items[i].name); /* Free the name memory */
        }
        free(c->items);
        free(c->limits);
        free(c);
    }
}

int collection_add_variable(struct collection *c, char *var_name, int first_val, ...)
{
    if (c == NULL || var_name == NULL)
        return -1;

    /* Check capacity of items and double if needed */
    if (c->size == c->capacity) {
        int new_capacity = (c->capacity == 0) ? 1 : c->capacity * 2;
        void *new_items = realloc(c->items, new_capacity * sizeof(struct item));
        if (new_items == NULL)
            return -1;
        c->items = new_items;
        c->capacity = new_capacity;
    }

    /* Initialize new item */
    struct item *new_item = &c->items[c->size];
    new_item->name = strdup(var_name); /* Duplicate name */
    new_item->values = malloc(sizeof(int) * 10); /* Initial values size 10 */
    if (new_item->values == NULL) {
        free(new_item->name);
        return -1;
    }
    new_item->numberOfValues = 0;
    new_item->values_capacity = 10; /* Set initial capacity */

    /* Handle variable arguments */
    va_list args;
    va_start(args, first_val);
    int value = first_val;
    const int max_args = 100; /* Define max arguments */
    while (value != -1 && new_item->numberOfValues < max_args) {
        if (new_item->numberOfValues == new_item->values_capacity) {
            new_item->values_capacity *= 2; /* Double the capacity */
            int *new_values = realloc(new_item->values, new_item->values_capacity * sizeof(int));
            if (new_values == NULL) {
                free(new_item->values);
                free(new_item->name);
                va_end(args);
                return -1;
            }
            new_item->values = new_values;
        }
        new_item->values[new_item->numberOfValues++] = value;
        value = va_arg(args, int);
    }
    va_end(args);

    if (value != -1) {
        free(new_item->values);
        free(new_item->name);
        return -1;
    }

    c->size++;
    return 0;
}

int collection_add_limit(struct collection *c, limit_fp_t func)
{
    if (c == NULL || func == NULL)
        return -1;

    /* Check and expand capacity of limits array */
    if (c->num_limits >= c->capacity) {
        int new_capacity = (c->capacity == 0) ? 4 : c->capacity * 2;
        limit_fp_t *new_limits = realloc(c->limits, new_capacity * sizeof(limit_fp_t));
        if (new_limits == NULL)
            return -1;
        c->limits = new_limits;
        c->capacity = new_capacity;
    }

    /* Add new limit function */
    c->limits[c->num_limits++] = func;
    return 0;
}

bool check_limits(struct collection *c, struct item *current_combination)
{
    for (int i = 0; i < c->num_limits; i++) {
        if (!c->limits[i](c->size, current_combination))
            return false;
    }
    return true;
}

void solve(struct collection *c, struct item *current_combination, int current_index, travel_fp_t process_solution)
{
    if (current_index == c->size) {
        if (check_limits(c, current_combination))
            process_solution(c->size, current_combination);
        return;
    }

    struct item *current_item = &c->items[current_index];
    for (int i = 0; i < current_item->numberOfValues; i++) {
        current_combination[current_index].name = current_item->name;
        current_combination[current_index].values[0] = current_item->values[i]; /* Copy values to current combination */
        solve(c, current_combination, current_index + 1, process_solution);
    }
}

int collection_travel_solution(struct collection *c, travel_fp_t process_solution)
{
    if (c == NULL || process_solution == NULL)
        return -1;

    struct item *current_combination = malloc(sizeof(struct item) * c->size);
    if (current_combination == NULL)
        return -1;

    for (int i = 0; i < c->size; i++) {
        current_combination[i].name = c->items[i].name;
        current_combination[i].values = malloc(sizeof(int)); /* Allocate memory for one int */
        if (current_combination[i].values == NULL) {
            for (int j = 0; j < i; j++) {
                free(current_combination[j].values);
                current_combination[j].values = NULL;
            }
            free(current_combination);
            return -1;
        }
        current_combination[i].numberOfValues = 1; /* Set numberOfValues to 1 */
        current_combination[i].values_capacity = 1; /* Set initial capacity */
    }

    solve(c, current_combination, 0, process_solution);

    for (int i = 0; i < c->size; i++) {
        free(current_combination[i].values);
        current_combination[i].values = NULL;
    }
    free(current_combination);
    return 0;
}
