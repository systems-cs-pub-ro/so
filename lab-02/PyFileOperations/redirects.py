#!/usr/bin/python
import sys
 
# a simple class with a write method
class WritableObject:
    def __init__(self):
        self.content = []
    def write(self, string):
        self.content.append(string)
 
# Exemplu de redirectare sys.stdout
foo = WritableObject()                   # a writable object
sys.stdout = foo                         # redirection
print "one, two, three, four"            # some writing
print "little hat"
print "little hat"
sys.stdout = sys.__stdout__              # remember to reset sys.stdout!
print "foo's content:", foo.content                # show the result of the writing
 
# example with redirection of the print statement
bar = WritableObject()                   # another writable object
print >>bar, "one, two, three, four"     # some (redirected) writing
print >>bar, "little hat made of paper"
print "bar's content:", bar.content                # the result

