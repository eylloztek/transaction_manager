CC = gcc
CFLAGS = -Wall

all: trmn adder subtractor multiplier divider

trmn: trmn.c
	$(CC) $(CFLAGS) trmn.c -o trmn

adder: adder.c
	$(CC) $(CFLAGS) adder.c -o adder

subtractor: subtractor.c
	$(CC) $(CFLAGS) subtractor.c -o subtractor

multiplier: multiplier.c
	$(CC) $(CFLAGS) multiplier.c -o multiplier

divider: divider.c
	$(CC) $(CFLAGS) divider.c -o divider

clean:
	rm -f trmn adder subtractor multiplier divider
	rm -f adder_pipe subtractor_pipe multiplier_pipe divider_pipe