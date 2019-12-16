import os
import pandas as pd

from git_root import git_root

BASE_DIR = os.path.join(git_root(), "data")
JSON_OUTPUT = os.path.join(BASE_DIR, "full_data.json")

old_df = pd.read_json(
    os.path.join(BASE_DIR, "old_data", "old_data.json")
)

new_df = pd.read_json(
    os.path.join(BASE_DIR, "new_data", "full_data.json")
)

new_df.drop(columns=["source_username", "source_repo", "file_path"])

old_df.drop(columns=["is_header"])

full_df = pd.concat(
    [old_df, new_df], 
    ignore_index=True
)

full_df.to_json(JSON_OUTPUT, orient='records')

