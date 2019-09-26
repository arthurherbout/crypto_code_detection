#!/usr/bin/env python
# coding: utf-8

import math
import os
import random
import re
import sys



def build_list_of_lines(file_location):
        text_file = open(file_location,"r")
        full = text_file.read().splitlines()
        text_file.close()
        return full



if __name__ == '__main__':
    sys.stdout.write("Please provide name of .txt file for the code :")
    file_location = input()
    crypto_file=build_list_of_lines(file_location)
    DD,N,CC,key,encrypt,decrypt,prime=0,0,0,0,0,0,0
    for line in crypto_file :
        if '>>' in line : DD+=1
        if '^' in line : N+=1
        if '<<' in line : CC+=1
        if 'key' in line : key+=1
        if 'encrypt' in line : encrypt+=1
        if 'decrypt'  in line : decrypt +=1
        if 'prime' in line : prime +=1
    
    a=['number of >>','number of ^','number of <<','number of mentions of key','number of mentions of encrypt','number of mentions of decrypt','number of mentions of prime']
    b=[DD,N,CC,key,encrypt,decrypt,prime]
    res=list(zip(a,b))
    for i in res : 
        sys.stdout.write(str(i[0])+" "+str(i[1]))
        sys.stdout.write('\n')

