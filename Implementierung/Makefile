CC:=gcc -g -o
CFLAGS=-O0

all: xtea
xtea: main.c xtea.S xtea_v1.c xtea_v3.S xtea_v4.c version_definer.c
	$(CC) $(CFLAGS) -o $@ $^
clean:
	rm -f xtea