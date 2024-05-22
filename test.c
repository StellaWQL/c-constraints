#include "collection.h"

/*
 * size_of_list: the length of variable-list
 * list: item list like [Item("x", 1), Item("y", 2), ....]
 */
bool x_bigger_than_y(int size_of_list, struct item *list)
{
	// TODO: return x > y
}

bool y_bigger_than_z(int size_of_list, struct item *list)
{
	// TODO: return y > z
}

void print_result(int size_of_list, struct item *list)
{
	// TODO: print info
}

int main(void)
{
	/* Demo of APIs' usage */
	struct collection *collection = collection_new();

	/* Add variables to collection */
	collection_add_variable(collection, "x", 1, 2, 3, 5);
	collection_add_variable(collection, "y", 3, 5);
	collection_add_variable(collection, "z", 1, 2, 3, 5);

	/* Add limits to collection */
	collection_add_limit(collection, (limit_fp_t)x_bigger_than_y);
	collection_add_limit(collection, (limit_fp_t)y_bigger_than_z);

	/* Travel all solutions */
	collection_travel_solution(collection, (travel_fp_t)print_result);
	/* Expect:
	 *	x = 5, y = 3, z = 1
	 *	x = 5, y = 3, z = 2
	 *	x = 5, y = 3, z = 3
	 *	...
	 * */
};
