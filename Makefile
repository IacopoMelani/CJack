debug: main.c
	$(CC) main.c app/**/*.c libs/**/**/*.c -o main -Wall -Wextra -pedantic -std=c99 -Wno-unknown-pragmas -D DEBUG
release: main.c
	$(CC) main.c app/**/*.c libs/**/**/*.c -o $(shell basename $(CURDIR)) -Wall -Wextra -pedantic -std=c99 -Wno-unknown-pragmas -D RELEASE