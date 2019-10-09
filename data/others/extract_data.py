import git
import os 
import numpy as np 
import random as rd
import shutil
import stat
import json
import sys

N_SAMPLE = 10
DL_DIR = "C:/Users/coren/columbia/capstone/data/others/gitclone"
DATA_DIR = "C:/Users/coren/columbia/capstone/data/others/files"
JSON_DIR = "C:/Users/coren/columbia/capstone/data/others"
LABEL = 0
DATA_SOURCE = "others"
JSON_OUTPUT = "C:/Users/coren/columbia/capstone/data/others/others_data.json"

list1 = os.popen("pga list -l c").read().split("\n")
list2 = os.popen("pga list -l c++").read().split("\n")
flist = sorted(np.unique(list1+list2))

def del_rw(action, name, exc):
    """Utility function so that shutil can remove the git directory
    (otherwise, the .git folder is read only and cause problems)"""
    os.chmod(name, stat.S_IWRITE)
    os.remove(name)
        


def flush_gitclone(): 
    """This deletes the gitclone directory"""
    shutil.rmtree(DL_DIR, onerror=del_rw)
    
    
def file_sampler():
    """This samples a random repo in the list, and samples a random file from
    it, then copying said file in the DATA_DIR directory. 
    The file has to have a .c, .h, .cpp or .hpp extension."""
    
    # There might be various errors when downloading the repo, so if we 
    # encounter one we just run the function again
    try: 
        # Selecting a random repo
        git_url = flist[rd.randint(0,len(flist)-1)]
        
        # Cloning that repo
        git.Repo.clone_from(git_url, DL_DIR)
        
        pattern = ('.c', '.cpp', '.h', '.hpp','.C', '.cc', '.CPP', '.c++', 
                   '.cp', '.cxx')
        # Getting a list of all the files with the right extension
        valid_files = []
        for path, subdir, files in os.walk(DL_DIR):
            for name in files:
                if name.endswith(pattern):
                    valid_files.append(os.path.join(path, name))
        
        # if no valid files (very unlikely), we run the function again
        
        if valid_files==[]:
            file_sampler()
            
        file_kept = valid_files[rd.randint(0,len(valid_files) - 1)]
        shutil.copy(file_kept, DATA_DIR)
        flush_gitclone()
    except:
        file_sampler()
   
def flush_data():
    """This deletes the contents of the data directory"""    
    shutil.rmtree(DATA_DIR)
    os.mkdir(DATA_DIR)
    
    
def build_data():
    """This populates the data directory with the desired number of samples"""
    for i in range(N_SAMPLE) :
        file_sampler()
        sys.stdout.write("\rLoaded file " + str(i+1) + "/" + str(N_SAMPLE))
        sys.stdout.flush()


def build_json():
    """This creates the json data file with the file names, labels and 
    contents, from the data directory"""
    data = []
    files_list = os.listdir(DATA_DIR)
    for filename in files_list: 
        filepath = DATA_DIR + "/" + filename
        with open(filepath) as f: 
            content = f.read()
        dentry = {"file_name": filename, "content": content}
        data.append(dentry)
    json_to_save = {"data_source": DATA_SOURCE, "label": LABEL, "data": data}
    with open(JSON_OUTPUT, 'w') as fp:
        json.dump(json_to_save, fp)
   
if os.path.exists(DL_DIR):
    flush_gitclone()
flush_data()
build_data()
build_json()