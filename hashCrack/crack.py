import math
import string
import time
import hashlib
import multiprocessing
import sys


# read in file of hashes, convert to string,
infile = open("hashes.txt")
hashes = infile.read()
hashes = hashes.strip()
hashes = hashes.split("\n")
numHashes = len(hashes)
if (not hashes) or (hashes[0] == ""):
    print("Hash file empty, exiting.")
    sys.exit()
numCracked = 0


# use to hash generated password strings


def hash1(strhash):
    result = hashlib.md5(strhash.encode())
    return (result.hexdigest())

# conmpares guess to hash from file, tracks the time elapsed


def compare(password, filehash, initial_time):
    if hash1(password) == filehash:
        # calculate time, print and proceed
        end_time = time.time()
        time_elapsed = (end_time - initial_time)
        print(password + "\t" + str(time_elapsed))
        return True
    else:
        return False


# functions to generate combos of certain sizes
# did this to work in multiprocessing
# explanation: gen5loop function generates the combos of size 5;
# the process is split into sections depending on number of processors used, each executed in parallel

# loop function for gen5, start/end are bounds on the character list so the process can be split up
def gen5loop(list1, start, end, fhash, time_1):
    for i in list1[start:end]:
        for j in list1:
            for y in list1:
                for x in list1:
                    for z in list1:
                        if compare(i + j + y + x + z, fhash, time_1) == True:
                            return True
    return False

# made individual looping functions to accomodate for different combo sizes


def gen6loop(list1, start, end, fhash, t1):
    for i in list1[start:end]:
        for j in list1:
            for y in list1:
                for x in list1:
                    for z in list1:
                        for a in list1:
                            if compare(i + j + y + x + z + a, fhash, t1) == True:
                                return True
    return False


def gen7loop(list1, start, end, fhash, t1):
    for i in list1[start:end]:
        for j in list1:
            for y in list1:
                for x in list1:
                    for z in list1:
                        for a in list1:
                            for b in list1:
                                if compare(i + j + y + x + z + a + b, fhash, t1) == True:
                                    return True
    return False


def gen8loop(list1, start, end, fhash, t1):
    for i in list1[start:end]:
        for j in list1:
            for y in list1:
                for x in list1:
                    for z in list1:
                        for a in list1:
                            for b in list1:
                                for c in list1:
                                    if compare(i + j + y + x + z + a + b + c, fhash, t1) == True:
                                        return True
    return False


# use all printable characters in guesses
chars = string.printable


# main loop
# iterate through each hash in the file
for line in hashes:

    # temp fix for looping issue
    maxi = len(hashes) - 1
    done = False
    if line == hashes[maxi]:
        done = True

    # make sure hash contains no leading/trailing space
    line = line.strip()
    start_time = time.time()
    found = False

    # use loops to generate passwords up to size 4
    # 1
    for i in chars:
        if compare(str(i), line, start_time) == True:
            found = True
            break
    # 2
    if found == True:
        continue
    for i in chars:
        if found:
            break
        for j in chars:
            if compare(i + j, line, start_time) == True:
                found = True
                break
    # 3
    if found == True:
        continue
    for i in chars:
        if found:
            break
        for j in chars:
            if found:
                break
            for y in chars:
                if compare(i + j + y, line, start_time) == True:
                    found = True
                    break
    # 4
    if found == True:
        continue
    for i in chars:
        for j in chars:
            if found:
                break
            for y in chars:
                if found:
                    break
                for x in chars:
                    if compare(i + j + y + x, line, start_time) == True:
                        found = True
                        break

    if found == True:
        if done:
            break
        else:
            continue
    
    # starting at combo size 5, move on to multiprocessing strategy
    # tried to make this tighter with functions for each size/number of processors but was having trouble (not best practice but needed to work)
    p1 = multiprocessing.Process(
        target=gen5loop, args=(chars, 0, 16,  line, start_time, ))
    p2 = multiprocessing.Process(
        target=gen5loop, args=(chars, 17, 33, line, start_time, ))
    p3 = multiprocessing.Process(
        target=gen5loop, args=(chars, 34, 49, line, start_time, ))
    p4 = multiprocessing.Process(
        target=gen5loop, args=(chars, 50, 66, line, start_time, ))
    p5 = multiprocessing.Process(
        target=gen5loop, args=(chars, 67, 83, line, start_time, ))
    p6 = multiprocessing.Process(target=gen5loop, args=(
        chars, 84, 100, line, start_time, ))

    # begin processes at same time
    p1.start()
    p2.start()
    p3.start()
    p4.start()
    p5.start()
    p6.start()

    # if one process yields password, terminate them all
    if ((p1 == True) or (p2 == True) or (p3 == True) or (p4 == True) or (p5 == True) or (p6 == True)):
        p1.terminate()
        p2.terminate()
        p3.terminate()
        p4.terminate()
        p5.terminate()
        p6.terminate()
        if done:
            break
        else:
            continue

    # otherwise, wait for them all to complete
    p1.join()
    p2.join()
    p3.join()
    p4.join()
    p5.join()
    p6.join()

    p1 = multiprocessing.Process(
        target=gen6loop, args=(chars, 0, 16,  line, start_time, ))
    p2 = multiprocessing.Process(
        target=gen6loop, args=(chars, 17, 33, line, start_time, ))
    p3 = multiprocessing.Process(
        target=gen6loop, args=(chars, 34, 49, line, start_time, ))
    p4 = multiprocessing.Process(
        target=gen6loop, args=(chars, 50, 66, line, start_time, ))
    p5 = multiprocessing.Process(
        target=gen6loop, args=(chars, 67, 83, line, start_time, ))
    p6 = multiprocessing.Process(target=gen6loop, args=(
        chars, 84, 100, line, start_time, ))

    # begin processes at same time
    p1.start()
    p2.start()
    p3.start()
    p4.start()
    p5.start()
    p6.start()

    # if one process yields password, terminate them all
    if ((p1 == True) or (p2 == True) or (p3 == True) or (p4 == True) or (p5 == True) or (p6 == True)):
        p1.terminate()
        p2.terminate()
        p3.terminate()
        p4.terminate()
        p5.terminate()
        p6.terminate()
        if done:
            break
        else:
            continue

    # otherwise, wait for them all to complete
    p1.join()
    p2.join()
    p3.join()
    p4.join()
    p5.join()
    p6.join()

    p1 = multiprocessing.Process(
        target=gen7loop, args=(chars, 0, 16,  line, start_time, ))
    p2 = multiprocessing.Process(
        target=gen7loop, args=(chars, 17, 33, line, start_time, ))
    p3 = multiprocessing.Process(
        target=gen7loop, args=(chars, 34, 49, line, start_time, ))
    p4 = multiprocessing.Process(
        target=gen7loop, args=(chars, 50, 66, line, start_time, ))
    p5 = multiprocessing.Process(
        target=gen7loop, args=(chars, 67, 83, line, start_time, ))
    p6 = multiprocessing.Process(target=gen7loop, args=(
        chars, 84, 100, line, start_time, ))

    # begin processes at same time
    p1.start()
    p2.start()
    p3.start()
    p4.start()
    p5.start()
    p6.start()

    # if one process yields password, terminate them all
    if ((p1 == True) or (p2 == True) or (p3 == True) or (p4 == True) or (p5 == True) or (p6 == True)):
        p1.terminate()
        p2.terminate()
        p3.terminate()
        p4.terminate()
        p5.terminate()
        p6.terminate()
        if done:
            break
        else:
            continue

    # otherwise, wait for them all to complete
    p1.join()
    p2.join()
    p3.join()
    p4.join()
    p5.join()
    p6.join()

    p1 = multiprocessing.Process(
        target=gen8loop, args=(chars, 0, 16,  line, start_time, ))
    p2 = multiprocessing.Process(
        target=gen8loop, args=(chars, 17, 33, line, start_time, ))
    p3 = multiprocessing.Process(
        target=gen8loop, args=(chars, 34, 49, line, start_time, ))
    p4 = multiprocessing.Process(
        target=gen8loop, args=(chars, 50, 66, line, start_time, ))
    p5 = multiprocessing.Process(
        target=gen8loop, args=(chars, 67, 83, line, start_time, ))
    p6 = multiprocessing.Process(target=gen8loop, args=(
        chars, 84, 100, line, start_time, ))

    # begin processes at same time
    p1.start()
    p2.start()
    p3.start()
    p4.start()
    p5.start()
    p6.start()

    # if one process yields password, terminate them all
    if ((p1 == True) or (p2 == True) or (p3 == True) or (p4 == True) or (p5 == True) or (p6 == True)):
        p1.terminate()
        p2.terminate()
        p3.terminate()
        p4.terminate()
        p5.terminate()
        p6.terminate()
        if done:
            break
        else:
            continue

    # otherwise, wait for them all to complete
    p1.join()
    p2.join()
    p3.join()
    p4.join()
    p5.join()
    p6.join()

    # if these all return False, then password has not been found

    # moves to next hash
    if numCracked == numHashes:
        print("Done~")
        break


# end of program, close file
infile.close()
