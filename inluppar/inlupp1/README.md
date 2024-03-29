 # Make commands
   #### Build and run word processing: 
   ```
   $ make clean
   $ make freq_count.out
   $ ./freq_count.out filename.txt
   ```
   #### Run tests:
   ```
   $ make clean
   $ make tests
   ```
   #### Memory tests:
   ```
   $ make clean

   for hash_test and list_test:
   $ make mem_tests
   
   for freq_count:
   $ make mem_freq_count ARGS="filename.txt"
   ```

   #### Coverage tests:
   ```
   $ make clean
   $ make cov
   ```
   _Coverage tests done with gcov_\
   `hash_table.c`: 
  - Lines executed: 100.00% of 138
  - Branches executed: 100.00% of 60
  - Taken at least once: 96.67% of 60
  - Calls executed: 100.00% of 20

   `linked_list.c`: 
   - Lines executed: 100.00% of 148
   - Branches executed: 100.00% of 72
   - Taken at least once: 86.11% of 72
   - Calls executed: 100.00% of 15
   
   #### Performace tests:
   ```
   $ make clean

   for all included .txt files: 
   $ make prof

   for a single prof: 
   $ make freq_count_prof.out
   $ ./freq_count_prof.out filename.txt
   $ gprof freq_count_prof.out gmon.out > output
   ```

   #### Time: 
   ```
   $ make clean
   $ make freq_count.out
   $ command time --verbose ./freq_count.out filename.txt
   ```

 # Notes and error handling

  - Errors and edge-cases are handled, mostly, by returning a void pointer to NULL. This void pointer is a part of an element with the type elem_t. The type is declared in common.h.  
  - The functions in hash_table assumes a suitable hash_function (hash_fun) and equality function (eq_fun) to fit the elem_t type ioopm_eq_function as seen in common.h. 
  - The functions in linked_list assumes a suitable equality function (eq_fun) to fit the elem_t type ioopm_eq_function as seen in common.h. 
  - It is assumed that the user ensures proper memory management when using the hash_table, linked_list and iterator, including freeing the allocated for keys, values, lists, iterators and options after use. 

 # Initial Profiling Results 
   ## small.txt
   **Most used functions:** \
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1. string_eq\
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. find_previous_entry_for_key\
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. get_bucket_index\
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. string_sum_hash

   **Time average (of 10):**
   - real    0m0.005s
   - user    0m0.003s
   - sys     0m0.002s

   Percent of CPU this job got: 0%

   ## 1k-long-words.txt
   **Most used functions:** \
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1. string_eq\
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. find_previous_entry_for_key\
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. get_bucket_index\
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. string_sum_hash
   
   **Time average:**
   - real    0m0.002s
   - user    0m0.002s
   - sys     0m0.000s
   
   Percent of CPU this job got: 100%

   ## 10k-words.txt
   **Most used functions:** \
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1. string_eq\
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. find_previous_entry_for_key\
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. get_bucket_index\
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. string_sum_hash
   
   **Time average:**
   - real    0m0.006s
   - user    0m0.005s
   - sys     0m0.000s
   
   Percent of CPU this job got: 88%

   ## 16k-words.txt
   **Most used functions:** \
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;1. string_eq\
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. find_previous_entry_for_key\
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. get_bucket_index\
   &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;2. string_sum_hash
   
   **Time average:**
   - real    0m0.035s
   - user    0m0.027s
   - sys     0m0.001s
   
   Percent of CPU this job got: 95%
      
   ##
   No library functions were among the most used and the most used was consistent across all inputs (a trend).


   It was consistent because for every lookup, has_key and insert we needed to run `find_previous_entry_for_key`. This function is also run in linear time. Meaning in the worst case we may need to run through the entire bucket to find the previous value in a case where all entries are in the same bucket. With a smaller hashtable (in our case with 17 buckets) the entries are not spread out enough to make an efficient hashtable. In the 16k-word file we have 4370 unique words and if all buckets were filled the same we would still have 257 entries in each and in the worst case we need the last element.


   `get_bucket_index` is used as many times as `find_previous_entry_for_key` because it is also used in every insert-, lookup- and has_key-functions. Though time spent in this function is dependent on how long the words are, and so far our long words are still not up to a measurable threshold.

   `string_eq` is used the most because in every `find_previous_entry_for_key` we will run the fuction each iteration meaning for every one time we run find_previous we run `string_eq` as many times as we have entried in that specific bucket. It is used similarly in some linked_list functions meaning we have doubble troubble. 

   While 1k-long-words did not use as much time as 10k-words and 16k-words it had worse performance (used most of the CPU to get the job done). It was unexpected but also logical. Longer words will spend more time in our hashing function since it adds the values of each character. Compare-functions will also take longer because there are more characters to compare with each other before we can determine that they are in fact identical or not.


   It was expected for the larger files to have worse performance and take more time. But 10k-words was surprisingly not as bad as we initially thought. It was closer to 1k-long-words in time than to 16k-words and took almost as much percent of the CPU as the 16k-words-file. Though the difference in unique words between 16k-words and 10k-words is 4171 and as explained with the 16k-words above, is a major player in the performance aspect.


   Based on the results we would need to find a better solution to find the previous key for an entry. Best case would be to find a constant time solution for it or implement another method for inserting, removing etc.
   A short-term solution would be to increase our buckets so these linear functions have better conditions for their search through the buckets. With more buckets we can spread the elements across and have fewer elements in each. This will take a larger amount of memory but time-wise the performance would improve.
   While string_eq is the most called function time-wise it does not affect the program significantly. Which is why it can be left, for now. 


The 16k-words was the only input which spent mesurable amount of time in each functions.\
**17 Buckets:**                               
Each sample counts as 0.01 seconds.
| % time |  cumulative seconds   |self seconds   | calls |  self ns/call | total ns/call | name  |    
|------|------------|----------|-------|----------|---------|--------------------------------|
|100.00  |    0.01 |    0.01 |  55346  |   0.00  |   0.00 | find_previous_entry_for_key       |       
|  0.00  |    0.01 |    0.00 |  4304460 |   0.00  |   0.00 | string_eq    |    
|  0.00  |    0.01 |    0.00 |   55346  |   0.00  |   0.00 | get_bucket_index               |       
|  0.00  |    0.01 |    0.00 |   55346  |   0.00  |   0.00 | string_sum_hash                |          
|  0.00  |    0.01 |    0.00 |   38354  |   0.00  |   0.00 | ioopm_hash_table_lookup        |             
|  0.00  |    0.01 |    0.00 |   16992  |   0.00  |   0.00 | ioopm_hash_table_has_key       |             
|  0.00  |    0.01 |    0.00 |   16992  |   0.00  |   0.00 | ioopm_hash_table_insert        |             
|  0.00  |    0.01 |    0.00 |   16992  |   0.00  |   0.00 | process_word                   |    
|  0.00  |    0.01 |    0.00 |    4370  |   0.00  |   0.00 | entry_create                   |    
|  0.00  |    0.01 |    0.00 |    4370  |   0.00  |   0.00 | free_keys                      |    
|  0.00  |    0.01 |    0.00 |    4370  |   0.00  |   0.00 | ioopm_iterator_has_next        |          
|  0.00  |    0.01 |    0.00 |    4370  |   0.00  |   0.00 | ioopm_iterator_next            |       
|  0.00  |    0.01 |    0.00 |    4370  |   0.00  |   0.00 | ioopm_linked_list_append       |                      
|  0.00  |    0.01 |    0.00 |    4370  |   0.00  |   0.00 | link_create                    |                
|  0.00  |    0.01 |    0.00 |      17  |   0.00  |   0.00 | entry_destroy                  |          
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_hash_table_apply_to_all  |             
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_hash_table_clear         |             
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_hash_table_create        |             
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_hash_table_destroy       |          
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_hash_table_keys          |          
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_hash_table_size          |                
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_iterator_current         |             
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_iterator_destroy         |             
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_linked_list_clear        |                
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_linked_list_create       |                
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_linked_list_destroy      |             
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_linked_list_is_empty     |                   
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_linked_list_size         |             
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | ioopm_list_iterator            |                
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   9.21 | process_file                   |          
|  0.00  |    0.01 |    0.00 |       1  |   0.00  |   0.00 | sort_keys                      |          


**100 Buckets:**  
Each sample counts as 0.01 seconds.
| % time |  cumulative seconds   |self seconds   | calls |  self ns/call | total ns/call | name  |    
|------|------------|----------|-------|----------|---------|--------------------------------|
|  0.00  |    0.00 |    0.00 |  768415  |   0.00  |   0.00 | string_eq       |       
|  0.00  |    0.00 |    0.00 |  55346 |   0.00  |   0.00 | find_previous_entry_for_key    |    
|  0.00  |    0.00 |    0.00 |   55346  |   0.00  |   0.00 | get_bucket_index               |       
|  0.00  |    0.00 |    0.00 |   55346  |   0.00  |   0.00 | string_sum_hash                |          
|  0.00  |    0.00 |    0.00 |   38354  |   0.00  |   0.00 | ioopm_hash_table_lookup        |             
|  0.00  |    0.00 |    0.00 |   16992  |   0.00  |   0.00 | ioopm_hash_table_has_key       |             
|  0.00  |    0.00 |    0.00 |   16992  |   0.00  |   0.00 | ioopm_hash_table_insert        |             
|  0.00  |    0.00 |    0.00 |   16992  |   0.00  |   0.00 | process_word                   |    
|  0.00  |    0.00 |    0.00 |    4370  |   0.00  |   0.00 | entry_create                   |    
|  0.00  |    0.00 |    0.00 |    4370  |   0.00  |   0.00 | free_keys                      |    
|  0.00  |    0.00 |    0.00 |    4370  |   0.00  |   0.00 | ioopm_iterator_has_next        |          
|  0.00  |    0.00 |    0.00 |    4370  |   0.00  |   0.00 | ioopm_iterator_next            |       
|  0.00  |    0.00 |    0.00 |    4370  |   0.00  |   0.00 | ioopm_linked_list_append       |                      
|  0.00  |    0.00 |    0.00 |    4370  |   0.00  |   0.00 | link_create                    |                
|  0.00  |    0.00 |    0.00 |      17  |   0.00  |   0.00 | entry_destroy                  |          
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_hash_table_apply_to_all  |             
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_hash_table_clear         |             
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_hash_table_create        |             
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_hash_table_destroy       |          
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_hash_table_keys          |          
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_hash_table_size          |                
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_iterator_current         |             
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_iterator_destroy         |             
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_linked_list_clear        |                
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_linked_list_create       |                
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_linked_list_destroy      |             
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_linked_list_is_empty     |                   
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_linked_list_size         |             
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | ioopm_list_iterator            |                
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | process_file                   |          
|  0.00  |    0.00 |    0.00 |       1  |   0.00  |   0.00 | sort_keys                      |

- real    0m0.034s
- user    0m0.008s
- sys     0m0.022s

As seen with 100 buckes the time improved a bit, because we don't have as many elements in each buckets to search through before we find what we want. The number of calls to string_eq has decresed significantly bacuse we don't have as many elements in eahc bucket to iterate over. Though the amount of calls to `find_previous_entry_for_key`, `get_bucket_index` and `string_sum_hash` will stay the same since during the calls they are not dependant on the length of each bucket, only how many lookups, has_key and inserts we need to do (amount of elements in the input file). 

