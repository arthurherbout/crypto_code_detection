import json
import os
import fnmatch

items_to_exclude = ['.DS_Store','.gitignore','README.md']


file_list = []

#Read each crypto_library folder
for crypto_library in os.listdir(os.getcwd()+'/files/'):

	library_file_counter = 0

	#Make sure we only look at the library folders
	if(crypto_library not in items_to_exclude and not (os.path.isfile(os.path.join('files/',crypto_library)))):

		root ='files/'+ crypto_library+'/'
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
							'file_name':os.path.join(path, name),
							'content':data
						}
						)
					library_file_counter += 1
		

		print("Found {} files".format(library_file_counter))
		print("\n")

print("Total number of files extracted: {}".format(len(file_list)))

json_output = {'data_source':'crypto_library',
				'label':1,
				'data': file_list}

json.dump(json_output, open('crypto_library_data.json','w'), indent=4)
