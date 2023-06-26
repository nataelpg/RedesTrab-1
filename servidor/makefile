FLAGS = -Wall -std=c99
OBJS = message.o command.o
PROG = server

.PHONY: all debug clean purge

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(PROG): % : $(OBJS) %.o
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

all: client server $(PROG)  # Add $(PROG) to the 'all' target

client: client.o $(OBJS)
	gcc -o client client.o $(OBJS) $(FLAGS)

server: server.o $(OBJS)
	gcc -o server server.o $(OBJS) $(FLAGS)

command: command.o $(OBJS)
	gcc -o command command.o $(OBJS) $(FLAGS)

command.o: command.c
	gcc -c command.c $(FLAGS)

message.o: message.c
	gcc -c message.c $(FLAGS)

client.o: client.c
	gcc -c client.c $(FLAGS)

server.o: server.c
	gcc -c server.c $(FLAGS)

clean:
	@rm -f ~.bak .tmp

purge: clean
	@rm -f $(PROG) $(PROG_AUX).o
