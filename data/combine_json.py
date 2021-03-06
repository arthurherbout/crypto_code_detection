import os
import json
import pandas as pd

from git_root import git_root

BASE_DIR = os.path.join(git_root(), "data")
JSON_OUTPUT = os.path.join(BASE_DIR, "full_data.json")

crypto_library_df = pd.read_json(
    os.path.join(BASE_DIR, "crypto-library", "crypto-library_data.json")
)
crypto_competitions_df = pd.read_json(
    os.path.join(BASE_DIR, "crypto-competitions", "crypto-competitions_data.json")
)
code_jam_df = pd.read_json(
    os.path.join(BASE_DIR, "code-jam", "code-jam_data.json")
)
others_df = pd.read_json(
    os.path.join(BASE_DIR, "others", "others_data.json")
)

full_df = pd.concat(
    [crypto_library_df, crypto_competitions_df, code_jam_df, others_df], 
    ignore_index=True
)

def transform_df(df): 
    """This transform the df formed with the json into the format we want, 
    which is file_name and content as two separate columns instead of being
    unites under data"""
    df['file_name'] = [df.data[i]['file_name'] for i in range(df.shape[0])]
    df['is_header'] = [df.file_name[i].split('.')[-1][0] == 'h' for i in range(df.shape[0])]
    df['content'] = [df.data[i]['content'] for i in range(df.shape[0])]
    df.drop('data', axis=1, inplace=True)
    return(df)
    
full_df = transform_df(full_df)

full_df.to_json(JSON_OUTPUT, orient='records')
