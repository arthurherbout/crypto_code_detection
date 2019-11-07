"""
This file contains clustering methods coming from the graph theory field.
"""

# imports
import pandas as pd
import numpy as np
import os
import re
import time

from functools import partial
from scipy.linalg import svd

def remove_content(string):
    # remove all occurrences streamed comments (/*COMMENT */) from string
    string = re.sub(re.compile(r"/\*.*?\*/",re.DOTALL ) , "" , string)
    # remove all occurrence single-line comments (//COMMENT\n ) from string
    string = re.sub(re.compile(r"//.*?\n" ) , "" , string)
    return string

def count_pattern(string, pattern):
    return string.count(pattern)

def compute_count_feature(name):
    global count_features
    pattern = feature_dict[name]
    count_col = content.apply(
        partial(count_pattern, **{"pattern": pattern})
    )
    count_features[name + "_count"] = count_col

def spectral_recursive_embeding(X):
    """
    Implementation of the Spectral Recursive Embedding as defined in the paper
    "Bipartite Graph Partitioning and Data Clustering".
    Arguments:
        - X: numpy array matrix of size (n,m)
    Returns:
        - A, A_, B, B_ : co-clusters
    """
    start = time.time()

    Dx = np.diag(1/np.sqrt(np.sum(X, axis = 1).astype(float)))
    Dy = np.diag(1/np.sqrt(np.sum(X, axis = 0).astype(float)))

    W = np.linalg.multi_dot([Dx, X, Dy])
    W = W.astype(float)

    U, _, Vh = svd(W)

    # taking the second largest left and right singular vectors
    x_hat = U[1]
    y_hat = Vh[1]


    x = Dx @ x_hat
    y = Dy @ y_hat

    A = np.argwhere(x >= 0).flatten()
    A_ = np.argwhere(x < 0).flatten()

    B = np.argwhere(y >= 0).flatten()
    B_ = np.argwhere(y < 0).flatten()

    end = time.time()

    print("Function executed in %.3f seconds"%(end - start))

    return A, A_, B, B_

def multipartitioning(X, n_clusters = 2, n_dim_embedding = 2):
    """
    Implementation of the Bipartition algorithm as defined in the paper
    "Co-clustering documents and words using Bipartite Spectral Graph Partitioning".
    Arguments:
        - X: numpy array matrix of size (n,m)
        - n_clusters:
        - n_dim_embedding:
    Returns:
        - Z: n dimensional embedding of the data
        - labels: labels based on KMeans on the Z data
        - clusters: clusters found by KMeans on the Z data
    """
    start = time.time()

    # alias for the embedding dimension
    l = n_dim_embedding

    Dx = np.diag(1/np.sqrt(np.sum(X, axis = 1).astype(float)))
    Dy = np.diag(1/np.sqrt(np.sum(X, axis = 0).astype(float)))

    W = np.linalg.multi_dot([Dx, X, Dy])
    W = W.astype(float)

    # computing Singular Value Decomposition
    print("Computing SVD...")
    U, _, Vh = svd(W)
    print("...done \n")

    # taking the most important right and left vectors
    u = np.transpose([U[i + 1] for i in range(l)])
    v = np.transpose([Vh[i + 1] for i in range(l)])

    # forming l-dimensional data
    Z_x = Dx @ u
    Z_y = Dy @ v
    Z = np.concatenate((Z_x, Z_y), axis = 0)

    # running KMeans on Z
    print("Running KMeans on {} clusters in the {}-dimension data...".format(n_clusters, l))
    clustering = KMeans(n_clusters=n_clusters).fit(Z)
    print("...done")

    # getting execution time
    end = time.time()

    print("Function executed in %.3f seconds"%(end - start))


    return Z, clustering.labels_, clustering.cluster_centers_

if __name__ == "__main__":

    dataframe = pd.read_json('../data/full_data.json')
    content = dataframe["content"]
    dataframe = dataframe.drop(columns=["content"])
    content = content.apply(remove_content)

    feature_dict = {
    "proxy_line": "\n",
    "proxy_comment": "//",
    "proxy_multiline_comment": "/*",
    "proxy_int": "int",
    "proxy_long": "long",
    "proxy_while_loops": "while",
    "proxy_for_loops": "for",
    "proxy_include": "#include",
    "proxy_bit_left_shift": "<<",
    "proxy_bit_right_shift": ">>",
    "proxy_bitwise_and": "&",
    "proxy_complement": "~",
    "proxy_bitwise_xor": "^",
    "proxy_xor": "xor",
    "proxy_hexadecimal": "0x",
    }

    count_features = {}

    _ = [compute_count_feature(name) for name in feature_dict]

    dataframe = dataframe.assign(**count_features)
    loops = ["proxy_while_loops_count", "proxy_for_loops_count"]
    bitwise_ops = ["proxy_bit_left_shift_count", "proxy_bit_right_shift_count",
               "proxy_bitwise_and_count", "proxy_complement_count",
               "proxy_bitwise_xor_count", "proxy_xor_count"]

    dataframe = dataframe.assign(
    proxy_loops_count = sum(dataframe[col] for col in loops),
    proxy_bitwise_count = sum(dataframe[col] for col in bitwise_ops)
    )

    columns_to_be_divided = dataframe.columns.values[5:]
    dataframe[columns_to_be_divided] = dataframe[columns_to_be_divided].div(dataframe.proxy_line_count, axis=0)
    dataframe = dataframe[dataframe.proxy_line_count != 0]
    dataframe.fillna(0, inplace=True)

    X = dataframe.values[:, 4:]

    row_sums = X.sum(axis=1)
    X = X / row_sums[:, np.newaxis]

    A, A_, B, B_ = spectral_recursive_embeding(X)
    labels_A = dataframe.iloc[A].label.values
    labels_A_ = dataframe.iloc[A_].label.values
    labels_B = list(dataframe.columns[B + 4])
    labels_B_ = list(dataframe.columns[B_ + 4])
    n_A1 = sum(labels_A)
    n_A0 = len(labels_A) - n_A1
    n_A_1 = sum(labels_A_)
    n_A_0 = len(labels_A_) - n_A_1
    print("\nResults of spectral recursive embedding:")
    print("First cluster: A and B")
    print("number of non crypto code in A : %i" %n_A0)
    print("number of crypto code in A : %i \n" %n_A1)
    print("selected features for first cluster: {}\n \n".format(labels_B))
    print("Second cluster: A_ and B_")
    print("number of non crypto code in A_ : %i" %n_A_0)
    print("number of crypto code in A_ : %i \n" %n_A_1)
    print("selected features for second cluster: {}\n \n".format(labels_B_))
