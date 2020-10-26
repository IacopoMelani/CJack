app   = app/**/*.c
libs  = libs/**/**/*.c
utils = utils/*.c

debug: main.c
	$(CC) -g main.c $(utils) $(app) $(libs) -o main -Wall -Wextra -pedantic -std=c99 -Wno-unknown-pragmas -D DEBUG

detect-leak: main.c
	$(CC) -g main.c $(utils) $(app) $(libs) -o main -Wall -Wextra -pedantic -std=c99 -Wno-unknown-pragmas -D DEBUG && \
	rm -f valgrind-out.txt 2> /dev/null && \
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=valgrind-out.txt \
			./main

release: main.c
	$(CC) main.c $(utils) $(app) $(libs) -o $(shell basename $(CURDIR)) -Wall -Wextra -pedantic -std=c99 -Wno-unknown-pragmas -D RELEASE