#! /usr/bin/env python
import os
import sys

LAB_NAMES_FILE = 'lab_names'
LAB_FOLDER_PATH = '..'
HEADER_SCRIPT = 'set-header.py'
LINKS_SCRIPT = 'set-links.py'

def set_header(lab, lab_names, silent, sep):
    found_labs = False # test if lab folders were found
    
    if  lab_names.has_key(lab):
        print lab, sep
        lab_name = lab_names[lab]
        
        os.system('python ' + HEADER_SCRIPT + ' ' \
                    + LAB_FOLDER_PATH + '/' + lab + ' ' \
                    + lab_name + ' lin' \
                    + silent
                    )
        found_labs = True
    return found_labs
    
def set_links(lab, lab_names, silent, sep):
    found_labs = False
    if  lab_names.has_key(lab):
        print lab, sep  
        
        os.system('python ' + LINKS_SCRIPT + ' ' \
                    + LAB_FOLDER_PATH + '/' + lab + ' ' \
                    + silent
                    )        
        found_labs = True
    return found_labs

def load_lab_names():
    try:
        f = open(LAB_NAMES_FILE);
    except IOError, (errno, strerror):
        print "open(%s) error(%s): %s" % (file_path, errno, strerror)
        exit()
        
    lab_names = []
    
    while 1:
        line = f.readline()
        if (len(line) == 0): break  # stop condition

        line = line.strip(' \n\t')
        if ((len(line) == 0) or (line[0] == '#')): continue

        lab_names.append( line.split(None, 1) )
        
    f.close()
    
    return dict(lab_names)


def main(args):
    lab_names = load_lab_names()
    found_labs = False;
    
    actions = []
    
    if 'links' in args:
        actions.append(set_links)
        
    if 'header' in args:
        actions.append(set_header)
        
    
    sep = ''
    if not '-v' in args:
        silent = ' > /dev/null'
    else:
        silent = ''
        for i in range(70): sep+='#'
        
    """Check to see if we must set header only for some labs"""
    for i in args:
        for action in actions:
            found_labs = found_labs or action(i, lab_names, silent, sep)
            
    if found_labs: return 0 # work done
    
    """Set header for all labs"""
    for (path, dirs, files) in os.walk(LAB_FOLDER_PATH):
        for d in dirs:
            for action in actions:
                action(d, lab_names, silent, sep)
        break   # do this only for root folder
    

if __name__ == "__main__":
    sys.exit(main(sys.argv))
