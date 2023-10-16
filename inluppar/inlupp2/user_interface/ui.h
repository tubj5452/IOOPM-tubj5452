#include "../logic/merch_storage.h"
#include "../logic/shop_cart.h"
#include "../data_structures/common.h"

#define PRINT_AT_A_TIME 20

/**
 * @file ui.h
 * @author Tuva Björnberg & Marcus Ray Sandersson
 * @date 2/10-2023
 * @brief  
 *
 */

typedef bool(*ioopm_exist_function)(store_t *a, char *b);
