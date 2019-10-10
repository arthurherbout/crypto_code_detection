import git
import os 
import numpy as np 
import random as rd
import shutil
import stat
import json
import sys
import glob

N_SAMPLE = 5000
REPO_SIZE_LIMIT = 10000000
BASE_DIR = "C:/Users/coren/columbia/capstone/data/others"
DL_DIR = BASE_DIR + "/gitclone"
DATA_DIR = BASE_DIR + "/files"
JSON_OUTPUT = BASE_DIR + "/others_data.json"
LABEL = 0
DATA_SOURCE = "others"


def csv_to_list(csv):
    """Helper function that takes in repo info in a csv format and return
    the list of repos with valid size (above 0 and below REPO_SIZE_LIMIT)"""
    repo_list = []
    for i in range(len(csv)):
        repo_line = csv[i].split(',')
        repo_url = repo_line[0]
        sizechar = repo_line[-1]
        if repo_url != '': 
            # If size is unavailable (or 0 or 1), we don't want to download the repo
            if sizechar != '': 
                size = int(sizechar)
                if size < REPO_SIZE_LIMIT and size > 0:
                    repo_list.append(repo_url)
    return(repo_list)
    
def load_repo_list():
    csv1 = os.popen("pga list -l c -f csv").read().split("\n")
    csv2 = os.popen("pga list -l c++ -f csv").read().split("\n")
    list1 = csv_to_list(csv1)
    list2 = csv_to_list(csv2)
    repo_list = sorted(np.unique(list1+list2))
    return(repo_list)

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
    
    # Selecting a random repo
    git_url = repo_list[rd.randint(0,len(repo_list)-1)]
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
        flush_gitclone()
        print("no valid files")
        raise ValueError("No valid files")
    else:
        file_kept = valid_files[rd.randint(0,len(valid_files) - 1)]
        shutil.copy(file_kept, DATA_DIR)
        flush_gitclone()
   
    
def flush_data():
    """This deletes the contents of the data directory"""    
    files = glob.glob(DATA_DIR + '/*')
    for f in files:
        os.remove(f)
    
    
def build_data():
    """This populates the data directory with the desired number of samples"""
    i = 0
    while i < N_SAMPLE :
        sys.stdout.write("\rLoading file " + str(i+1) + "/" + str(N_SAMPLE))
        sys.stdout.flush()
        # There might be errors when sampling files (because file is too big
        # or because of a git clone error or because there are no c files in 
        # the repo). If an error is encountered, we just
        # go to the next iteration and don't download the file.
        try: 
            file_sampler()
        except:
            print("load failed")
            i = i - 1
        i = i + 1 


def build_json():
    """This creates the json data file with the file names, labels and 
    contents, from the data directory"""
    data = []
    files_list = os.listdir(DATA_DIR)
    for filename in files_list: 
        filepath = DATA_DIR + "/" + filename
        with open(filepath, 'r', errors='replace') as f: 
            content = f.read()
        dentry = {"file_name": filename, "content": content}
        data.append(dentry)
    json_to_save = {"data_source": DATA_SOURCE, "label": LABEL, "data": data}
    with open(JSON_OUTPUT, 'w') as fp:
        json.dump(json_to_save, fp)
   
  
if os.path.exists(DL_DIR):
    flush_gitclone()
flush_data()
repo_list = load_repo_list()
build_data()
build_json()