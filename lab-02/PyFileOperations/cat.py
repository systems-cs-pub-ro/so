#!/usr/bin/python

import sys

# Aplicatie echivalenta cu utilitarul "cat"

class CatFile:
    def __init__(self, file_name):
        self.file = open(file_name, 'r')

    def cat(self):
        for line in self.file:
            print line[:-1]

if __name__ == '__main__':
    for arg in sys.argv[1:]:
        CatFile(arg).cat()

