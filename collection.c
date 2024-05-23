#include "collection.h"
#include <string.h>
struct collection *collection_new(void) {
    struct collection *c = malloc(sizeof(struct collection));
    if (c == NULL) {
        return NULL;
    }

    c->items = malloc(sizeof(struct item) * 10);  // 初始容量设置为10
    if (c->items == NULL) {
        free(c);
        return NULL;
    }

    c->size = 0;
    c->capacity = 10;
    c->limits = malloc(sizeof(limit_fp_t) * 4); // 为 limits 分配初始内存
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
        free(c->limits);  // 释放 limits 数组
        free(c);
    }
}



int collection_add_variable(struct collection *c, char *var_name, int first_val, ...) {
    if (c == NULL || var_name == NULL) {
        return -1;
    }

    // 检查容量并动态扩展
    if (c->size == c->capacity) {
        int new_capacity = (c->capacity == 0) ? 1 : c->capacity * 2;
        void *new_items = realloc(c->items, new_capacity * sizeof(struct item));
        if (new_items == NULL) {
            return -1; // 无法分配更多内存
        }
        c->items = new_items;
        c->capacity = new_capacity;
    }

    // 初始化新item
    struct item *new_item = &c->items[c->size];
    new_item->name = strdup(var_name); // 复制名称
    new_item->values = malloc(sizeof(int) * 10); // 假设初始容量为10
    if (new_item->values == NULL) {
        free(new_item->name); // 释放已分配的名称内存
        return -1; // 内存分配失败
    }
    new_item->numberOfValues = 0;

    // 处理可变参数
    va_list args;
    va_start(args, first_val);
    int value = first_val;
    const int max_args = 100; // 定义最大参数数量，以防无终止符
    while (value != -1 && new_item->numberOfValues < max_args) {
        if (new_item->numberOfValues == 10) { // 需要更多空间
            int *new_values = realloc(new_item->values, 20 * sizeof(int)); // 增加容量
            if (new_values == NULL) {
                free(new_item->values);
                free(new_item->name);
                return -1; // 内存分配失败
            }
            new_item->values = new_values;
        }
        new_item->values[new_item->numberOfValues++] = value;
        value = va_arg(args, int);
    }
    va_end(args);

    if (value != -1) {
        // 如果未达到终止符而退出循环，则视为错误
        free(new_item->values);
        free(new_item->name);
        return -1;
    }

    c->size++;
    return 0;
}

int collection_add_limit(struct collection *c, limit_fp_t func) {
    if (c == NULL || func == NULL) {
        return -1; // 输入无效
    }

    // 检查并动态扩容限制函数数组
    if (c->num_limits >= c->capacity) {
        int new_capacity = (c->capacity == 0) ? 4 : c->capacity * 2;
        limit_fp_t *new_limits = realloc(c->limits, new_capacity * sizeof(limit_fp_t));
        if (new_limits == NULL) {
            return -1; // 内存分配失败
        }
        c->limits = new_limits;
        c->capacity = new_capacity;
    }

    // 添加新的限制函数
    c->limits[c->num_limits++] = func;
    return 0; // 添加成功
}
//int collection_travel_solution(struct collection *c, travel_fp_t func)
//{
//
//}
