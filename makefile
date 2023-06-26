FLAGS = -Wall -std=c99 -lm -lcrypto -lssl
OBJS = message.o command.o 

# .PHONY: all debug clean purge

# %.o: %.c %.h
# 	$(CC) $(CFLAGS) -c $< -o $@

# $(PROG): % : $(OBJS) %.o
# 	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

all: cliente server

cliente: cliente.o $(OBJS)
	gcc -o cliente cliente.o $(OBJS) $(FLAGS)

server: server.o $(OBJS)
	gcc -o server server.o $(OBJS) $(FLAGS)

command.o: command.c
	gcc -c command.c $(FLAGS)

message.o: message.c
	gcc -c message.c $(FLAGS)

cliente.o: cliente.c
	gcc -c cliente.c $(FLAGS)

server.o: server.c
	gcc -c server.c $(FLAGS)

clean:
	@rm -f *~ *.bak *.tmp

purge: clean
	@rm -f  $(PROG) $(PROG_AUX) *.o $(OBJS) core a.out
	@rm -f *.png marker.out *.log


