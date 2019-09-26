#!/usr/bin/env python
# coding: utf-8

# In[7]:


import math
import os
import random
import re
import sys


# In[4]:


def build_dictionary(dictionary_file_location):
        text_file = open(dictionary_file_location,"r")
        full_dictionary = text_file.read().splitlines()
        text_file.close()
        return full_dictionary


# In[8]:


if __name__ == '__main__':
    fptr = open(os.environ['OUTPUT_PATH'], 'w')

    dictionary_file_location = input()
    crypto_file=build_dictionary(dictionary_file_location)

    DD=0
    N=0
    CC=0

    for line in crypto_file :
        if '>>' in line : DD+=1
        if '^' in line : N+=1
        if '<<' in line : CC+=1
        if 'key' in line : key+=1
        if 'encrypt' in line : encrypt+=1
        if 'decrypt'  in line : decrypt +=1

    fptr.write(' '.join(['>>','^','<<','key','encrypt','decrypt']))
    result=[DD,N,CC,key,encrypt,decrypt]
    fptr.write(' '.join(map(str, result)))
    fptr.write('\n')
    fptr.close()

