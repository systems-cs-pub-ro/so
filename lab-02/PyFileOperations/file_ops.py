#!/usr/bin/python

import os
import stat
import time

MAX_NUM = 1024 * 1024

# ord(x) = codul ASCII a lui x
MAX = ord('z') - ord('a') + 1

class TestFile:
    def __init__(self, file_name):
        # Deschid un fisier si afisez structura lui din python
        self.file_name = file_name

        try:
            # Daca fisierul nu exista, atunci el nu se poate deschide in Read+Write
            # cu functia standard open()
            # Deci, open() e cam limitata....
            self.file = open(file_name, 'r+')
        except:
            # Apelez o functie care executa cod nativ
            self.file = self.low_level_io()
        print self.file

    def low_level_io(self):
        # Vezi http://docs.python.org/library/os.html
        return os.fdopen(os.open(self.file_name, os.O_CREAT | os.O_RDRW))

    def generate(self):
        file_stats = os.stat(self.file_name)

        print '[Raw file stat] ' + str(file_stats)
        print 'Compare this result with <<man 2 stat>>'

        file_info = {
            'fname': self.file_name,
            'fsize': file_stats [stat.ST_SIZE],
            'f_lm': time.strftime("%m/%d/%Y %I:%M:%S %p",time.localtime(file_stats[stat.ST_MTIME])),
            'f_la': time.strftime("%m/%d/%Y %I:%M:%S %p",time.localtime(file_stats[stat.ST_ATIME])),
            'f_ct': time.strftime("%m/%d/%Y %I:%M:%S %p",time.localtime(file_stats[stat.ST_CTIME]))
        }

        print "file name = %(fname)s" % file_info
        print "file size = %(fsize)s bytes" % file_info
        print "last modified = %(f_lm)s" % file_info
        print "last accessed = %(f_la)s" % file_info
        print "creation time = %(f_ct)s" % file_info

        # Check if file was already generated
        if (file_info['fsize'] > 0):
            return

        for i in range(MAX_NUM):
            # chr(x) = codul caracter al numarului x
            self.file.write(chr(ord('a') + i % MAX))

    def close(self):
        self.file.close()

    def read_byte(self, index = 0, base = 0):
        self.file.seek(index, base)
        return self.file.read(1)

    def read(self, size):
        return self.file.read(size)

t_file = TestFile("test.txt")
t_file.generate()

# Citesc un octet de pe o anumita pozitie
print t_file.read_byte()
print t_file.read_byte(1)

# Incarc tot fisierul in memorie
print t_file.read(-1)

t_file.close()

