#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/utils.h"
#include "../data_structures/linked_list.h"
#include "../data_structures/hash_table.h"
#include "ui.h"
#include <string.h>
#include <ctype.h>
#include <time.h>

static int cmp_stringp(const void *p1, const void *p2)
{
    return strcmp(*(char *const *)p1, *(char *const *)p2);
}

void sort_keys(char *keys[], size_t no_keys)
{
    qsort(keys, no_keys, sizeof(char *), cmp_stringp);
}

bool string_eq(elem_t e1, elem_t e2)
{
    return (strcmp(e1.string, e2.string) == 0);
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

static char *input_name_check(ioopm_exist_function exist_fun, store_t *store)
{
    char *input_name = ask_question_string("\nWrite the name of the merch: "); 

    while (!exist_fun(store, input_name))
    {
        char *new_alt = ask_question_string("\nThe merch doesn't exist, do you want to write another one (y/n)? "); 
        if (toupper(*new_alt) == 'Y')
        {
            input_name = ask_question_string("\nWrite the name of the merch: "); 
        }
        else
        {
            return NULL;    
        }
    }
    return input_name; 
}

static merch_t *input_merch(void)
{
    char *name = ask_question_string("\nWrite the name of the merch: "); 
    char *description = ask_question_string("\nWrite a description of the merch: "); 
    int price = ask_question_int("\nWrite the price of the merch: ");
    char *shelf = ask_question_shelf("\nWrite the shelf: (Format: 'A36') "); 

    return merch_create(name, description, price, shelf); 
}

void add_merch(store_t *store)
{    
    //what happens if identical locations?? only checking for merch exists
    merch_t *input = input_merch(); 

    while (merch_exists(store, input->name))
    {
        char *new_alt = ask_question_string("\nThe merch already exists, do you want to add another one (y/n)? "); 
        if (toupper(*new_alt) == 'Y')
        {
            input = input_merch();  
        }
        else 
        {
            return; 
        }
    }

    store_add(store, input); 

}

void list_merch(store_t *store)
{
    if (store_is_empty(store)) 
    {
        printf("\nThe store is empty, no merch to list"); 
        return; 
    } 

    size_t st_size = store_size(store);
    int fist_print_size = st_size < 20 ? st_size : 20; 

    char *names[st_size];           //TODO: needs to be allocated?
    get_names_in_arr(store, names); //
    sort_keys(names, st_size); 

    for (int i; i < fist_print_size; i++)
    {   
        merch_t *merch = get_merch(store, names[i]); 
        print_merch(merch); 
    }

    char *continue_list = ask_question_string("\nPress 'N' to return to menu, press anywhere to continue list ");
    if (toupper(*continue_list) == 'N' ) 
    {   
        return; 
    }
    else 
    {
        for (int i = fist_print_size; i < st_size; i++)
        {
            merch_t *merch = get_merch(store, names[i]); 
            print_merch(merch); 
        }
    }

}

void remove_merch(store_t *store)
{
    if (store_is_empty(store)) 
    {
        puts("\nThe store is empty, please add items first"); 
        return; 
    }

    char *name = input_name_check(merch_exists, store);
    if (name == NULL) return; 

    char *conf_remove = ask_question_string("\nAre you sure you want to remove this merch? \n"); 

    if (toupper(*conf_remove) == 'Y')
    {
        store_remove(store, name); 
    }
    return; 
}

void edit_merch(store_t *store)
{
    if (store_is_empty(store)) 
    {
        puts("\nThe store is empty, please add items first"); 
        return; 
    }

    char *name = input_name_check(merch_exists, store);
    if (name == NULL) return; 

    char *conf_edit = ask_question_string("\nAre you sure you want to edit this merch? "); 

    if (!(toupper(*conf_edit) == 'Y'))
    {
        return; 
    }
    
    merch_t *merch = get_merch(store, name); 
    char *alt_edit = ask_question_string("\nChoose what you want to edit:\n[A] Edit name\n[B] Edit description\n[C] Edit price\n"); 

    char *new_name; 
    char *new_description; 
    int new_price; 

    switch (toupper(*alt_edit)) 
    {
        case 'A':
            new_name = ask_question_string("\nWrite the new name: "); 
            set_name(merch, new_name); 
            break; 
        case 'B': 
            new_description = ask_question_string("\nWrite the new decription: "); 
            set_description(merch, new_description); 
            break; 
        case 'C': 
            new_price = ask_question_int("\nWrite the new price: "); 
            set_price(merch, new_price); 
            break; 
        default:
            alt_edit = ask_question_string("Try again with a valid input \n");
    } 

}

void show_stock(store_t *store)
{
    if (store_is_empty(store)) 
    {
        puts("\nThe store is empty, please add items first"); 
        return; 
    }

    char *name = input_name_check(merch_exists, store);
    if (name == NULL) return; 

    merch_t *merch = get_merch(store, name); 

    print_stock(merch); 

}

void replenish_stock(store_t *store)
{
    if (store_is_empty(store)) 
    {
        puts("\nThe store is empty, please add items first"); 
        return; 
    }

    char *name = input_name_check(merch_exists, store);
    if (name == NULL) return; 

    merch_t *merch = get_merch(store, name); 

    printf("\nYou selected this merch:\n"); 
    print_merch(merch); 

    int input_replenish = ask_question_int("\nEnter the amount to increase the stock: "); 
    while (input_replenish < 1)
    {
        input_replenish = ask_question_int("\nEnter at least 1: "); 
    }

    for (int i = 0; i < input_replenish; i++)
    {
        char *input_shelf = ask_question_shelf("\nEnter the shelf to add stock to: "); 
        location_add(merch, input_shelf); 
        stock_add(merch, 1); 
    }

}

void create_cart(store_t *store)
{



}   

void remove_cart(store_t *store)
{

}

void add_to_cart(store_t *store)
{

}

void remove_from_cart(store_t *store)
{

}

void calculate_cart_cost(store_t *store)
{

}

void checkout_cart(store_t *store)
{

}


void print_menu(void) 
{
    printf("\n[A] Add merchendise\n[L] List merchendise\n[D] Remove merchendise\n[E] Edit merchendise\n"
            "[S] Show stock\n[P] Replenish stock\n[C] Create cart\n[R] Remove cart\n[+] Add to cart\n"
            "[-] Remove item from cart\n[=] Calculate cost of all items in your cart\n[O] Checkout\n[Q] Quit\n"); 

}

char ask_question_menu(void) 
{
    print_menu(); 
    char *user_input = ask_question_string("");
    int valid_user_input = 0; 

    do 
    {
        switch (toupper(*user_input)) 
        {
            case 'A':
            case 'L': 
            case 'D': 
            case 'E': 
            case 'S': 
            case 'P': 
            case 'C': 
            case 'R': 
            case '+': //should work with toupper?
            case '-': //-||-
            case '=': //-||-
            case 'O': 
            case 'Q': 
                valid_user_input = 1; 
                break;
            default:
                user_input = ask_question_string("Try again with a valid input\n");
            } 
    }
    while(!valid_user_input); 

    return toupper(*user_input);
}


void event_loop(store_t *store, int store_size) 
{
    bool running = true; 
    char *quit_confirmation; 

    do 
    {
        switch (ask_question_menu()) 
        {
            case 'A': 
                add_merch(store); 
                break; 
            case 'L': 
                list_merch(store); 
                 break; 
            case 'D':
                remove_merch(store); 
                break;
            case 'E':
                edit_merch(store); 
                break;
            case 'S': 
                show_stock(store); 
                break;
            case 'P': 
                replenish_stock(store); 
                break;
            case 'C': 
                create_cart(store); 
                break; 
            case 'R':
                remove_cart(store); 
                break; 
            case '+':
                add_to_cart(store); 
                break; 
            case '-': 
                remove_from_cart(store); 
                break; 
            case '=': 
                calculate_cart_cost(store); 
                break; 
            case 'O': 
                checkout_cart(store); 
                break; 
            case 'Q':
                quit_confirmation = ask_question_string("Press 'Y' if you really want to quit");

                if (toupper(*quit_confirmation) == 'Y')
                {
                    running = false; 
                    store_destroy(store); 
                } 
                break; 
            default:
                break;
        }
        //error handling??
        //if (store_size == 16) {
        //    printf("Du har lagt till max antal varor (16 st), fortsätter du att lägga till varor skrivs det över dina tidigare varor\n"); 
        //    store_size = 0; 
        //}
    } while (running); 
}

int main() { 
    store_t *store = store_create(string_sum_hash, string_eq); 
    int store_siz = store_size(store); // Antalet varor i arrayen just nu
    event_loop(store, store_siz); 
    return 0;
}