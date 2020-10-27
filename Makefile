app   = app/**/*.c
libs  = libs/**/**/*.c
utils = utils/*.c
cli   = cli/**.c
gtk   = gtk/**.c

outfile-cli-debug=main-cli
outfile-gtk-debug=main-gtk
outfile-cli-release=cjack-cli

debug-cli:
	$(CC) -g $(cli) $(utils) $(app) $(libs) -o $(outfile-cli-debug) -Wall -Wextra -pedantic -std=c99 -Wno-unknown-pragmas -D DEBUG -D CLI

detect-leak-cli:
	$(CC) -g $(cli) $(utils) $(app) $(libs) -o $(outfile-cli-debug) -Wall -Wextra -pedantic -std=c99 -Wno-unknown-pragmas -D DEBUG -D CLI && \
	rm -f valgrind-out.txt 2> /dev/null && \
	valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--verbose \
			--log-file=valgrind-out.txt \
			./$(outfile-cli-debug)

release-cli:
	$(CC) $(cli) $(utils) $(app) $(libs) -o $(outfile-cli-release) -Wall -Wextra -pedantic -std=c99 -Wno-unknown-pragmas -D RELEASE -D CLI


debug-gtk:
	$(CC) -g $(gtk) $(utils) $(app) $(libs) `pkg-config --cflags gtk+-3.0` -o $(outfile-gtk-debug) `pkg-config --libs gtk+-3.0`
