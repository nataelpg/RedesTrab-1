FLAGS = -Wall -std=c99
OBJS = message.o 
PROG = cliente

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

message.o: message.c
	gcc -c message.c $(FLAGS)

client.o: client.c
	gcc -c client.c $(FLAGS)

server.o: server.c
	gcc -c server.c $(FLAGS)

your_program_name.o: your_program_name.c  # Add rule for main program file
	gcc -c your_program_name.c $(FLAGS)

clean:
	@rm -f ~.bak .tmp

purge: clean
	@rm -f $(PROG) $(PROG_AUX).o $(OBJS) core a.out
	@rm -f .png marker.out.log
