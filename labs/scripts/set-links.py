#! /usr/bin/env python

import os
import sys
import stat

SKEL_PATH = '/skel/lin'
SOL_PATH = '/sol/lin'

def main(args):
    if len(args) < 2:
        print "Usage:\n\t" \
                + " python " + args[0] \
                + " lab_path"
        return 0
                
    lab_path = args[1]
    skel_path = lab_path + SKEL_PATH
    
    
    """Set right comment header for every ex in lab"""
    for (path, dirs, files) in os.walk(skel_path):
        if (path == skel_path): continue # skip root path
        
        for file in files:
        
            """Clen up by calling make clean"""
            """
            clean_skel = 'cd ' + path + '/' + ' ; make clean '
            print clean_skel
            os.system(clean_skel)
            
            clean_sol = clean_skel.replace('skel', 'sol')
            print clean_sol
            os.system(clean_sol)
            contiune
            """
        
            skel_path = path + "/" + file
            sol_path = path.replace('skel','sol') + "/" + file
        
            rc = os.system('diff -wBe ' + skel_path + ' ' 
                                        + sol_path + ' > /dev/null');
                                                    
            if rc == 0 or rc == 512: # 512 no file or broken link
            
                """Only way tot test if a file is a link."""
                """
                rc = os.system('stat --format=%F ' + sol_path + ' | grep link | wc -l || false')
                if rc == 1: continue # already link
                """
                
                relative_path = skel_path.replace(lab_path, '../../..');
                print 'ln -s -f ' + relative_path + ' ' + sol_path
                rc = os.system('ln -s -f ' + relative_path + ' ' + sol_path)

if __name__ == "__main__":
    sys.exit(main(sys.argv))
