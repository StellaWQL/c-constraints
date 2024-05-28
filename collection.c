#include "collection.h"
#include <string.h>
#include <stdio.h>
//#include "debug.h"

struct collection *collection_new(void) {
    struct collection *c = malloc(sizeof(struct collection));
    if (c == NULL) {
        return NULL;
    }

    c->items = malloc(sizeof(struct item) * 10); // 为 items 分配内存，size为10
    if (c->items == NULL) {
        free(c);
        return NULL;
    }

    c->size = 0;
    c->capacity = 10;
    c->limits = malloc(sizeof(limit_fp_t) * 4); // 为 limits 分配内存，size为4
    if (c->limits == NULL) {
        free(c->items);
        free(c);
        return NULL;
    }
    c->num_limits = 0;

    return c;
}

void collection_free(struct collection* c) {
    if (c != NULL) {
        for (int i = 0; i < c->size; i++) {
            free(c->items[i].values);
        }
        free(c->items);
        free(c->limits);
        free(c);
    }
}

int collection_add_variable(struct collection *c, char *var_name, int first_val, ...) {
    if (c == NULL || var_name == NULL) {
        return -1;
    }

    // check item's capacity，如果不够就double
    if (c->size == c->capacity) {
        int new_capacity = (c->capacity == 0) ? 1 : c->capacity * 2;
        void *new_items = realloc(c->items, new_capacity * sizeof(struct item));
        if (new_items == NULL) {
            return -1; // 检查一下放对没
        }
        c->items = new_items;
        c->capacity = new_capacity;
    }

    // 初始化新item
    struct item *new_item = &c->items[c->size];
    new_item->name = strdup(var_name); // 复制名称
    new_item->values = malloc(sizeof(int) * 10); // 假设初始values的size为10
    if (new_item->values == NULL) {
        free(new_item->name); // 释放已分配的名称内存
        return -1; // 检查一下放对没
    }
    new_item->numberOfValues = 0;

    // 处理可变参数
    va_list args;
    va_start(args, first_val);
    int value = first_val;
    const int max_args = 100; // 定义最大参数数量，以防无终止符
    while (value != -1 && new_item->numberOfValues < max_args) {
        if (new_item->numberOfValues == 10) { // 施工中！！！！！！！！！！！！！！！！！！！之后给item加个values_capacity再改
            int *new_values = realloc(new_item->values, 20 * sizeof(int)); // 增加容量
            if (new_values == NULL) {
                free(new_item->values);
                free(new_item->name);
                return -1; // 检查一下放对没
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

int collection_add_limit(struct collection *c, limit_fp_t func) {
    if (c == NULL || func == NULL) {
        return -1;
    }

    // 检查并动态扩容限制函数数组
    if (c->num_limits >= c->capacity) {
        int new_capacity = (c->capacity == 0) ? 4 : c->capacity * 2;
        limit_fp_t *new_limits = realloc(c->limits, new_capacity * sizeof(limit_fp_t));
        if (new_limits == NULL) {
            return -1;
        }
        c->limits = new_limits;
        c->capacity = new_capacity;
    }

    // 添加新的限制函数
    c->limits[c->num_limits++] = func;
    return 0;
}

// 调试信息到时候记得删掉
void log_debug(const char* message) {
    printf("DEBUG: %s\n", message);
}

//检查限制
bool check_limits(struct collection *c, struct item *current_combination) {
    log_debug("Entering check_limits");
    for (int i = 0; i < c->num_limits; i++) {
        if (!c->limits[i](c->size, current_combination)) {
            log_debug("Limit check failed");
            return false;
        }
    }
    log_debug("All limits passed");
    return true;
}

//求解
void solve(struct collection *c, struct item *current_combination, int current_index, travel_fp_t process_solution) {
    printf("DEBUG: Entering solve. current_index: %d, current_combination: %p\n", current_index, (void*)current_combination);

    for (int i = 0; i < c->size; i++) {
        printf("DEBUG: current_combination[%d].values = %p\n", i, (void*)current_combination[i].values);
    }

    if (current_index == c->size) {
        printf("DEBUG: Reached end of collection\n");
        if (check_limits(c, current_combination)) {
            process_solution(c->size, current_combination);
        }
        printf("DEBUG: Exiting solve. current_combination: %p\n", (void*)current_combination);
        return;
    }

    struct item *current_item = &c->items[current_index];
    for (int i = 0; i < current_item->numberOfValues; i++) {
        current_combination[current_index].name = current_item->name;
        current_combination[current_index].values[0] = current_item->values[i];  // 将values复制到current combination

        printf("DEBUG: Calling solve recursively. current_index: %d, current_combination: %p\n", current_index + 1, (void*)current_combination);
        solve(c, current_combination, current_index + 1, process_solution);

        // After recursive call
        printf("DEBUG: Returned from solve. current_index: %d, current_combination: %p\n", current_index + 1, (void*)current_combination);
    }
    printf("DEBUG: Exiting solve loop. current_combination: %p\n", (void*)current_combination);
}



int collection_travel_solution(struct collection *c, travel_fp_t process_solution) {
    printf("DEBUG: Entering collection_travel_solution\n");
    if (c == NULL || process_solution == NULL) {
        printf("DEBUG: Invalid arguments\n");
        return -1;
    }

    struct item *current_combination = malloc(sizeof(struct item) * c->size);
    if (current_combination == NULL) {
        printf("DEBUG: Failed to allocate memory for current_combination\n");
        return -1;
    }

    for (int i = 0; i < c->size; i++) {
        current_combination[i].name = c->items[i].name;
        current_combination[i].values = malloc(sizeof(int));  // 分配一个int大小的内存
        if (current_combination[i].values == NULL) {
            printf("DEBUG: Failed to allocate memory for values at index %d\n", i);
            for (int j = 0; j < i; j++) {
                if (current_combination[j].values != NULL) {
                    free(current_combination[j].values);
                    current_combination[j].values = NULL;
                    printf("DEBUG: Freed values at index %d\n", j);
                }
            }
            free(current_combination);
            printf("DEBUG: Freed current_combination\n");
            return -1;
        }
        current_combination[i].numberOfValues = 1;  // 更新numberOfValues为1
    }

    solve(c, current_combination, 0, process_solution);

    for (int i = 0; i < c->size; i++) {
        if (current_combination[i].values != NULL) {
            free(current_combination[i].values);
            printf("DEBUG: Freed values at index %d\n", i);
            current_combination[i].values = NULL;
        }
    }
    free(current_combination);
    printf("DEBUG: Exiting collection_travel_solution\n");
    return 0;
}
