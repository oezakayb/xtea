CC:=gcc -g -o
CFLAGS=-O0

all: main
main: main.c xtea.S xtea_v1.c xtea_v3.c version_definer.c
	$(CC) $(CFLAGS) -o $@ $^
clean:
	rm -f main