#TODO: move .o files in a separate folder...
CC=gcc

CFLAGS= -Wall -Wextra -g -lreadline

shell.o: src/shell/shell.c src/shell/shell.h
	$(CC) -c src/shell/shell.c $(CFLAGS)

config.o: src/shell/config.c src/shell/config.h
	$(CC) -c src/shell/config.c $(CFLAGS)

command_parser.o: src/shell/command_parser.c src/shell/command_parser.h
	$(CC) -c src/shell/command_parser.c $(CFLAGS)

command_processor.o : src/command_processor/command_processor.c src/command_processor/command_processor.h
	$(CC) -c src/command_processor/command_processor.c $(CFLAGS)

log.o : src/log/log.c src/log/log.h
	$(CC) -c src/log/log.c $(CFLAGS)

arg.o : src/arguments_handling/arg.c src/arguments_handling/arg.h
	$(CC) -c src/arguments_handling/arg.c $(CFLAGS)

arg_handler.o : src/arguments_handling/arg_handler.c src/arguments_handling/arg_handler.h
	$(CC) -c src/arguments_handling/arg_handler.c $(CFLAGS)

main: arg.o arg_handler.o log.o command_processor.o config.o shell.o command_parser.o
	$(CC) arg.o arg_handler.o log.o command_processor.o config.o command_parser.o shell.o src/main.c $(CFLAGS) -o shell

build: main
	if [ ! -d "bin" ]; then mkdir bin; fi
	mv shell bin/
	cd src/log/logger_process/ && make build
clean:
	if [ -d "bin" ]; then rm -rf *.o; if [ -f "bin/shell" ]; then rm bin/shell; fi fi
	cd src/log/logger_process/ && make clean