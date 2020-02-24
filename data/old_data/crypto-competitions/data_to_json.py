import os
import json

from git_root import git_root

data_source = "crypto-competitions"
data_folder = os.path.join(git_root(), "data/old_data", data_source)
files_folder = os.path.join(data_folder, "files")

data_dict = {"data_source": data_source, "label": 1, "data": []}

# fill up the dictionary with the files content
for filename in os.listdir(files_folder):
    if not filename.lower().endswith('h'):
        file_path = os.path.join(files_folder, filename)
        
        with open(file_path, 'r') as file:
            file_string = file.read() 
            # strip opening comment for libObfuscate 
            if file_string.startswith(47 * "/"):
                file_string = file_string[546:]
         
        data_dict["data"].append(
            {
                "file_name": filename, "content": file_string
            }
        )

# write resulting json
json_path = os.path.join(data_folder, data_source + "_data.json")
with open(json_path, 'w') as json_to_write:
    json.dump(data_dict, json_to_write, indent=4)
    