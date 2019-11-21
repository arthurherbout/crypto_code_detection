import os

import json

from tqdm import tqdm
import glob

from git_root import git_root

def data_to_json():
    data = []
    DATA_SOURCE = "code-jam"
    data_path = git_root("data", DATA_SOURCE, "files", "*")
    JSON_OUTPUT = git_root("data", DATA_SOURCE, "code-jam_data.json")
    LABEL = 0
    for filename in tqdm(glob.iglob(data_path)):
        print_filename = os.path.basename(filename)
        with open(filename,'r', errors='replace') as f:
            content = f.read()

        dentry = {"file_name": print_filename, "content": content}
        data.append(dentry)

    json_to_save = {"data_source": DATA_SOURCE, "label": LABEL, "data": data}
    with open(JSON_OUTPUT, 'w') as fp:
        json.dump(json_to_save, fp)

data_to_json()
