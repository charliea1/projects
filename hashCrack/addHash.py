import hashlib
import string
import sys

# input file name is 1st sys argument -- $python addHash.py input.txt
# INPUT FILE FORMAT: passwords separated by \n:
# password1
# password2
# etc
# all hashes written to hashes.txt

length = len(sys.argv)
# if no user input, exit
if(length < 2):
    print("No arguments, exiting.")
    sys.exit()

# open hashes file for manipulation
outFile = open("hashes.txt", "a")

# if "clear" specified, clear "hashes.txt"
if sys.argv[1] == "clear":
    outFile.truncate(0)
    outFile.close()

#otherwise, try taking argument as input file

else:
    # take first terminal argument as file
    # ensure entry is a valid file name
    valid = False
    try:
        inputFile = open(sys.argv[1])
    except IOError:
        print("Input file not valid.")
        while(not valid):
            try:
                file2 = input("Enter a file name: ")
                inputFile = open(file2)
                valid = True
            except IOError:
                print("Input file not valid.")
                valid = False

    # read file, hash words, enter hashes into output file
    fileStr = inputFile.read()
    fileStr = fileStr.split("\n")
    for line in fileStr:
        # do not read in blank lines
        if not line.strip():
            break

        # perform hashing/entry
        line = line.strip()
        line = hashlib.md5(line.encode())
        line = line.hexdigest()
        if line == fileStr[-1]:
            outFile.write(line)
        else:
            outFile.write(line + "\n")
    inputFile.close()
    outFile.close()
