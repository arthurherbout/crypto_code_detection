#!/usr/bin/env python
# coding: utf-8


import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
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
dataframe = pd.read_json(os.path.join(data_folder,'full_data_v2.json'))

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
					 lower=True, 
					 split=' ', 
					 char_level=False)

tokenizer.fit_on_texts(docs)

# Size of the vocabulary
print("Size of the vocabulary: {}".format(len(tokenizer.word_index)))

sequences = tokenizer.texts_to_sequences(docs)

len_seqs = [len(s) for s in sequences]
print("Sequences: mean={}, std={}, max={}".format(np.mean(len_seqs), np.std(len_seqs), np.max(len_seqs)))

MAX_LENGTH = 2000


# Dictionary to transform a vector back to the program:
id_to_word = { v: k for k, v in tokenizer.word_index.items() }

# pad documents
padded_docs = pad_sequences(sequences, maxlen=MAX_LENGTH, padding='post')


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
model.add(Dropout(rate=0.4))
model.add(Dense(1, activation='sigmoid'))
    
model.compile(optimizer=Adam(learning_rate=5e-5), 
                  loss='binary_crossentropy', 
                  metrics=['acc', f1,f2,precision_,recall_])

print(model.summary())




def plot(history):
  
  # The history object contains results on the training and test
  # sets for each epoch
	acc = history.history['recall_']
	val_acc = history.history['val_recall_']
	loss = history.history['loss']
	val_loss = history.history['val_loss']

	# Get the number of epochs
	epochs = range(len(acc))

	plt.title('Training and validation recall')
	plt.plot(epochs, acc, color='blue', label='Train')
	plt.plot(epochs, val_acc, color='orange', label='Val')
	plt.xlabel('Epoch')
	plt.ylabel('Recall')
	plt.legend()
	plt.savefig('../code_embedding/plots/training_recall.png')

	_ = plt.figure()
	plt.title('Training and validation loss')
	plt.plot(epochs, loss, color='blue', label='Train')
	plt.plot(epochs, val_loss, color='orange', label='Val')
	plt.xlabel('Epoch')
	plt.ylabel('Loss')
	plt.legend()
	plt.savefig('../code_embedding/plots/training_loss.png')


indices = np.arange(padded_docs.shape[0])

X_train, X_test, y_train, y_test,train_idx, test_idx = train_test_split(padded_docs, labels,indices, 
													test_size=0.2, random_state=0,
													stratify=labels)



# fit the model

history = model.fit(X_train, 
          y_train,
          epochs=1,
          validation_data=(X_test,y_test),
        class_weight = {0: 1.,
                1: 10.})


# # Result QA


y_pred = model.predict(X_test)
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
	sources.append(dataframe.loc[test_idx[elem],'data_source'])
	false_pred.append({"data_source": dataframe.loc[test_idx[elem],'data_source'],
							'label': int(dataframe.loc[test_idx[elem],'label']),
							'file_name': dataframe.loc[test_idx[elem],'file_name']})


result_folder = os.path.join(git_root(), "result_analysis","code_embedding")

with open(os.path.join(result_folder,"code_embedding_false_pred.json"),'w') as outfile:
	json.dump(false_pred,outfile,indent=4)

print("Number of False from others: {}".format(sources.count("github")))
print("Number of False from crypto-competitions: {}".format(sources.count("crypto-competitions")))
print("Number of False from crypto-library: {}".format(sources.count("crypto-library")))
print("Number of False from code-jam: {}".format(sources.count("code-jam")))






