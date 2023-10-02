C_COMPILER     = gcc
C_OPTIONS      = -Wall -pedantic -g
C_LINK_OPTIONS = -lm 
CUNIT_LINK     = -lcunit
C_PROF		   = -pg
C_GCOV	   	   = -fprofile-arcs -ftest-coverage

%.o:  %.c 
	$(C_COMPILER) $(C_OPTIONS) $^ -c 

ui.out: hash_table.o linked_list.o shop_cart.o merch_storage.o utils.o
	$(C_COMPILER) $(C_LINK_OPTIONS) $(C_OPTIONS) $^ -o $@ 

#ui.out: hash_table.c linked_list.c
#	$(C_COMPILER) $(C_OPTIONS) $^ -o $@ $(C_PROF)


merch_storage_tests.out: merch_storage_tests.o merch_storage.o 
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@ $(CUNIT_LINK) 

shop_cart_tests.out: shop_cart_tests.o shop_cart.o 
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@ $(CUNIT_LINK) 

ui_tests.out: ui_tests.o ui.o 
	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@ $(CUNIT_LINK)

tests: merch_storage_tests.out shop_cart_tests.out ui_tests.out
	./merch_storage_tests.out 
	./shop_cart_tests.out
	./ui_tests.out


#merch_test_coverage.out: merch_storage_tests.o merch_storage.c 
#	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@ $(CUNIT_LINK) $(C_GCOV)
#shop_test_coverage.out: shop_cart_tests.o shop_cart.c 
#	$(C_COMPILER) $(C_LINK_OPTIONS) $^ -o $@ $(CUNIT_LINK) $(C_GCOV)

#cov: merch_test_coverage.out shop_test_coverage.out
#	./merch_test_coverage.out
#	gcov -b -c merch_test_coverage.out-merch_storage.c
#	./shop_test_coverage.out
#	gcov -b -c shop_test_coverage.out-shop_cart.c


#prof: ui.out
#	./ui.out input.txt
#	gprof ui.out gmon.out > input.profiling
#

clean:
	rm -f *.o *.out *.profiling *.gcno *gcda *.gcov ui merch_storage_tests shop_cart_tests  


mem_tests: merch_storage_tests.out shop_cart_tests.out ui_tests.out
	valgrind --leak-check=full ./merch_storage_tests.out
	valgrind --leak-check=full ./shop_cart_tests.out
	valgrind --leak-ckeck=full ./ui_tests.out

mem_ui: ui.out
	valgrind --leak-check=full ./ui.out $(ARGS) 


.PHONY: ui test mem_tests mem_ui clean 