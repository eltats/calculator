CC= gcc -std=c11 #-Wall -Wextra -Werror# -g
LIBSOURCES= helpers.c bank.c algo.c
LIBOBJECTS= $(LIBSOURCES:.c=.o)
COVERAGE= --coverage
LIB=s21_calc.a

test: $(LIB)
	$(CC) --coverage $(LIBSOURCES) tests/*.c -L. $(LIB) -lcheck `pkg-config --cflags --libs check`
	leaks -q -atExit -- ./a.out

$(LIB): $(LIBOBJECTS)
	ar rc $(LIB) $^
	ranlib $(LIB)

clean:
	rm -rf $(LIB)
	rm -rf *.o
	rm -rf a.out
	rm -rf algo
	find . -name '*.gcno' -type f -delete
	find . -name '*.gcda' -type f -delete
	find . -name '*.info' -type f -delete
	rm -rf report
	rm -rf *.dSYM

gcov_report: $(LIB) test
	lcov -t "test" -o test.info -c -d . 
	genhtml -o report test.info
	open report/index.html

