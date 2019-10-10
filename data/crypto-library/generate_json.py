import json
import os

items_to_exclude = ['.DS_Store','.gitignore','README.md']


file_list = []

#Read each crypto_library folder
for crypto_library in os.listdir(os.getcwd()+'/files/'):

	#Make sure we only look at the library folders
	if(crypto_library not in items_to_exclude and not (os.path.isfile(os.path.join(crypto_library)))):
		mypath = 'files/'+crypto_library+'/'
		print(mypath)
		files = [f for f in os.listdir(mypath) if os.path.isfile(os.path.join(mypath, f)) and f.endswith(".c")]
		folders = [f for f in os.listdir(mypath) if not os.path.isfile(os.path.join(mypath, f))]
		print(files)
		for file in files:
			file_path = os.path.join(crypto_library, file)
			print("**********")
			print(file_path)
			with open(file_path,'r') as file_to_read:
				data = file_to_read.read()
			file_list.append({
					'file_path':file_path,
					'file_content':data
				}
				)
		print(file_list)
		print(folders)