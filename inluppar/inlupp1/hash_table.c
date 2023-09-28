#include <stdlib.h>
#include "hash_table.h"
#include "linked_list.h"
#include "common.h"
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define Success(v) (option_t){.success = true, .value = v};
#define Failure() (option_t){.success = false};

/// the types from abovem  
typedef struct entry entry_t;
typedef struct hash_table ioopm_hash_table_t;
typedef struct option option_t;

struct entry
{
  elem_t key;       // holds the key
  elem_t value;   // holds the value
  entry_t *next; // points to the next entry (possibly NULL)
};

struct hash_table
{
  entry_t buckets[No_Buckets];
  ioopm_hash_function hash_fun;
  ioopm_eq_function eq_fun; 
};

static unsigned get_bucket_index(ioopm_hash_table_t *ht, ioopm_hash_function hash_fun, elem_t key)
{
  return ht->hash_fun(key) % No_Buckets; 
}

ioopm_hash_table_t *ioopm_hash_table_create(ioopm_hash_function hash_fun, ioopm_eq_function eq_fun)
{
  /// Allocate space for a ioopm_hash_table_t = No_Buckets (17) pointers to
  /// entry_t's, which will be set to NULL
  ioopm_hash_table_t *ht = calloc(1, sizeof(ioopm_hash_table_t));
  ht->hash_fun = hash_fun;
  ht->eq_fun = eq_fun; 
  return ht;
}

static void entry_destroy(entry_t *entry)
{
  while (entry != NULL)
  {
    entry_t *next = entry->next;
    free(entry);
    entry = next;
  }
}

void ioopm_hash_table_destroy(ioopm_hash_table_t *ht)
{
  ioopm_hash_table_clear(ht); 
  free(ht);
}

// Creates a new entry with a given key, value and next pointer
static entry_t *entry_create(elem_t key, elem_t value, entry_t *next)
{
  entry_t *new_entry = calloc(1, sizeof(entry_t));
  new_entry->key = key;
  new_entry->value = value;
  new_entry->next = next;

  return new_entry;
}

static entry_t *find_previous_entry_for_key(entry_t *bucket, elem_t key, ioopm_eq_function eq_fun)
{
  entry_t *prev = bucket;
  entry_t *current = bucket->next;

  while (current != NULL && !(eq_fun(current->key, key)))
  {
    prev = current;
    current = current->next;
  }

  return prev;
}

void ioopm_hash_table_insert(ioopm_hash_table_t *ht, elem_t key, elem_t value)
{
  /// Calculate the bucket for this entry
  unsigned bucket_index = get_bucket_index(ht, ht->hash_fun, key); 

  /// Search for an existing entry for a key
  entry_t *entry = find_previous_entry_for_key(&ht->buckets[bucket_index], key, ht->eq_fun);
  entry_t *next = entry->next;

  /// Check if the next entry should be updated or not

  if (next == NULL)
  {
    entry->next = entry_create(key, value, next);
  }
  else 
  {
    next->value = value;
  }

}

option_t *ioopm_hash_table_lookup(ioopm_hash_table_t *ht, elem_t key)
{
  unsigned bucket_index = get_bucket_index(ht, ht->hash_fun, key); 

  option_t *lookup_result = calloc(1, sizeof(option_t));
  entry_t *prev = find_previous_entry_for_key(&ht->buckets[bucket_index], key, ht->eq_fun);
  entry_t *current = prev->next;

  if (current != NULL)
  {
    *lookup_result = Success(current->value);
  }
  else
  {
    *lookup_result = Failure();
  }

  return lookup_result;
}

elem_t ioopm_hash_table_remove(ioopm_hash_table_t *ht, elem_t key)
{
  unsigned bucket_index = get_bucket_index(ht, ht->hash_fun, key); 

  option_t *lookup_result = ioopm_hash_table_lookup(ht, key);

  entry_t *prev = find_previous_entry_for_key(&ht->buckets[bucket_index], key, ht->eq_fun);
  entry_t *current = prev->next;
  elem_t removed_value; 

  if (lookup_result->success)
  {
    removed_value = current->value;

    if (current->next == NULL)
    {
      // for last entries
      prev->next = NULL;
      free(current);
    }
    else
    {
      // for first and middle entries
      prev->next = current->next;
      free(current);
    }
  }
  else
  {
    //error handeling
    removed_value.string = "key does not have an entry";
  }

  free(lookup_result);
  return removed_value;
}

size_t ioopm_hash_table_size(ioopm_hash_table_t *ht) 
{
  int counter = 0; 
  for (int i = 0; i < No_Buckets; i++) // CHEAT/TODO: hardcoded, implement something general //*ht != NULL
  {
    entry_t *cursor = &ht->buckets[i]; 
    while (cursor->next != NULL)
    {
      counter++; 
      cursor = cursor->next; 
    }
  }
  return counter;  
}

bool ioopm_hash_table_is_empty(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < No_Buckets; i++) 
  {
    entry_t *cursor = &ht->buckets[i]; 
    if (cursor->next != NULL)
    {
      return false; 
    }
  }
  
  return true;   
}

void ioopm_hash_table_clear(ioopm_hash_table_t *ht)
{
  for (int i = 0; i < No_Buckets; i++) 
  {
    entry_destroy((&ht->buckets[i])->next);
    ht->buckets[i].next = NULL; //reset all dangling pointers 
  }
}

static bool bool_eq_fun(elem_t a, elem_t b) 
{
    return b.integer == a.integer; 
}

ioopm_list_t *ioopm_hash_table_keys(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(bool_eq_fun);

  for (int i = 0; i < No_Buckets; i++) 
  {
    entry_t *current = (&ht->buckets[i])->next; 
    while (current != NULL)
    {
      ioopm_linked_list_append(list, current->key); 
      current = current->next; 
    }
  }
  return list;  
}

//functions the same as hash_table_keys, only difference is the name
ioopm_list_t *ioopm_hash_table_values(ioopm_hash_table_t *ht)
{
  ioopm_list_t *list = ioopm_linked_list_create(bool_eq_fun); 

  for (int i = 0; i < No_Buckets; i++) 
  {
    entry_t *current = (&ht->buckets[i])->next; 
    while (current != NULL)
    {
      ioopm_linked_list_append(list, current->value); 
      current = current->next; 
    }
  }
  return list;   
}

bool ioopm_hash_table_has_key(ioopm_hash_table_t *ht, elem_t key)
{
  option_t *lookup_result = ioopm_hash_table_lookup(ht, key);

  if (lookup_result->success)
  {
    free(lookup_result); 
    return true; 
  } 
  else
  {
    free(lookup_result); 
    return false; 
  }
}

bool ioopm_hash_table_has_value(ioopm_hash_table_t *ht, elem_t value)
{
  for (int i = 0; i < No_Buckets; i++) 
  {
    entry_t *current = (&ht->buckets[i])->next; 
    
    while (current != NULL)
    {
      char *duplicate = strdup(current->value.string); 

      //test using both the identical string and the equivalent string 
      if (!strcmp(current->value.string, value.string) && !strcmp(duplicate, value.string) && current->value.string == value.string)
      {
        free(duplicate); 
        return true; 
      }
      
      free(duplicate); 
      current = current->next; 
    }
  }
  return false; 
}

bool ioopm_hash_table_any(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg) 
{
  for (int i = 0; i < No_Buckets; i++) 
  {
    entry_t *current = (&ht->buckets[i])->next;

    while (current != NULL) 
    {
      if (pred(current->key, current->value, arg)) 
      {
        return true;
      }
      current = current->next;
    } 
  }
  return false;
}

bool ioopm_hash_table_all(ioopm_hash_table_t *ht, ioopm_predicate pred, void *arg) 
{
  for (int i = 0; i < No_Buckets; i++) 
  {
    entry_t *current = (&ht->buckets[i])->next;

    while (current != NULL) 
    {
      if (!pred(current->key, current->value, arg)) 
      {
        return false;
      }
      current = current->next;
    } 
  }
  return true;
}

void ioopm_hash_table_apply_to_all(ioopm_hash_table_t *ht, ioopm_apply_function apply_fun, void *arg)
{
  for (int i = 0; i < No_Buckets; i++) 
  {
    entry_t *current = (&ht->buckets[i])->next;
  
  while (current != NULL) 
  {
    apply_fun(current->key, &current->value, arg); // address of value to apply function
    current = current->next;
    }
  }
}


