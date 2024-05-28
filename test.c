#include <CUnit/Basic.h>
#include "collection.h"
#include <limits.h>
#include <stdlib.h>

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

// 测试 ADD VARIABLE
// add_variable 有效输入
void test_add_variable_valid_input(void) {
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    // 添加变量，检查是否成功
    CU_ASSERT_EQUAL_FATAL(collection_add_variable(c, "x", 10, 20, 30, -1), 0);
    CU_ASSERT_EQUAL(c->size, 1);
    CU_ASSERT_STRING_EQUAL(c->items[0].name, "x");
    CU_ASSERT_EQUAL(c->items[0].numberOfValues, 3);
    CU_ASSERT_EQUAL(c->items[0].values[0], 10);
    CU_ASSERT_EQUAL(c->items[0].values[1], 20);
    CU_ASSERT_EQUAL(c->items[0].values[2], 30);

    // 由 collection_free 完成所有清理工作
    collection_free(c);
}


// add_variable varname是NULL
void test_add_variable_null() {
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    int result = collection_add_variable(c, NULL, 10, -1); // 测试Null作为变量名
    CU_ASSERT_NOT_EQUAL(result, 0); // should fail

    collection_free(c);
}

// add_variable 没有values
void test_add_variable_no_values() {
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    int result = collection_add_variable(c, "x", -1); // 没有值，只有terminator
    CU_ASSERT_EQUAL(result, 0); // should pass
    CU_ASSERT_EQUAL(c->items[0].numberOfValues, 0); // 值的数量应该为0

    collection_free(c);
}

// add_variable 极端数值
void test_add_variable_extreme_values() {
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    int result = collection_add_variable(c, "x", INT_MAX, INT_MIN, -1); // 测试极端值
    CU_ASSERT_EQUAL(result, 0); // should pass
    CU_ASSERT_EQUAL(c->items[0].values[0], INT_MAX);
    CU_ASSERT_EQUAL(c->items[0].values[1], INT_MIN);

    collection_free(c);
}

// add_variable 没有 terminator -1
void test_add_variable_no_terminator() {
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    int result = collection_add_variable(c, "x", 10, 20, 30); // 没有-1结束
    CU_ASSERT_NOT_EQUAL(result, 0); // should fail

    collection_free(c);
}


//两个是limitation function 用于add limit 测试，还有个print的是遍历
/*
 * size_of_list: the length of variable-list
 * list: item list like [Item("x", 1), Item("y", 2), ....]
 */

bool x_bigger_than_y(int size_of_list, struct item *list) {
    int x_val = 0, y_val = 0;
    bool x_found = false, y_found = false;

    for (int i = 0; i < size_of_list; i++) {
        if (strcmp(list[i].name, "x") == 0) {
            x_val = list[i].values[0];
            x_found = true;
        } else if (strcmp(list[i].name, "y") == 0) {
            y_val = list[i].values[0];
            y_found = true;
        }
    }

    return x_found && y_found && (x_val > y_val);
}

bool y_bigger_than_z(int size_of_list, struct item *list) {
    int y_val = 0, z_val = 0;
    bool y_found = false, z_found = false;

    for (int i = 0; i < size_of_list; i++) {
        if (strcmp(list[i].name, "y") == 0) {
            y_val = list[i].values[0];
            y_found = true;
        } else if (strcmp(list[i].name, "z") == 0) {
            z_val = list[i].values[0];
            z_found = true;
        }
    }

    return y_found && z_found && (y_val > z_val);
}

void print_result(int size_of_list, struct item *list) {
    printf("Output from collection_travel_solution:\n");
    for (int i = 0; i < size_of_list; i++) {
        printf("%s = %d", list[i].name, list[i].values[0]);  // 假设每个 item 至少有一个值
        if (i < size_of_list - 1) {
            printf(", ");
        }
    }
    printf("\n");
}

//测试这些helper function
void test_x_bigger_than_y(void) {
    struct item items[] = {
            {"x", (int[]){5}, 1},
            {"y", (int[]){3}, 1}
    };

    bool result = x_bigger_than_y(2, items);
    CU_ASSERT_TRUE(result);  // x is greater than y, expect true

    items[0].values[0] = 2;  // Change x to be less than y
    result = x_bigger_than_y(2, items);
    CU_ASSERT_FALSE(result); // Now x is less than y, expect false
}

void test_y_bigger_than_z(void) {
    struct item items[] = {
            {"y", (int[]){10}, 1},
            {"z", (int[]){5}, 1}
    };

    bool result = y_bigger_than_z(2, items);
    CU_ASSERT_TRUE(result);  // y is greater than z, expect true

    items[0].values[0] = 3;  // Change y to be less than z
    result = y_bigger_than_z(2, items);
    CU_ASSERT_FALSE(result); // Now y is less than z, expect false
}

void test_print_result_all_one_value(void) {
    struct item items[] = {
            {"x", (int[]){20}, 1},
            {"y", (int[]){12}, 1},
            {"z", (int[]){27}, 1}
    };
    int size_of_list = sizeof(items) / sizeof(items[0]);

    // 调用 print_result 函数，观察输出
    print_result(size_of_list, items);
}

void test_print_result_complex_value(void) {
    // 创建包含多个值的 items
    struct item items[] = {
            {"x", (int[]){370, 15, 20}, 3},
            {"y", (int[]){25, 30}, 2},
            {"z", (int[]){35}, 1}
    };
    int size_of_list = sizeof(items) / sizeof(items[0]);

    // 调用 print_result 函数，观察输出
    print_result(size_of_list, items);
}






// 测试 ADD LIMIT
// add_limit 有效输入
void test_add_limit_valid(void) {
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    // 添加一个有效的限制函数
    int result = collection_add_limit(c, x_bigger_than_y);
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_EQUAL(c->num_limits, 1);

    // free
    collection_free(c);
}

// add_limit 当 collection 是 NULL 时
void test_add_limit_null_collection(void) {
    int result = collection_add_limit(NULL, x_bigger_than_y);
    CU_ASSERT_NOT_EQUAL(result, 0);  // should fail
}

// add_limit 当 limit function 是 NULL 时
void test_add_limit_null_function(void) {
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    int result = collection_add_limit(c, NULL);
    CU_ASSERT_NOT_EQUAL(result, 0);  // should fail

    collection_free(c);
}

// TRAVEL SOLUTION

void test_collection_travel_solution_1_limit(void) {
    struct collection *collection = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(collection);

    // 添加变量
    collection_add_variable(collection, "x", 1, 2, 3, 5, -1);
    collection_add_variable(collection, "y", 3, 5, -1);
    collection_add_variable(collection, "z", 1, 2, 3, 5, -1);

    // 添加限制条件
    collection_add_limit(collection, x_bigger_than_y);

    printf("Expected output (if limits are working correctly):\n");
    printf("x = 5, y = 3, z = 1\nx = 5, y = 3, z = 2\nx = 5, y = 3, z = 3\nx = 5, y = 3, z = 5\n");
    printf("Actual output:\n");
    collection_travel_solution(collection, print_result);
    collection_free(collection);
}

void test_collection_travel_solution_2_limits(void) {
    struct collection *collection = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(collection);

    // 添加变量
    collection_add_variable(collection, "x", 1, 2, 3, 5, -1);
    collection_add_variable(collection, "y", 3, 5, -1);
    collection_add_variable(collection, "z", 1, 2, 3, 5, -1);

    // 添加限制条件
    collection_add_limit(collection, x_bigger_than_y);
    collection_add_limit(collection, y_bigger_than_z);

    printf("Expected output (if limits are working correctly):\n");
    printf("x = 5, y = 3, z = 1\nx = 5, y = 3, z = 2\n");
    printf("Actual output:\n");
    collection_travel_solution(collection, print_result);
    collection_free(collection);
}

int main() {
    // initialize
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    // ！！！！！！！！！！！！！！施工中，记得最后分一下
    CU_pSuite pSuite = CU_add_suite("Variable Test Suite", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_add_test(pSuite, "Test of x_bigger_than_y", test_x_bigger_than_y);
    CU_add_test(pSuite, "Test of y_bigger_than_z", test_y_bigger_than_z);
    CU_add_test(pSuite, "Test of add_variable with valid input", test_add_variable_valid_input);
    CU_add_test(pSuite, "Test of add_variable with NULL varname", test_add_variable_null);
    CU_add_test(pSuite, "Test of add_variable with no values", test_add_variable_no_values);
    CU_add_test(pSuite, "Test of add_variable with extreme values", test_add_variable_extreme_values);
    CU_add_test(pSuite, "Test of add_variable with missing terminator", test_add_variable_no_terminator);
    CU_add_test(pSuite, "Test Valid Add Limit", test_add_limit_valid);
    CU_add_test(pSuite, "Test Null Collection", test_add_limit_null_collection);
    CU_add_test(pSuite, "Test Null Function", test_add_limit_null_function);
    CU_add_test(pSuite, "Test Print Result Simple One", test_print_result_all_one_value);
    CU_add_test(pSuite, "Test Print Result Complex One", test_print_result_complex_value);
    //CU_add_test(pSuite, "Test Collection Travel Solution", test_collection_travel_solution_simple);
    CU_add_test(pSuite, "Test Collection Travel Solution 1 limit", test_collection_travel_solution_1_limit);
    CU_add_test(pSuite, "Test Collection Travel Solution 2 limits", test_collection_travel_solution_2_limits);




    // set mode
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // run all tests
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}