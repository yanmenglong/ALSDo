#!/usr/bin/python
#-*- coding:utf-8 -*-

import os

def clean():
    print "clean files ..."
    dir_path = os.getcwd()
    group = os.walk(dir_path, True)
    for g in group:
        for file_name in g[2]:
            file_path = os.path.join(g[0], file_name)
            if file_path.endswith(".sdf") or file_path.endswith(".obj") or \
               file_path.endswith(".ilk") or file_path.endswith(".pdb") or \
               file_path.endswith(".idb") or file_path.endswith(".pch") or \
               file_path.endswith(".exp") or file_path.endswith(".lib") or \
               file_path.endswith(".ipch") or file_path.endswith(".tlog") or \
               file_path.endswith(".ncb"):
                try:
                    print "remove " + file_path
                    os.remove(file_path)
                except:
                    print "remove " + file_path + " failed !!!!!!!!!!"


    print "clean files finished!\n"
    
def main():    
    clean()
    os.system("pause")
main()


