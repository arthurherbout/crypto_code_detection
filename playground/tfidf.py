#!/usr/bin/env python
# coding: utf-8

# In[55]:


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
def computeTF(wordDict, bow):
    tfDict = {}
    bowCount = len(bow)
    for word, count in wordDict.items():
        tfDict[word] = count/float(bowCount)
    return tfDict
def computeIDF(docList):
    import math
    idfDict = {}
    N = len(docList)
    
    idfDict = dict.fromkeys(docList[0].keys(), 0)
    for doc in docList:
        for word, val in doc.items():
            if val > 0:
                idfDict[word] = idfDict.get(word,0)+1
    
    for word, val in idfDict.items():
        idfDict[word] = math.log10(N / float(val))
        
    return idfDict
def computeTFIDF(tfBow, idfs):
    tfidf = {}
    for word, val in tfBow.items():
        tfidf[word] = val*idfs[word]
    return tfidf

if __name__ == '__main__':
    DIR_NAME="crypto_examples/"
    all_files= os.listdir(DIR_NAME) 
    wordSet=set()
    res=[]
    tfbows=[]
    for file_location in all_files :
        text_file = open(DIR_NAME+file_location,"r")
        full = text_file.read().replace('\n',' ').replace(';',' ').split(' ')
        wordSet=wordSet.union(set(full))
        wordDictA = dict.fromkeys(wordSet, 0) 
        for word in full: wordDictA[word]+=1
        tfbows.append(computeTF(wordDictA,full))
        res.append(wordDictA)
    df=pd.DataFrame(res)
    idfs=computeIDF(res)
    
    tfidfBows=[]
    for tfBowA in tfbows :  tfidfBows.append(computeTFIDF(tfBowA, idfs))
    
    df=pd.DataFrame(tfidfBows).dropna(axis='columns')
    df['filename'] = all_files
    C=df.columns[:-1]
    df=df[['filename']+list(C)]
    df.to_csv('tf.idf.csv')

