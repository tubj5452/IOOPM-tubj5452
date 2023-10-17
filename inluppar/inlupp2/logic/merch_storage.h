#pragma once

#include "../data_structures/hash_table.h"
#include "../data_structures/linked_list.h"
#include "../data_structures/iterator.h"

/**
 * @file merch_storage.h
 * @author Tuva Björnberg & Marcus Ray Sandersson
 * @date 2/10-2023
 * @brief  
 *
 */

#define INITIAL_CAPACAITY 10

typedef struct {
    char *name;
    char *description;
    int price;
    ioopm_list_t *stock;
    int stock_size; 
} merch_t;

typedef struct {
  char *shelf;
  int quantity;
} location_t;

typedef struct {
  char **merch_names;
  ioopm_hash_table_t *merch_details;
  int merch_count;
  int capacity;
} store_t;

typedef void(*ioopm_cart_apply_function)(elem_t key, elem_t *value, void *arg1, void *arg2);

typedef struct hash_table ioopm_hash_table_t;
typedef struct entry entry_t;

struct entry
{
  elem_t key;      
  elem_t value;   
  entry_t *next; 
};

struct hash_table
{
  entry_t buckets[No_Buckets];
  ioopm_hash_function hash_fun;
  ioopm_eq_function eq_fun; 
};


/// @brief 
/// @param
/// @return
store_t *store_create(ioopm_hash_function hash_fun, ioopm_eq_function eq_fun);

/// @rief creates a new merch
/// @param
/// @return
//ioopm_eq_function eq_fun, 
merch_t *merch_create(char *name, char *description, int price, ioopm_list_t *stock, int stock_size);

/// @brief 
/// @param
/// @return
void store_add(store_t *store, merch_t *merch); 

/// @brief 
/// @param merch merch to add location to, expects merch following its struct
/// @return
void location_add(merch_t *merch, char *shelf, int amount); 

/// @brief 
/// @param
/// @return
bool merch_exists(store_t *store, char *name); 

/// @brief 
/// @param
/// @return
bool store_is_empty(store_t *store); 

/// @brief 
/// @param
/// @return
size_t store_size(store_t *store); 

/// @brief 
/// @param
/// @return
size_t shelves_size(merch_t *merch); //TODO: CURRENTLY ONLY USED BY TESTS

/// @brief 
/// @param
/// @return
merch_t *get_merch(store_t *store, char *name); 

/// @brief 
/// @param merch to get name from, expects merch following its struct
/// @return
char *get_name(merch_t *merch); //TODO: CURRENTLY ONLY USED BY TESTS

/// @brief 
/// @param merch to get despription from, expects merch following its struct
/// @return
char *get_description(merch_t *merch); //TODO: CURRENTLY ONLY USED BY TESTS

/// @brief 
/// @param merch to get price from, expects merch following its struct
/// @return
int get_price(merch_t *merch); 

/// @brief 
/// @param
/// @return
char *get_shelf(location_t *location); //TODO: CURRENTLY ONLY USED BY TESTS

/// @brief 
/// @param
/// @return
int get_quantity(location_t *location); //TODO: CURRENTLY ONLY USED BY TESTS

/// @brief 
/// @param
/// @return
location_t *get_location(merch_t *merch, char *shelf); //TODO: CURRENTLY ONLY USED BY TESTS

/// @brief may need to insert again to get a valid hashing TODO: edit to better breif
/// @param
/// @return
void set_name(store_t *store, merch_t *old_merch, char *new_name, ioopm_hash_table_t *carts); 

/// @brief 
/// @param
/// @return
void set_description(merch_t *merch, char *new_description); 

/// @brief 
/// @param
/// @return
void set_price(merch_t *merch, int new_price); 

/// @brief 
/// @param merch to print, expects merch following its struct
/// @return
void print_merch(merch_t *merch); 

/// @brief 
/// @param merch print its stock, expects merch following its struct
/// @return
void print_stock(merch_t *merch); 

/// @brief 
/// @param
/// @return
void store_remove(store_t *store, ioopm_hash_table_t *carts, char *name);

/// @brief 
/// @param
/// @return
void store_destroy(store_t *store); 
