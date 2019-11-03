from git_root import git_root
import pandas as pd
import os
import json
import fnmatch

# ctaocrypt folder has been removed

root = git_root()

crypto_files_df = pd.read_json(os.path.join(root,'data','crypto-library','crypto-library_data.json'))

crypto_files = [crypto_files_df.data[i]['file_name'] for i in range(crypto_files_df.shape[0]) if "wolfssl" in crypto_files_df.data[i]['file_name']]


data_folder = os.path.join(git_root(), "result_analysis", "data_slice_test","wolfssl-master")

items_to_exclude = ['.DS_Store','.gitignore','README.md']

path_to_exclude = "/Users/arnaudstiegler/Desktop/crypto_code_detection/result_analysis/data_slice_test/wolfssl-master/tests"

file_list = []


crypto_counter = 0
non_crypto_counter = 0

#Read each crypto_library folder
for path, subdirs, files in os.walk(data_folder):

	for name in files:
				#We match it with our pattern
				if fnmatch.fnmatch(name, "*.c"):
					with open(os.path.join(path, name),'r') as file_to_read:
						data = file_to_read.read()
						if(os.path.join('wolfssl', name) in crypto_files and path != path_to_exclude):
							label=1 #This is crypto
							crypto_counter += 1
						else:
							label= 0 #not crypto
							non_crypto_counter += 1
					file_list.append({
							'file_name':os.path.join(path, name), #Not an actual path but still a unique identifier
							'content':data,
							'label': label,
						}
						)
		

print("Total number of files extracted: {}".format(len(file_list)))
print("Number of crypto files: {}".format(crypto_counter))
print("Number of crypto files: {}".format(non_crypto_counter))
root = git_root()
output_path = os.path.join(root, 'result_analysis','data_slice_test','wolfssl_data_slice.json')

json.dump(file_list, open(output_path,'w'), indent=4)
