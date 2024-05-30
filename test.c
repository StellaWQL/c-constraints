#include <CUnit/Basic.h>
#include "collection.h"
#include <limits.h>
#include <stdlib.h>

int init_suite(void) { return 0; }
int clean_suite(void) { return 0; }

/* Test ADD VARIABLE */

/* Test add_variable with valid input */
void test_add_variable_valid_input(void)
{
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    /* Add variable and check if successful */
    CU_ASSERT_EQUAL_FATAL(collection_add_variable(c, "x", 10, 20, 30, -1), 0);
    CU_ASSERT_EQUAL(c->size, 1);
    CU_ASSERT_STRING_EQUAL(c->items[0].name, "x");
    CU_ASSERT_EQUAL(c->items[0].numberOfValues, 3);
    CU_ASSERT_EQUAL(c->items[0].values[0], 10);
    CU_ASSERT_EQUAL(c->items[0].values[1], 20);
    CU_ASSERT_EQUAL(c->items[0].values[2], 30);

    /* Free collection */
    collection_free(c);
}

/* Test add_variable with NULL varname */
void test_add_variable_null(void)
{
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    int result = collection_add_variable(c, NULL, 10, -1); /* Test NULL as variable name */
    CU_ASSERT_NOT_EQUAL(result, 0); /* Should fail */

    collection_free(c);
}

/* Test add_variable with no values */
void test_add_variable_no_values(void)
{
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    int result = collection_add_variable(c, "x", -1); /* No values, only terminator */
    CU_ASSERT_EQUAL(result, 0); /* Should pass */
    CU_ASSERT_EQUAL(c->items[0].numberOfValues, 0); /* Number of values should be 0 */

    collection_free(c);
}

/* Test add_variable with extreme values */
void test_add_variable_extreme_values(void)
{
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    int result = collection_add_variable(c, "x", INT_MAX, INT_MIN, -1); /* Test extreme values */
    CU_ASSERT_EQUAL(result, 0); /* Should pass */
    CU_ASSERT_EQUAL(c->items[0].values[0], INT_MAX);
    CU_ASSERT_EQUAL(c->items[0].values[1], INT_MIN);

    collection_free(c);
}

/* Test add_variable with no terminator */
void test_add_variable_no_terminator(void)
{
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    int result = collection_add_variable(c, "x", 10, 20, 30); /* No -1 terminator */
    CU_ASSERT_NOT_EQUAL(result, 0); /* Should fail */

    collection_free(c);
}

/* Helper functions for adding limits */

/* size_of_list: the length of variable-list */
/* list: item list like [Item("x", 1), Item("y", 2), ....] */

bool x_bigger_than_y(int size_of_list, struct item *list)
{
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

bool y_bigger_than_z(int size_of_list, struct item *list)
{
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

bool x_equal_to_y(int size_of_list, struct item *list)
{
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

    return x_found && y_found && (x_val == y_val);
}

void print_result(int size_of_list, struct item *list)
{
    for (int i = 0; i < size_of_list; i++) {
        printf("%s = %d", list[i].name, list[i].values[0]);
        if (i < size_of_list - 1)
            printf(", ");
    }
    printf("\n");
}

void sum_values(int size_of_list, struct item *list)
{
    int sum = 0;
    for (int i = 0; i < size_of_list; i++) {
        sum += list[i].values[0];
    }
    printf("Sum = %d\n", sum);
}

void max_value(int size_of_list, struct item *list)
{
    int max_val = INT_MIN;
    for (int i = 0; i < size_of_list; i++) {
        if (list[i].values[0] > max_val) {
            max_val = list[i].values[0];
        }
    }
    printf("Max value = %d\n", max_val);
}

void count_combinations(int size_of_list, struct item *list)
{
    static int count = 0;
    count++;
    printf("Combination %d: ", count);
    for (int i = 0; i < size_of_list; i++) {
        printf("%s = %d", list[i].name, list[i].values[0]);
        if (i < size_of_list - 1)
            printf(", ");
    }
    printf("\n");
}


/* Test helper functions */

void test_x_bigger_than_y(void)
{
    struct item items[] = {
            {"x", (int[]){5}, 1},
            {"y", (int[]){3}, 1}
    };

    bool result = x_bigger_than_y(2, items);
    CU_ASSERT_TRUE(result); /* x is greater than y, expect true */

    items[0].values[0] = 2; /* Change x to be less than y */
    result = x_bigger_than_y(2, items);
    CU_ASSERT_FALSE(result); /* Now x is less than y, expect false */
}

void test_y_bigger_than_z(void)
{
    struct item items[] = {
            {"y", (int[]){10}, 1},
            {"z", (int[]){5}, 1}
    };

    bool result = y_bigger_than_z(2, items);
    CU_ASSERT_TRUE(result); /* y is greater than z, expect true */

    items[0].values[0] = 3; /* Change y to be less than z */
    result = y_bigger_than_z(2, items);
    CU_ASSERT_FALSE(result); /* Now y is less than z, expect false */
}

void test_print_result_all_one_value(void)
{
    struct item items[] = {
            {"x", (int[]){20}, 1},
            {"y", (int[]){12}, 1},
            {"z", (int[]){27}, 1}
    };
    int size_of_list = sizeof(items) / sizeof(items[0]);

    /* Call print_result and observe the output */
    print_result(size_of_list, items);
}

void test_print_result_complex_value(void)
{
    /* Create items with multiple values */
    struct item items[] = {
            {"x", (int[]){370, 15, 20}, 3},
            {"y", (int[]){25, 30}, 2},
            {"z", (int[]){35}, 1}
    };
    int size_of_list = sizeof(items) / sizeof(items[0]);

    /* Call print_result and observe the output */
    print_result(size_of_list, items);
}

/* Test ADD LIMIT */

/* Test add_limit with valid input */
void test_add_limit_valid(void)
{
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    /* Add a valid limit function */
    int result = collection_add_limit(c, x_bigger_than_y);
    CU_ASSERT_EQUAL(result, 0);
    CU_ASSERT_EQUAL(c->num_limits, 1);

    /* Free collection */
    collection_free(c);
}

/* Test add_limit when collection is NULL */
void test_add_limit_null_collection(void)
{
    int result = collection_add_limit(NULL, x_bigger_than_y);
    CU_ASSERT_NOT_EQUAL(result, 0); /* Should fail */
}

/* Test add_limit when limit function is NULL */
void test_add_limit_null_function(void)
{
    struct collection *c = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(c);

    int result = collection_add_limit(c, NULL);
    CU_ASSERT_NOT_EQUAL(result, 0); /* Should fail */

    collection_free(c);
}

/* TRAVEL SOLUTION */

void test_collection_travel_solution_1_limit_x_bigger_than_y(void)
{
    struct collection *collection = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(collection);

    /* Add variables */
    collection_add_variable(collection, "x", 1, 2, 3, 5, -1);
    collection_add_variable(collection, "y", 3, 5, -1);
    collection_add_variable(collection, "z", 1, 2, 3, 5, -1);

    /* Add limit condition */
    collection_add_limit(collection, x_bigger_than_y);

    printf("Expected output (if limits are working correctly):\n");
    printf("x = 5, y = 3, z = 1\nx = 5, y = 3, z = 2\nx = 5, y = 3, z = 3\nx = 5, y = 3, z = 5\n");
    printf("Actual output:\n");
    collection_travel_solution(collection, print_result);
    collection_free(collection);
}

void test_collection_travel_solution_1_limit_y_bigger_than_z(void)
{
    struct collection *collection = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(collection);

    /* Add variables */
    collection_add_variable(collection, "x", 1, 2, 3, 5, -1);
    collection_add_variable(collection, "y", 3, 5, -1);
    collection_add_variable(collection, "z", 1, 2, 3, 5, -1);

    /* Add limit condition */
    collection_add_limit(collection, y_bigger_than_z);

    printf("Expected output (if limits are working correctly):\n");
    printf("x = 1, y = 3, z = 1\n"
           "x = 1, y = 3, z = 2\n"
           "x = 1, y = 5, z = 1\n"
           "x = 1, y = 5, z = 2\n"
           "x = 1, y = 5, z = 3\n"
           "x = 2, y = 3, z = 1\n"
           "x = 2, y = 3, z = 2\n"
           "x = 2, y = 5, z = 1\n"
           "x = 2, y = 5, z = 2\n"
           "x = 2, y = 5, z = 3\n"
           "x = 3, y = 3, z = 1\n"
           "x = 3, y = 3, z = 2\n"
           "x = 3, y = 5, z = 1\n"
           "x = 3, y = 5, z = 2\n"
           "x = 3, y = 5, z = 3\n"
           "x = 5, y = 3, z = 1\n"
           "x = 5, y = 3, z = 2\n"
           "x = 5, y = 5, z = 1\n"
           "x = 5, y = 5, z = 2\n"
           "x = 5, y = 5, z = 3\n");
    printf("Actual output:\n");
    collection_travel_solution(collection, print_result);
    collection_free(collection);
}

void test_collection_travel_solution_2_limits(void)
{
    struct collection *collection = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(collection);

    /* Add variables */
    collection_add_variable(collection, "x", 1, 2, 3, 5, -1);
    collection_add_variable(collection, "y", 3, 5, -1);
    collection_add_variable(collection, "z", 1, 2, 3, 5, -1);

    /* Add limit conditions */
    collection_add_limit(collection, x_bigger_than_y);
    collection_add_limit(collection, y_bigger_than_z);

    printf("Expected output (if limits are working correctly):\n");
    printf("x = 5, y = 3, z = 1\nx = 5, y = 3, z = 2\n");
    printf("Actual output:\n");
    collection_travel_solution(collection, print_result);
    collection_free(collection);
}

void test_collection_travel_solution_1_limit_x_equal_to_y(void)
{
    struct collection *collection = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(collection);

    /* Add variables */
    collection_add_variable(collection, "x", 1, 2, 3, 5, -1);
    collection_add_variable(collection, "y", 3, 5, -1);
    collection_add_variable(collection, "z", 1, 2, 3, 5, -1);

    /* Add limit condition */
    collection_add_limit(collection, x_equal_to_y);

    printf("Expected output (if limits are working correctly):\n");
    printf("x = 3, y = 3, z = 1\n"
           "x = 3, y = 3, z = 2\n"
           "x = 3, y = 3, z = 3\n"
           "x = 3, y = 3, z = 5\n"
           "x = 5, y = 5, z = 1\n"
           "x = 5, y = 5, z = 2\n"
           "x = 5, y = 5, z = 3\n"
           "x = 5, y = 5, z = 5\n");
    printf("Actual output:\n");
    collection_travel_solution(collection, print_result);
    collection_free(collection);
}

void test_collection_travel_solution_sum_values(void)
{
    struct collection *collection = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(collection);

    collection_add_variable(collection, "x", 1, 2, 3, 5, -1);
    collection_add_variable(collection, "y", 3, 5, -1);
    collection_add_variable(collection, "z", 1, 2, 3, 5, -1);

    collection_add_limit(collection, x_bigger_than_y);

    printf("Expected output (if limits are working correctly):\n");
    printf("Sum = 9\nSum = 10\nSum = 11\nSum = 13");
    printf("Actual output:\n");
    printf("Sum of values for valid combinations:\n");
    collection_travel_solution(collection, sum_values);
    collection_free(collection);
}

void test_collection_travel_solution_max_value(void)
{
    struct collection *collection = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(collection);

    collection_add_variable(collection, "x", 1, 2, 3, 5, -1);
    collection_add_variable(collection, "y", 3, 5, -1);
    collection_add_variable(collection, "z", 1, 2, 3, 5, -1);

    collection_add_limit(collection, y_bigger_than_z);

    printf("Expected output (if limits are working correctly):\n");
    printf("Sum = 9\nSum = 10\nSum = 11\nSum = 13");
    printf("Max value in valid combinations:\n");
    collection_travel_solution(collection, max_value);
    collection_free(collection);
}

void test_collection_travel_solution_count_combinations(void)
{
    struct collection *collection = collection_new();
    CU_ASSERT_PTR_NOT_NULL_FATAL(collection);

    collection_add_variable(collection, "x", 1, 2, 3, 5, -1);
    collection_add_variable(collection, "y", 3, 5, -1);
    collection_add_variable(collection, "z", 1, 2, 3, 5, -1);

    collection_add_limit(collection, x_equal_to_y);

    printf("Count of valid combinations:\n");
    collection_travel_solution(collection, count_combinations);
    collection_free(collection);
}



int main(void) {
    /* Initialize CUnit test registry */
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    /* Add Variable Test Suite */
    CU_pSuite pSuite_Variable = CU_add_suite("Variable Test Suite", init_suite, clean_suite);
    if (NULL == pSuite_Variable) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add Limit Test Suite */
    CU_pSuite pSuite_Limit = CU_add_suite("Limit Test Suite", init_suite, clean_suite);
    if (NULL == pSuite_Limit) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add Print Result Test Suite */
    CU_pSuite pSuite_Print = CU_add_suite("Print Result Test Suite", init_suite, clean_suite);
    if (NULL == pSuite_Print) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add Travel Solution Test Suite */
    CU_pSuite pSuite_Travel = CU_add_suite("Travel Solution Test Suite", init_suite, clean_suite);
    if (NULL == pSuite_Travel) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    /* Add tests to Variable Test Suite */
    CU_add_test(pSuite_Variable, "Test of add_variable with valid input", test_add_variable_valid_input);
    CU_add_test(pSuite_Variable, "Test of add_variable with NULL varname", test_add_variable_null);
    CU_add_test(pSuite_Variable, "Test of add_variable with no values", test_add_variable_no_values);
    CU_add_test(pSuite_Variable, "Test of add_variable with extreme values", test_add_variable_extreme_values);
    CU_add_test(pSuite_Variable, "Test of add_variable with missing terminator", test_add_variable_no_terminator);

    /* Add tests to Limit Test Suite */
    CU_add_test(pSuite_Limit, "Test of x_bigger_than_y", test_x_bigger_than_y);
    CU_add_test(pSuite_Limit, "Test of y_bigger_than_z", test_y_bigger_than_z);
    CU_add_test(pSuite_Limit, "Test Valid Add Limit", test_add_limit_valid);
    CU_add_test(pSuite_Limit, "Test Null Collection", test_add_limit_null_collection);
    CU_add_test(pSuite_Limit, "Test Null Function", test_add_limit_null_function);

    /* Add tests to Print Result Test Suite */
    CU_add_test(pSuite_Print, "Test Print Result Simple One", test_print_result_all_one_value);
    CU_add_test(pSuite_Print, "Test Print Result Complex One", test_print_result_complex_value);

    /* Add tests to Travel Solution Test Suite */
    CU_add_test(pSuite_Travel, "Test Collection Travel Solution 1 limit: x bigger than y", test_collection_travel_solution_1_limit_x_bigger_than_y);
    CU_add_test(pSuite_Travel, "Test Collection Travel Solution 2 limits", test_collection_travel_solution_2_limits);
    CU_add_test(pSuite_Travel, "Test Collection Travel Solution 1 limit: y bigger than z", test_collection_travel_solution_1_limit_y_bigger_than_z);
    CU_add_test(pSuite_Travel, "Test Collection Travel Solution 1 limit: x equal to y", test_collection_travel_solution_1_limit_x_equal_to_y);
    CU_add_test(pSuite_Travel, "Test Collection Travel Solution: find sum of each combination", test_collection_travel_solution_sum_values);
    CU_add_test(pSuite_Travel, "Test Collection Travel Solution: find max of each combination", test_collection_travel_solution_max_value);
    CU_add_test(pSuite_Travel, "Test Collection Travel Solution: count combinations", test_collection_travel_solution_count_combinations);
    /* Set test mode */
    CU_basic_set_mode(CU_BRM_VERBOSE);

    /* Run all tests */
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
