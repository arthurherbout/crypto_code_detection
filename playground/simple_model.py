#!/usr/bin/env python
# coding: utf-8

import math
import os
import random
import re
import sys
import pandas as pd


def build_list_of_lines(file_location,DIR_NAME):
        text_file = open(DIR_NAME+file_location,"r")
        full = text_file.read().splitlines()
        text_file.close()
        return full



if __name__ == '__main__':
    DIR_NAME="crypto_examples/"
    all_files= os.listdir(DIR_NAME) 
    a=['number of >>','number of ^','number of <<','number of mentions of key',
           'number of mentions of encrypt','number of mentions of decrypt','number of mentions of prime'
          ,'number of mentions of (a)symmetric', 'number of mentions of cipher', 'number of mentions of crypto', 'number of mentions of password'
          ,'number of mentions of bits','number of mentions of generator'
          , 'library import of cryptolib', 'library import of openssl', 'library import of gnutls' ,'library import of cryptlib' 
           ,'library import of gcrypt' ,'library import of sodium' , 'library import of nettle' , 'library import of nss'
           , 'library import of wolfssl','library import of mbedtls']
    df = pd.DataFrame(columns=['File Name']+a)
    file_number=0
    for file_location in all_files :
        crypto_file=build_list_of_lines(file_location,DIR_NAME)
        DD,N,CC,key,encrypt,decrypt,prime,sym,cipher,crypto,passwrd,bits,generator=0,0,0,0,0,0,0,0,0,0,0,0,0
        cryptolib,openssl, gnutls,cryptlib,gcrypt,sodium,nettle,nss,wolfssl,mbedtls=0,0,0,0,0,0,0,0,0,0
        for line in crypto_file :
            if '>>' in line : DD+=1
            if '^' in line : N+=1
            if '<<' in line : CC+=1
            if 'key' in line : key+=1
            if 'encrypt' in line : encrypt+=1
            if 'decrypt'  in line : decrypt +=1
            if 'prime' in line : prime +=1
            if 'symmetric' in line : sym+=1
            if 'cipher' in line : cipher+=1
            if 'crypto' in line : crypto +=1
            if 'password' in line : passwrd +=1 
            if 'bits' in line : bits +=1 
            if 'generator' in line : generator +=1 
            #Library imports in C and C++
            if re.search('include..crypto', line) : cryptolib+=1
            if re.search('include..openssl', line) : openssl+=1
            if re.search('include..gnutls', line)  : gnutls+=1
            if re.search('include..cryptlib', line)  : cryptlib+=1
            if re.search('include..gcrypt', line)  : gcrypt+=1
            if re.search('include..sodium', line) : sodium+=1
            if re.search('include..nettle', line)  : nettle+=1
            if re.search('include..nss', line)  : nss+=1
            if re.search('include..wolfssl', line)  : wolfssl+=1
            if re.search('include..mbedtls', line)  : mbedtls+=1

    
        b=[DD,N,CC,key,encrypt,decrypt,prime,sym,cipher,crypto,passwrd,bits,generator, cryptolib,openssl, gnutls,cryptlib,gcrypt
          ,sodium,nettle,nss,wolfssl,mbedtls]
        df.loc[file_number] = [DIR_NAME+file_location] + b
        res=list(zip(a,b))
        sys.stdout.write("In the file : {}, we have counted : \n".format(file_location)) 
        for i in res : 
            sys.stdout.write(str(i[0])+" "+str(i[1]))
            sys.stdout.write('\n')
        sys.stdout.write('*************************************** \n')
        file_number+=1
    sys.stdout.write('FINISHED READING THE DIRECTORY {}'.format(DIR_NAME))
    df.to_csv("output_data_frame.csv")
