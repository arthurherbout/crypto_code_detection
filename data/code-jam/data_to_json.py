import json

from tqdm import tqdm
import glob

def data_to_json():
    data = []
    DATA_SOURCE = "code-jam_"
    data_path = './files/*'
    JSON_OUTPUT = './code-jam_data.json'
    LABEL = 0
    for filename in tqdm(glob.iglob(data_path)):
        print_filename = filename.split("/")[-1]
        with open(filename,'r', errors='replace') as f:
            content = f.read()

        dentry = {"file_name": print_filename, "content": content}
        data.append(dentry)

    json_to_save = {"data_source": DATA_SOURCE, "label": LABEL, "data": data}
    with open(JSON_OUTPUT, 'w') as fp:
        json.dump(json_to_save, fp)
