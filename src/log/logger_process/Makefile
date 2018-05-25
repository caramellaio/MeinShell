CC=gcc

#CFLAGS= -Wall -Wextra -g
CFLAGS= -w

arg.o: ../../arguments_handling/arg.c ../../arguments_handling/arg.h
	$(CC) -c ../../arguments_handling/arg.c $(CFLAGS)

arg_handler.o : ../../arguments_handling/arg_handler.c ../../arguments_handling/arg_handler.h
	$(CC) -c ../../arguments_handling/arg_handler.c $(CFLAGS)

logger_process.o: logger_process.c logger_process.h
	$(CC) -c logger_process.c $(CFLAGS)

main: arg.o arg_handler.o logger_process.o 
	$(CC) arg.o arg_handler.o logger_process.o logger_process_main.c $(CFLAGS) -o Logger

build: main
	mv Logger ../../../bin/
clean:
	if [ -d "../../../bin" ]; then rm -rf *.o; if [ -f "../../../bin/Logger" ]; then rm ../../../bin/Logger; fi fi