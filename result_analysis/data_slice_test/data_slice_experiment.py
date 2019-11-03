#!/usr/bin/env python
# coding: utf-8


import pandas as pd
import numpy as np
import re
import os
import json

from tensorflow.keras.preprocessing.text import Tokenizer
from tensorflow.keras.preprocessing.sequence import pad_sequences
from tensorflow.keras import backend as K
from tensorflow.keras.models import Sequential
from tensorflow.keras.layers import Flatten, Dense, Dropout
from tensorflow.keras.layers import Embedding
from tensorflow.keras.optimizers import Adam

from sklearn.model_selection import train_test_split

from git_root import git_root

data_folder = os.path.join(git_root(), "data")
dataframe = pd.read_json(os.path.join(data_folder,'full_data.json'))

#We remove all files belonging to the wolfssl package
dataframe = dataframe.loc[dataframe['file_name'].str.contains('wolfssl') == False]


def remove_comments(string):
	# remove all occurrences streamed comments (/*COMMENT */) from string
	string = re.sub(re.compile(r"/\*.*?\*/",re.DOTALL ) , "" , string) 
	# remove all occurrence single-line comments (//COMMENT\n ) from string
	string = re.sub(re.compile(r"//.*?\n" ) , "" , string)
	return string


def get_docs_and_labels(df):
	_docs = []
	_labels = []
	for index in df.index:
		# Program
		
		code = remove_comments(
			df.at[index, 'content']
		)
		_docs.append(code)
		# Label
		label = int(df.at[index, 'label'])
		_labels.append(label)
	return _docs, _labels

docs, labels = get_docs_and_labels(dataframe)

#To ignore the warnings
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '3'


#We set the size of the vocabulary
NUM_WORDS = 10000

tokenizer = Tokenizer(num_words=NUM_WORDS, 
					 filters='\t\n,', 
					 lower=True, 
					 split=' ', 
					 char_level=False)

tokenizer.fit_on_texts(docs)

# Size of the vocabulary
print("Size of the vocabulary: {}".format(len(tokenizer.word_index)))

sequences = tokenizer.texts_to_sequences(docs)

len_seqs = [len(s) for s in sequences]
print("Sequences: mean={}, std={}, max={}".format(np.mean(len_seqs), np.std(len_seqs), np.max(len_seqs)))

MAX_LENGTH = 1000


# Dictionary to transform a vector back to the program:
id_to_word = { v: k for k, v in tokenizer.word_index.items() }

# pad documents
padded_docs = pad_sequences(sequences, maxlen=MAX_LENGTH)


## Defining the model

#We add the epsilon to avoid any error in the function

def precision_(y_true, y_pred):
	true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
	predicted_positives = K.sum(K.round(K.clip(y_pred, 0, 1)))
	precision = true_positives / (predicted_positives + K.epsilon())
	return precision
	
def recall_(y_true, y_pred):
	true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
	possible_positives = K.sum(K.round(K.clip(y_true, 0, 1)))
	recall = true_positives / (possible_positives + K.epsilon())
	return recall

def f1(y_true, y_pred):
	
	precision = precision_(y_true, y_pred)
	recall = recall_(y_true, y_pred)
	return 2*((precision*recall)/(precision+recall+K.epsilon()))

def f2(y_true, y_pred):
	
	precision = precision_(y_true, y_pred)
	recall = recall_(y_true, y_pred)
	return 5*((precision*recall)/(4*precision+recall+K.epsilon()))



# define the model
model = Sequential()
model.add(Embedding(NUM_WORDS, 50, input_length=MAX_LENGTH))
model.add(Flatten())
model.add(Dropout(rate=0.3))
model.add(Dense(1, activation='sigmoid'))
	
model.compile(optimizer=Adam(learning_rate=0.001), 
				  loss='binary_crossentropy', 
				  metrics=['acc', f1,f2,precision_,recall_])

print(model.summary())

indices = np.arange(padded_docs.shape[0])


# <----- We don't train test split because test is the wolfssl package ? (To check validity of that) ------>


X_train, X_test, y_train, y_test,train_idx, test_idx = train_test_split(padded_docs, labels,indices, 
													test_size=0.2, random_state=0,
													stratify=labels)



# fit the model

history = model.fit(X_train, 
		  y_train,
		  epochs=10,
		  validation_data=(X_test,y_test))


# # Extract the wolfssl files to test on those


df_wolfssl = pd.read_json(os.path.join(git_root(),'result_analysis','data_slice_test','wolfssl_data_slice.json'))

df_wolfssl['content'] = df_wolfssl['content'].apply(lambda x: remove_comments(x))

#Preprocess the files with the same tokenizer
test_seq = tokenizer.texts_to_sequences(df_wolfssl['content'])
test_padded = pad_sequences(test_seq, maxlen=MAX_LENGTH)
y_test = df_wolfssl['label']


#Predict the labels for the files
y_pred = model.predict(test_padded)
super_threshold_indices_1 = y_pred >= 0.5
super_threshold_indices_0 = y_pred < 0.5
y_pred[super_threshold_indices_1] = 1
y_pred[super_threshold_indices_0] = 0
y_pred = y_pred.reshape(y_pred.shape[0])
y_false = np.where(y_pred != y_test)[0]

print("Number of False: {}".format(y_false.shape[0]))

false_pred = []

sources = []
for elem in y_false:
	false_pred.append({'file_name': str(df_wolfssl.loc[elem,'file_name']),
						'content': str(df_wolfssl.loc[elem,'content']),
						'true_label': str(y_test)})


result_folder = os.path.join(git_root(), "result_analysis","data_slice_test")

with open(os.path.join(result_folder,"wolfssl_false_pred.json"),'w') as outfile:
	json.dump(false_pred,outfile,indent=4)

header_0 = 0
header_1 = 0
for elem in y_false:
		if(df_wolfssl.loc[elem,'label']==1):
			print(df_wolfssl.loc[elem,'file_name'])
			header_1 += 1
		else:
			header_0 += 1
print("Number of non-crypto classified as crypto: {}".format(header_0))
print("Number of crypto classified as non-crypto: {}".format(header_1))


results = model.evaluate(test_padded, y_test)

print('Test Accuracy: {:.2f}'.format(results[1]))
print('Test F1-score: {:.2f}'.format(results[2]))
print('Test F2-score: {:.2f}'.format(results[3]))
print('Test Precision: {:.2f}'.format(results[4]))
print('Test Recall: {:.2f}'.format(results[5]))


