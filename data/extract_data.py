import git
import os 
import shutil
import stat
import json
import sys

BASE_DIR = os.getcwd()
DATA_DIR = BASE_DIR + "/files"
JSON_OUTPUT = BASE_DIR + "/full_data.json"


def generate_repos_labels():
    """This takes in a txt file with the repos and their associated label
    and builds the repo list and the label list"""
    file = "repos_labels.txt"
    repo_list = []
    label_list = []
    with open(file, 'r') as f:
        raw_content = f.read()
    content = raw_content.split("\n")
    for i in range(len(content)):
        url, label = content[i].split(",")
        repo_list.append(url)
        label_list.append(int(label))
    return(repo_list, label_list)


def del_rw(action, name, exc):
    """Utility function so that shutil can remove some directories
    (otherwise, the .git folder is read only and cause problems)"""
    os.chmod(name, stat.S_IWRITE)
    os.remove(name)

    
def flush_data():
    """This deletes the contents of the data directory if it exists, and 
    creates the data directory if it doesn't."""    
    if os.path.exists(DATA_DIR):
        shutil.rmtree(DATA_DIR, onerror=del_rw)
    

def cleanup_repo(repo_dir):
    """This cleans up a repo given by repo_dir, by deleting all its empty 
    subdirectories"""
    #TODO 
    pass


def add_repo_to_data(url, label):
    """This takes in a git url and a label and processes the repo. It downloads 
    the whole repo, then only keeps the .c and .h files from that repo, and 
    finally adds those to our data folder, and their contents to the JSON along 
    with the given label"""
    
    data = []
    repo_name = url.split("/")[-1]
    user_name = url.split("/")[-2]
    target_dir = DATA_DIR + "/" + user_name + "/" + repo_name 
    
    # We directly download the repo in the data directory so that we can keep
    # the file structure of the repo
    git.Repo.clone_from(url, target_dir)
    
    # We first delete the .git folder of the repo, which is read_only and might
    # cause problems in the future 
    shutil.rmtree(target_dir + "/.git", onerror=del_rw)
    
    pattern = ('.c', '.h', '.C', '.H')
    header_pattern = ('.h', '.H')
    # We only keep C files in our data. 
    # We delete all the files that don't have the right extension
    for path, subdir, files in os.walk(target_dir):
        for name in files:
            if name.endswith(pattern):
                is_header = name.endswith(header_pattern)
                relative_file_path = path.replace(target_dir, '')
                with open(path + "/" + name, 'r', errors='replace') as f: 
                    content = f.read()
                dentry = {"file_name": name, 
                          "is_header": is_header, 
                          "source_username": user_name, 
                          "source_repo": repo_name,
                          "file_path": relative_file_path,
                          "label": label, 
                          "content": content
                          }
                data.append(dentry)
            else: 
                os.remove(path + "/" + name)
    
    # We delete all the empty directories to make the folder easier to navigate
    cleanup_repo(target_dir)
    return(data)
    

def build_data_and_json(): 
    """This populates the data directory and builds the JSON at the same time"""
    flush_data()
    os.mkdir(DATA_DIR)
    repos, labels = generate_repos_labels()
    json_to_save = []
    n = len(repos)
    for i in range(n): 
        repo = repos[i]
        label = labels[i]
        
        sys.stdout.write("\rLoading repo " + str(i+1) + "/" + str(n))
        sys.stdout.flush()
        
        dentry = add_repo_to_data(repo, label)
        json_to_save.extend(dentry)
        
    with open(JSON_OUTPUT, 'w') as fp:
        json.dump(json_to_save, fp)
    
  
build_data_and_json()