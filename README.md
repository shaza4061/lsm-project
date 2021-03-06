# Write optimize key-value store using LSM-tree
LSM-tree implementation using hash table

# Building
    make clean; make lsm-tree;

# Running tests
1. Running test for single-thread lsm-tree  
> make clean; make test_singlethreaded_lsm;./test_singlethreaded_lsm;

2. Running test for multi-thread lsm-tree  
> make clean; make test_multithreaded_lsm; ./test_multithreaded_lsm;

3. Running test for multi-thread range operation
> make clean; make test_multi_thread_range; ./test_multi_thread_range;

# Running the program
    ./lsm-tree  
or  
You can now run the following to see all available options:  

    ./lsm-tree -h

## Usage  
You can start the lsm-tree with following options:  

    -b <size of Level 0 hash table>  
    -l <LSM-tree maximum height>  
    -r <level ratio>    
    -d <{r,s,+}=<data file location> (r = level range, e.g r1-2. s = single level, e.g. s1.+ = starting from this level, e.g. +1)  
    -f <bloom filter false positive rate. must be between 0-1>  
    -s (silent mode)  
    -t (number of thread for range operation)  
    -h (this help)  

### Example

1. Start lsm-tree with 127 items as the initial buffer size and maximum level 10
> ./lsm-tree -b 127 -l 10

2. Start lsm-tree with level ratio 4
> ./lsm-tree -r 2

3. Start lsm-tree and specify the location of the data file for all level to /var/data
> ./lsm-tree -d +1=/var/data/

4. Start lsm-tree and specify the location of the data file for level 1 to /mnt/ramdisk and all other level starting from level 2 to /var/data
> ./lsm-tree -d s1=/mnt/ramdisk,+2=/var/data/

5. Start lsm-tree and specify the location of the data file for level 1 and 2 to /mnt/ramdisk and all other level starting from level 3 to /var/data
> ./lsm-tree -d r1-2=/mnt/ramdisk,+3=/var/data/

6. Start lsm-tree with bloom filter false-positive-ratio set to 4%
> ./lsm-tree -f 0.04

7. Start lsm-tree with 4 threads (multi-threading is only available for *get* and *range* queries)
> ./lsm-tree -t 4

8. Start the lsm-tree in silent mode (useful for testing)
> ./lsm-tree -s

# Running workload demo

1. The lsm-tree supports redirection. To run the sample workload, you can run below command to load the small workload file:  

> make clean
> make lsm-tree  
> ./lsm-tree < demo/small_workload_demo.txt 

or below command to load the big workload file (100k puts and 50k gets)  

> make clean
> make lsm-tree  
> ./lsm-tree < demo/1_2_read_write_ratio.txt

2. The lsm tree also can load data from binary file (below example will load 1k keys into the tree and print the tree stats)  

> make clean
> make lsm-tree  
> ./lsm-tree < demo/load_binary_file.txt

3. Below example run the demo for range operation.

> make clean
> make lsm-tree  
> ./lsm-tree < demo/range_workload.txt


# Supported command
The upport six commands: put, get, range, delete, load, and print stats. Each command is explained in greater detail below.

## Put
The put command will insert a key-value pair into the LSM tree. Duplicates are not supported in the expected implementation so the repeated put of a key updates the value stored in the tree.

#### Syntax

> p [INT1] [INT2]

The 'p' indicates that this is a put command with key INT1 and value INT2.

#### Example:

    p 10 7
    p 63 222
    p 10 5

First the key 10 is added with value 7. Next, key 63 is added with value 222. The tree now holds two pairs. Finally, the key 10 is updated to have value 5. Note that the tree logically still holds only two pairs. These instructions include only puts therefore no output is expected.

## Get
The get command takes a single integer, the key, and returns the current value associated with that key.

#### Syntax

> g [INT1]

The 'g' indicates that this is a get for key INT1. The current value should be printed on a single line if the key exists and a blank line printed if the key is not in the tree.

#### Example:

    p 10 7
    p 63 222
    g 10
    g 15
    p 15 5
    g 15

output:

    7
    
    5

Here we first put (key:value) 10:7, then 63:222. The next instruction is a get for key 10, so the system outputs 7, the current value of key 10. Next we try to get key 15 but it is not included in the tree yet so a blank line is generated in the output. Next we put 15:5. Finally, we get key 15 again. This time it outputs 5 as the key exists at this point in the instruction list.

## Range
The range command works in a similar way to get but looks for a sequence of keys rather than a single point.

#### Syntax

> r [INT1] [INT2]

The 'r' indicates that this is a range request for all the keys from INT1 inclusive to INT2 exclusive. If the range is completely empty than the output should be a blank line, otherwise the output should be a space delineated list of all the found pairs (in the format key:value); order is irrelevant.

#### Example:

    p 10 7
    p 13 2
    p 17 99
    p 12 22
    r 10 12
    r 10 15
    r 14 17
    r 0 100

output:

    10:7
    10:7 12:22 13:2
    
    10:7 12:22 13:2 17:99

## Delete
The delete command will remove a single key-value pair from the LSM tree.

#### Syntax

    d [INT1]

The 'd' indicates that this is a delete command for key INT1 and its associated value (whatever that may be). A delete of a non-existing key has no effect.

#### Example:

    p 10 7
    p 12 5
    g 10
    d 10
    g 10
    g 12

output:

    7
    
    5

First the two pairs 10:7 and 12:5 are put into the tree. Next, we get key 10 which outputs the value 7. Key 10 is then deleted which generates no output. When we then try to get key 10 a second time we get a blank as the key has been deleted. Finally we get key 12 which is unaffected by the delete and therefore outputs the value 5.

## Load
The load command is included as a way to insert many values into the tree without the need to read and parse individual ascii lines.

#### Syntax

> l </path/to/file_name>

The 'l' command code is used for load. This command takes a single string argument. The string is a relative or absolute path to a binary file of integers that should be loaded into the tree. Note that there is no guarantee that the new values being inserted are absent from the current tree or unrepeated in the file, only that all of the pairs are puts in the order of the file.

#### Example:

> l ~/load_file.bin

This command will load the key value pairs contained in load_file.bin in the home directory of the user running the workload. The layout of the binary file is

> KEYvalueKEYvalueKEYvalue...

There is nothing but key-value pairs in the file. The first 4 bytes are the first key, the next 4 bytes are the first value and so on. Thus, a file of 32768 bytes has 4096 key value pairs to be loaded (though there could be fewer than 4096 pairs once loaded due to existing keys in the tree or duplicated keys in the file).

## Print Stats
The print stats command is a command that allows the caller to view some information about the current state of the tree. This is helpful for debugging as well as the final evaluation. Each implementation may vary somewhat but at a minimum the function must include: (1) the number of logical key value pairs in the tree Total Pairs: [some count]; (2) the number of keys in each level of the tree LVL1: [Some count], ... , LVLN [some count]; (3) a dump of the tree that includes the key, value, and which level it is on [Key]:[Value]:[Level].

#### Syntax

> s

The 's' command code is used for printing stats. This command takes no arguments and prints out the stats for the tree.
