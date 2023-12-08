CC = gcc
CFLAGS = -Wall -I.

SRCS = main.c cpu.c ram.c storage.c gpu.c wifi.c led.c percent.c screen.c
OBJS = $(SRCS:.c=.o)
EXEC = my_program

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJS) $(EXEC)
