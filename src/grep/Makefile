CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -g
SOURCES=s21_grep.c
OBJECTS=$(SOURCES:.c=.o)
OS = $(shell uname)

ifeq ($(OS), Darwin)
	CHECK_H=-lcheck
else
	CHECK_H=-lcheck -pthread -lrt -lm -D_GNU_SOURCE
endif

all: s21_grep test

s21_grep:
	$(CC) $(CFLAGS) $(SOURCES) -o s21_grep
	mkdir -p ../build/grep
	mv s21_grep ../build/grep

test: s21_grep
ifeq ($(OS), Darwin)
	cp ../../materials/tests/pattern.txt pattern.txt
	cp ../../materials/tests/test.txt test.txt
	cp ../../materials/tests/t_grep_mac.sh t_grep_mac.sh
	cp ../build/cat/s21_cat .
	bash t_grep_mac.sh
else
	cp ../../materials/tests/pattern.txt pattern.txt
	cp ../../materials/tests/test.txt test.txt
	cp ../../materials/tests/t_grep.sh t_grep.sh
	cp ../build/grep/s21_grep .
	bash t_grep.sh
endif

style:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c -Werror
	rm .clang-format

clean:
	rm -rf *.o *.a *.so *.gcda *.gcno *.gch rep.info *.html *.css test report *.txt *.sh *.out s21_grep
	rm -rf ../build

rebuild: all