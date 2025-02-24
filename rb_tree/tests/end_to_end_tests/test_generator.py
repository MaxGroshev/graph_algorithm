import subprocess
import argparse
import random
import os
import sys

# -----------------------------------------------------------------------------------------

def print_test_data(args, test_data):
    dat_file = open(args.file, "w")

    num_of_elem = 1
    for (val) in (test_data.values()) :
        for i in val:
            num_of_elem += 1
            dat_file.write("k %d "%  i)
            if (not(num_of_elem % 10)):
                dat_file.write("\n")
    dat_file.write("q %d %d "%  (args.lub[0], args.lub[1]))
    for (key) in (test_data.keys()):
        dat_file.write(": (%d) %d "%  (key, args.num))

    dat_file.close()


def generate_test_data(args):
    rand_list = []
    my_set = set(rand_list)

    while (len(my_set) != args.num):
        val = random.randint(args.vd[0], args.vd[1])
        if val not in my_set:
            my_set.add(val)
            rand_list.append(val)
            continue

    res = 0
    my_list = list(my_set)
    for elem in my_list:
        if (elem >= args.lub[0] and elem <= args.lub[1]):
            res += 1

    test_data = {res : rand_list}
    return test_data

# -----------------------------------------------------------------------------------------

def add_parse_arguments(parser):
    parser.add_argument("-n", "--num", type = int, default = 0)
    parser.add_argument('-vd', nargs = 2, type = int, default = [0, 0])
    parser.add_argument('-f', '--file', type = str, default = os.path.dirname(os.path.abspath(__file__)) +
                                                              '/my_test_dat/gen_test.dat')
    parser.add_argument('-lub', nargs = 2, type = int, default = [0, 0])

def check_args(args):
    if (args.vd[0] >= args.vd[1] or args.lub[0] >= args.lub[1] or args.num > args.vd[1] - args.vd[0]):
        print("ERROR")
        exit()

# -----------------------------------------------------------------------------------------

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    add_parse_arguments(parser)
    args = parser.parse_args()
    check_args(args)

    test_data = generate_test_data(args)
    print_test_data(args, test_data)

