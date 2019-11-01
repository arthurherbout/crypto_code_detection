import json
import os
import fnmatch

from git_root import git_root

items_to_exclude = ['.DS_Store','.gitignore','README.md']

file_list = []

data_folder = os.path.join(git_root(), "data", "crypto-library")

#Read each crypto_library folder
for crypto_library in os.listdir(os.path.join(data_folder, 'files')):

	library_file_counter = 0

	#Make sure we only look at the library folders
	if(crypto_library not in items_to_exclude and not (os.path.isfile(os.path.join('files/',crypto_library)))):

		root = os.path.join(data_folder, 'files', crypto_library)
		pattern = "*.c"

		print("<--- Processing new library --->")
		print(crypto_library)
		#Go through the entire list of files (embedded in folders or not)

		for path, subdirs, files in os.walk(root):
			for name in files:

				#We match it with our pattern
				if fnmatch.fnmatch(name, "*.c") or fnmatch.fnmatch(name, "*.h"):
					with open(os.path.join(path, name),'r') as file_to_read:
						data = file_to_read.read()
					file_list.append({
							'file_name':os.path.join(crypto_library, name), #Not an actual path but still a unique identifier
							'content':data
						}
						)
					library_file_counter += 1
		

		print("Found {} files".format(library_file_counter))
		print("\n")

print("Total number of files extracted: {}".format(len(file_list)))

json_output = {'data_source':'crypto-library',
				'label':1,
				'data': file_list}

output_path = os.path.join(data_folder, 'crypto-library_data.json')

json.dump(json_output, open(output_path,'w'), indent=4)
