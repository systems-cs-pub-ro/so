#!/usr/bin/python

# Check this thread for discussions about the lines counting problem:
#   http://stackoverflow.com/questions/845058/how-to-get-line-count-cheaply-in-python

import time
import mmap
import random
import subprocess
from collections import defaultdict

def mapcount(filename):
    f = open(filename, "r+")
    buf = mmap.mmap(f.fileno(), 0)
    lines = 0
    readline = buf.readline
    while readline():
        lines += 1
    return lines

def simplecount(filename):
    lines = 0
    for line in open(filename):
        lines += 1
    return lines

def bufcount(filename):
    f = open(filename)
    lines = 0
    buf_size = 1024 * 1024
    read_f = f.read # loop optimization

    buf = read_f(buf_size)
    while buf:
        lines += buf.count('\n')
        buf = read_f(buf_size)

    return lines

def opcount(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1

def wc_proc(fname):
    p = subprocess.Popen(['wc', '-l', fname], stdout=subprocess.PIPE,
                                              stderr=subprocess.PIPE)
    result, err = p.communicate()
    if p.returncode != 0:
        raise IOError(err)
    return int(result.strip().split()[0])

counts = defaultdict(list)

FILE = "my_big_file.txt"

for i in range(100):
    for func in [mapcount, simplecount, bufcount, opcount, wc_proc]:
        start_time = time.time()
        func(FILE)
        counts[func].append(time.time() - start_time)

for key, vals in counts.items():
    print key.__name__, ":", sum(vals) / float(len(vals))

