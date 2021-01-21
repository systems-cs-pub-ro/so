#! /usr/bin/env python

import os
import sys
import CommentHeader


SKEL_PATH_LIN = '/skel/lin'
SKEL_PATH_WIN = '/skel/win'

SOL_PATH_LIN = '/sol/lin'
SOL_PATH_WIN = '/sol/win'

COMMENT_FILE_LIN = '/.comment_file_lin'
COMMENT_FILE_WIN = '/.comment_file_win'


def parse_comments(file_path):
    """Read comment file

    Entries are in 2 forms:
        N   COMMENT     - apply header to all .c .h files in labN folder
        N-FILE  COMMENT - apply header only to FILE from labN folder
    """

    if (not file_path): return

    try:
        f = open(file_path)
    except IOError, (errno, strerror):
        print >> sys.stderr, \
                "open(%s) error(%s): %s" % (file_path, errno, strerror)
        exit()

    comments = []

    while 1:
        line = f.readline()
        if (len(line) == 0): break  # stop condition

        line = line.strip(' \n\t')
        if ((len(line) == 0) or (line[0] == '#')): continue

        comments.append(line.split(None, 1))

    f.close()
    return dict(comments)


def set_header_in_file(file_path, header):
    """Rewrite file so it contains the right comment header"""

    try:
        f = open(file_path, 'r')
    except IOError, (errno, strerror):
        print >> sys.stderr, "open(%s) error(%s): %s" % (file_path, errno, strerror)
        return

    skipped = False

    """Generate the corect comment header."""
    file_content = header.to_string()

    while 1:
        line = f.readline()

        if (line.find('#include') >= 0) or \
           (line.find('#ifndef') >=0) or \
           (line.find('#define') >=0):
            skipped = True

        """ 
            We presume that we have skipped the comment header
            and it's safe to start saving lines
        """
        if skipped:
            file_content += line

        if (len(line) == 0):    # stop condition
            break;
    #print file_content

    f.close();

    """Overwrite the file"""
    try:
        f = open(file_path, 'w');
    except IOError, (errno, strerror):
        print >> sys.stderr, \
                "open(%s) error(%s): %s" % (file_path, errno, strerror)
        return

    f.truncate(0)
    f.write(file_content)
    f.close()


def get_lab_no(lab_path):
    """Extract lab number from lat path.

    Lab name should be in the form of /path/labxx
    """
    aux = lab_path.split('/')
    if (not aux): 
        print 'Lab folder name should be like labXX'
        exit()

    if (aux[-1] == ''):
        lab_no = aux[len(aux) - 2]
    else:
        lab_no = aux[len(aux) - 1]
    
    try:
        lab_no = int(lab_no.lstrip('lab'))
    except ValueError:
        print >> sys.stderr,'Lab folder name should be like labXX'
        exit()
    return lab_no


def set_headers(comments, skel_or_sol_path, header):

     global lab_path
     global lab_name
     global lab_no
     global platform

     for (path, dirs, files) in os.walk(skel_or_sol_path):
        if (path == skel_or_sol_path): continue # skip root path
        
        split_path = path.split('-')
        """Not a task folder - probably 'utils' folder"""
        if (len(split_path) != 2): continue

        ex_no = path.split('-')[0][-1]
        ex_name = path.split('-')[1]
        
        """Keys could have two forms:
        - N  -  all source files (.c, .h) for task N should have 
                the same comment
        - N-file - only 'file' this comment
        """
        if comments.has_key(ex_no):
            comment = comments[ex_no]
        else:
            comment = ''


        header.set( lab_no, 
                    lab_name, 
                    ex_no, 
                    platform,
                    comment)

        for file in files:
            if (file.find('.c') > -1 or file.find('.h') > -1):
                print "Setting header in [lab%d : %s-%s : %s]" \
                        % (lab_no, ex_no, ex_name, file)

                if (comments.has_key(ex_no + "-" + file)):
                    header.comment = comments[ex_no + "-" + file]

                set_header_in_file(path + '/' + file, header)
                print header.to_string()

def main(args):
    if (len(args) < 4):
        print("Usage:\n\tpython " 
            + args[0] + " [lab_root_dir] [lab_name] [lin/win]\n\n"
            + "Example call\n\t"
            + "python "  + args[0] 
            + " /home/catalin/so_stuff/sandbox/lab03 Procese lin")
        return -1

    global lab_path
    global lab_name
    global lab_no
    global platform
    
    lab_path = args[1]
    lab_name = args[2]
    lab_no = get_lab_no(args[1]) 
    platform = args[3]

    if platform.lower().find('lin') > -1:
        comment_file = lab_path + COMMENT_FILE_LIN
        skel_path = lab_path + SKEL_PATH_LIN 
        sol_path  = lab_path + SOL_PATH_LIN 
    else:
        comment_file = lab_path + COMMENT_FILE_WIN
        skel_path = lab_path + SKEL_PATH_WIN 
        sol_path  = lab_path + SOL_PATH_WIN 


    """Load tasks descriptions from file"""
    comments = parse_comments(comment_file)

    header = CommentHeader.CommentHeader()

    """Set right comment header for every ex in lab"""
    
    """ Skel """
    set_headers(comments, skel_path, header)
    
    """ Sol """
    set_headers(comments, sol_path, header)
    
    return 0

if __name__ == "__main__":
    sys.exit(main(sys.argv))


