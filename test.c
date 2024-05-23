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
    // 如果c为NULL，则后续测试不执行

    // 添加变量，检查是否成功
    CU_ASSERT_EQUAL_FATAL(collection_add_variable(c, "x", 10, 20, 30, -1), 0);
    CU_ASSERT_EQUAL(c->size, 1);
    CU_ASSERT_STRING_EQUAL(c->items[0].name, "x");
    CU_ASSERT_EQUAL(c->items[0].numberOfValues, 3);
    CU_ASSERT_EQUAL(c->items[0].values[0], 10);
    CU_ASSERT_EQUAL(c->items[0].values[1], 20);
    CU_ASSERT_EQUAL(c->items[0].values[2], 30);

    // 清理
    for (int i = 0; i < c->size; i++) {
        free(c->items[i].values);
    }
    free(c->items);
    free(c);
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


//两个是limitation function 用于add limit 测试
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

//void print_result(int size_of_list, struct item *list)
//{
//	// TODO: print info
//}

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

    CU_add_test(pSuite, "Test of add_variable with valid input", test_add_variable_valid_input);
    CU_add_test(pSuite, "Test of add_variable with NULL varname", test_add_variable_null);
    CU_add_test(pSuite, "Test of add_variable with no values", test_add_variable_no_values);
    CU_add_test(pSuite, "Test of add_variable with extreme values", test_add_variable_extreme_values);
    CU_add_test(pSuite, "Test of add_variable with missing terminator", test_add_variable_no_terminator);
    CU_add_test(pSuite, "Test Valid Add Limit", test_add_limit_valid);
    CU_add_test(pSuite, "Test Null Collection", test_add_limit_null_collection);
    CU_add_test(pSuite, "Test Null Function", test_add_limit_null_function);

    // set mode
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // run all tests
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}