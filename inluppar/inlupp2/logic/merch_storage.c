#include "../data_structures/hash_table.h"
#include "../data_structures/linked_list.h"
#include "merch_storage.h"

merch_table_t *store_create(ioopm_hash_function hash_fun, ioopm_eq_function eq_fun)
{
    ioopm_hash_table_create(hash_fun, eq_fun); 
}

merch_t *merch_create(char *name, char *description, int price, char *shelf)
{
    merch_t *new_merch = calloc(1, sizeof(merch_t));
    new_merch->name = name;
    new_merch->description = description;
    new_merch->price = price;

    //ioopm_linked_list_create()
    //new_merch->location = 
    new_merch->stock = 0; 

    return new_merch;
}

void store_add(merch_table_t *store, merch_t *merch)
{
    return; 
}

void stock_add(merch_t *merch, int to_add)
{
    return; 
}

void location_add(merch_t *merch, char *shelf)
{
    return; 
}


void store_remove(merch_table_t *store, merch_t *merch)
{
    return; 
} 


bool merch_exists(merch_table_t *store, char *name)
{
    return false; 
}

bool store_is_empty(merch_table_t *store)
{
    return false; 
}


size_t store_size(merch_table_t *store)
{
    return 0; 
}


merch_t *get_merch(merch_table_t *store, char *name)
{
    return; 
}

void get_names_in_arr(merch_table_t *store, char *arr_of_names[])
{
    return; 
}

int get_stock(char *name)
{
    return 0; 
}


void set_name(merch_t *merch, char *new_name)
{
    return; 
}

void set_description(merch_t *merch, char *new_description)
{
    return; 
}

void set_price(merch_t *merch, int new_price)
{
    return; 
}


void print_merch(merch_t *merch)
{
    return; 
}

void print_stock(merch_t *merch)
{
    return; 
}



void merch_destroy(merch_t *merch)
{
    free(merch); 
}

void store_destroy(merch_table_t *store)
{
    ioopm_hash_table_destroy(store); 
}