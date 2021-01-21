#! /usr/bin/env python
import os
import sys
import shutil

"""
	Deletes unecessar files and dirs form VS projects
"""
def printf(string):
    if sys.version_info.major >= 3:
        print(string)
    """    
    else:
        print string
    """

def to_be_removed(filename, isdir):
	if (isdir and filename.rfind("Debug") > -1 ):
		return True

	if (filename.rfind(".user") > -1):
		return True

	if (filename.rfind(".obj") > -1):
		return True

def clean(PATH):

	for (path, dirs, files) in os.walk(PATH, True):
		"""Treat dirs"""
		for d in dirs:
			dirname = os.path.join(path,d)

			if to_be_removed(dirname, True):
				printf("deleting" + dirname)
				shutil.rmtree(dirname)
				continue

			"""Clean recursivly"""
			clean(d)

		"""Treat file"""
		for f in files:
			filename = os.path.join(path,f);

			if to_be_removed(filename, False):
					printf("deleting " + filename)
					os.remove(filename)
					os.system("git rm " + filename)

def main(args):

    printf(args)
    PATH = "."
    if (len(args) >= 2):
        PATH = args[1]

    printf(PATH)
    clean(PATH)


if __name__ == "__main__":
    sys.exit(main(sys.argv))
