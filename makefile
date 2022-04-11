all: clean build run
clean:
	rm -f test out
build:
	gcc -g -Wall -Wextra main.c -o test
	touch out
run:
	./test
run_val:
	valgrind --leak-check=full ./test


