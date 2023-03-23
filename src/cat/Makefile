CC=gcc
CFLAGS=-std=c11 -Wall -Werror -Wextra -g
SOURCES=s21_cat.c
OBJECTS=$(SOURCES:.c=.o)
OS = $(shell uname)

ifeq ($(OS), Darwin)
	CHECK_H=-lcheck
else
	CHECK_H=-lcheck -pthread -lrt -lm -D_GNU_SOURCE
endif

all: s21_cat test clean

s21_cat:
	$(CC) $(CFLAGS) $(SOURCES) -o s21_cat
	mkdir -p ../build/cat
	mv s21_cat ../build/cat

test: s21_cat
	cp ../../materials/tests/bytes.txt bytes.txt
	cp ../../materials/tests/t_cat.sh t_cat.sh
	cp ../build/cat/s21_cat .
	bash t_cat.sh

style:
	cp ../../materials/linters/.clang-format ./
	clang-format -n *.c -Werror
	rm .clang-format

clean:
	rm -rf *.o *.a *.so *.gcda *.gcno *.gch rep.info *.html *.css test report *.txt *.sh *.out s21_cat
	rm -rf ../build/cat

rebuild: all