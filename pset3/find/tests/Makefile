#
# Makefile
#
# Computer Science 50
# Problem Set 3
#

all: sort_test

sort_test: ../helpers.c ../helpers.h
	clang -ggdb3 -O0 -std=c11 -Wall -Werror -o sort_test sort_test.c ../helpers.c -lcs50 -lm

clean:
	rm -f *.o a.out sort_test