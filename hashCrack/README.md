Python MD5 hash generator and brute-force password cracker

Run from "hashes.txt" as input with $python crack.py

(prints password \t time taken to crack)

-"hashes.txt" loaded with a few hashes for a quick test

Convert file of passwords to hashes and add to "hashes.txt" with $python addHash.py <filename>

Clear hash file with $python addHash.py clear

-input file must list one password per line

NOTE: brute force method takes about a minute for passwords of len 4; with len>4, multiprocessing is used but still takes minutes to crack

```
FILES:

hashes.txt -- contains hashes to be cracked

crack.py -- main program, reads in hashes from hashes.txt and performs operations

addHash.py -- allows user to quickly generate hashes/enter them into hashes.txt

input.txt -- file for use with addHash.py
```
