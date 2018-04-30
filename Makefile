CC=gcc -std=c99
CFLAGS = -ggdb3 -W -Wall -Wextra -Werror -O0
LDFLAGS =
LIBS = 

default: main test benchmark

%.o: %.c %.h
	$(CC) -c -o $@ $< $(CFLAGS)

main: main.o bloom.o fileutil.o hashlib.o hasht.o lsm.o node.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

test_multithreaded_lsm: test.o test_multithreaded_lsm.o bloom.o fileutil.o hashlib.o hasht.o lsm.o node.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

test_singlethreaded_lsm: test.o test_singlethreaded_lsm.o bloom.o fileutil.o hashlib.o hasht.o lsm.o node.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)
	
test_multi_thread_range: test_multi_thread_range.o bloom.o fileutil.o hashlib.o hasht.o lsm.o node.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

test_single_thread_range: test_single_thread_range.o bloom.o fileutil.o hashlib.o hasht.o lsm.o node.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

benchmark: hash_table.o benchmark.o 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LIBS)

clean:
	rm -f main test benchmark *.exe *.o *.bin
