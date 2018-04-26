CC=gcc -std=c99
CFLAGS = -ggdb3 -W -Wall -Wextra -Werror -O0
LDFLAGS =
LIBS = 

default: main test benchmark

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o bloom.o fileutil.o hashlib.o hasht.o lsm.o node.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

test: test.o bloom.o fileutil.o hashlib.o hasht.o lsm.o node.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

test_range: test_range.o bloom.o fileutil.o hashlib.o hasht.o lsm.o node.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

benchmark: hash_table.o benchmark.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f main test benchmark *.exe *.o *.bin
