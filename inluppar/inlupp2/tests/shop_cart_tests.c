#include <CUnit/Basic.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../logic/shop_cart.h"

int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}

bool string_eq(elem_t e1, elem_t e2)
{
    return (strcmp(e1.string, e2.string) == 0);
}

bool int_eq(elem_t e1, elem_t e2)
{
    return (e1.integer == e2.integer);
}

unsigned string_sum_hash(elem_t e)
{
    char *str = e.string;
    unsigned result = 0;
    do
    {
        result += *str;
    }
    while (*++str != '\0');
    return result; 
}

static unsigned hash_fun_key_int(elem_t key)
{
  return key.integer;
}

store_t *store_with_inputs()
{
    store_t *store = store_create(string_sum_hash, string_eq); 

    char *name = "Apple"; 
    char *description = "Red"; 
    int price = 10; 
    int stock_size = 0; 

    merch_t *apple = merch_create(strdup(name), strdup(description), price, ioopm_linked_list_create(string_eq), stock_size); 

    store_add(store, apple); 

    char *shelf[] = {"B36", "R62", "A4"}; 
    int quantity[] = {0, 1, 4}; 

    for (int i = 0; i < 3; i++)
    {
        location_add(apple, strdup(shelf[i]), quantity[i]);
    }

    return store; 
}

void create_destroy_test()
{
    carts_t *storage_carts = cart_storage_create(hash_fun_key_int, int_eq); 
    CU_ASSERT_PTR_NOT_NULL(storage_carts); 
    CU_ASSERT_TRUE(carts_is_empty(storage_carts)); 
    cart_storage_destroy(storage_carts); 
}

void add_to_cart_test()
{
    carts_t *storage_carts = cart_storage_create(hash_fun_key_int, int_eq); 
    store_t *store = store_with_inputs(); 
    cart_create(storage_carts, string_sum_hash, string_eq); 
    storage_carts->total_carts++; 

    int id = 0; 
    char *name = "Apple"; 
    char *merch_name = get_name(get_merch(store, name)); 
    int amount = 2; 

    CU_ASSERT_EQUAL(item_in_cart_amount(storage_carts, id, merch_name), 0); 

    cart_add(storage_carts, id, merch_name, amount); 
    CU_ASSERT_EQUAL(item_in_cart_amount(storage_carts, id, merch_name), 2); 

    cart_storage_destroy(storage_carts); 
    store_destroy(store); 
}

void remove_from_cart_test()
{
    carts_t *storage_carts = cart_storage_create(hash_fun_key_int, int_eq); 
    store_t *store = store_with_inputs(); 
    cart_create(storage_carts, string_sum_hash, string_eq); 
    storage_carts->total_carts++; 

    int id = 0; 
    char *name = "Apple"; 
    char *merch_name = get_name(get_merch(store, name)); 
    int amount = 2; 

    cart_add(storage_carts, id, merch_name, amount); 

    ioopm_hash_table_t *cart_items = get_items_in_cart(storage_carts, id); 

    cart_remove(cart_items, merch_name, 1); 
    CU_ASSERT_EQUAL(item_in_cart_amount(storage_carts, id, merch_name), 1); 

    cart_remove(cart_items, merch_name, 1); 
    CU_ASSERT_EQUAL(item_in_cart_amount(storage_carts, id, merch_name), 0); 

    cart_storage_destroy(storage_carts); 
    store_destroy(store);
}

void empty_cart_test()
{
    carts_t *storage_carts = cart_storage_create(hash_fun_key_int, int_eq); 
    CU_ASSERT_TRUE(carts_is_empty(storage_carts)); 

    cart_create(storage_carts, string_sum_hash, string_eq); 
    CU_ASSERT_FALSE(carts_is_empty(storage_carts)); 

    cart_storage_destroy(storage_carts); 
}

void has_merch_in_cart_test()
{
    carts_t *storage_carts = cart_storage_create(hash_fun_key_int, int_eq); 
    store_t *store = store_with_inputs(); 
    cart_create(storage_carts, string_sum_hash, string_eq); 
    storage_carts->total_carts++; 

    int id = 0; 
    char *name = "Apple"; 
    char *merch_name = get_name(get_merch(store, name)); 
    int amount = 2; 

    ioopm_hash_table_t *cart_items = get_items_in_cart(storage_carts, id);
    CU_ASSERT_FALSE(has_merch_in_cart(cart_items, merch_name)); 

    cart_add(storage_carts, id, merch_name, amount);    
    
    cart_items = get_items_in_cart(storage_carts, id);  
    CU_ASSERT_TRUE(has_merch_in_cart(cart_items, merch_name)); 

    cart_storage_destroy(storage_carts);
    store_destroy(store); 
}

void cost_calculate_test()
{
    carts_t *storage_carts = cart_storage_create(hash_fun_key_int, int_eq); 
    store_t *store = store_with_inputs(); 
    cart_create(storage_carts, string_sum_hash, string_eq); 
    storage_carts->total_carts++; 

    int id = 0; 
    char *name = "Apple"; 
    char *merch_name = get_name(get_merch(store, name)); 
    int amount = 2; 

    CU_ASSERT_EQUAL(cost_calculate(store, storage_carts, id), 0); 

    cart_add(storage_carts, id, merch_name, amount); 

    CU_ASSERT_EQUAL(cost_calculate(store, storage_carts, id), 20); 

    ioopm_hash_table_t *cart_items = get_items_in_cart(storage_carts, id);
    cart_remove(cart_items, merch_name, 1); 
    CU_ASSERT_EQUAL(cost_calculate(store, storage_carts, id), 10); 

    cart_storage_destroy(storage_carts); 
    store_destroy(store); 
}
 
//TODO: Needs better tests 
void checkout_cart_test()
{
    carts_t *storage_carts = cart_storage_create(hash_fun_key_int, int_eq); 
    store_t *store = store_with_inputs(); 
    cart_create(storage_carts, string_sum_hash, string_eq); 
    storage_carts->total_carts++; 

    int id = 0; 
    char *name = "Apple"; 
    char *merch_name = get_name(get_merch(store, name)); 
    int amount = 2; 

    cart_add(storage_carts, id, merch_name, amount); 

    ioopm_hash_table_t *cart_items = get_items_in_cart(storage_carts, id);  
    CU_ASSERT_TRUE(has_merch_in_cart(cart_items, merch_name)); 

    cost_calculate(store, storage_carts, 0); 
    cart_items = get_items_in_cart(storage_carts, id);  
    CU_ASSERT_FALSE(has_merch_in_cart(cart_items, merch_name)); 

    cart_storage_destroy(storage_carts); 
    store_destroy(store); 
}

int main()
{
    // First we try to set up CUnit, and exit if we fail
    if (CU_initialize_registry() != CUE_SUCCESS)
        return CU_get_error();

    // We then create an empty test suite and specify the name and
    // the init and cleanup functions
    CU_pSuite my_test_suite = CU_add_suite("Tests for shop_cart.c", init_suite, clean_suite);
    if (my_test_suite == NULL)
    {
        // If the test suite could not be added, tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // This is where we add the test functions to our test suite.
    // For each call to CU_add_test we specify the test suite, the
    // name or description of the test, and the function that runs
    // the test in question. If you want to add another test, just
    // copy a line below and change the information
    if (
        (CU_add_test(my_test_suite, "Create and destroy storage carts test", create_destroy_test) == NULL ||
         CU_add_test(my_test_suite, "Create and add to cart test", add_to_cart_test) == NULL ||
         CU_add_test(my_test_suite, "Remove from cart test", remove_from_cart_test) == NULL ||
         CU_add_test(my_test_suite, "Empty carts in store test", empty_cart_test) == NULL ||
         CU_add_test(my_test_suite, "Has merch in cart test", has_merch_in_cart_test) == NULL ||
         CU_add_test(my_test_suite, "Calculate total in cart", cost_calculate_test) == NULL ||
         CU_add_test(my_test_suite, "Checkout cart test", checkout_cart_test) == NULL
        )
    )
    {
        // If adding any of the tests fails, we tear down CUnit and exit
        CU_cleanup_registry();
        return CU_get_error();
    }

    // Set the running mode. Use CU_BRM_VERBOSE for maximum output.
    // Use CU_BRM_NORMAL to only print errors and a summary
    CU_basic_set_mode(CU_BRM_VERBOSE);

    // This is where the tests are actually run!
    CU_basic_run_tests();

    // Tear down CUnit before exiting
    CU_cleanup_registry();
    return CU_get_error();
}
