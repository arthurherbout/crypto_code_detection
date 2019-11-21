import git
import os 
import shutil
import stat
import json
import sys

BASE_DIR = os.getcwd()
DATA_DIR = BASE_DIR + "/files"
UNTREATED_DIR = BASE_DIR+ "/untreated_crypto"
JSON_OUTPUT = BASE_DIR + "/full_data.json"


########################### UTILITY FUNCTIONS #################################


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



########################### MAIN FUNCTIONS ####################################
    

def generate_noncrypto_repos():
    """This takes in a txt file with the repos and their associated label
    and builds the repo list and the label list"""
    file = "noncrypto_repos.txt"
    repo_list = []
    with open(file, 'r') as f:
        raw_content = f.read()
    repo_list = raw_content.split("\n")
    return(repo_list)


def generate_crypto_repos(): 
    """This looks at the untreated_crypto/all_files folder and generates a nx2 
    array of username,repo_name for all the repos containing crypto we have 
    in our dataset"""
    ret_list = []
    users = os.listdir(UNTREATED_DIR + "/all_files")
    for u in users: 
        dr = UNTREATED_DIR  + "/all_files/" + u
        repos = os.listdir(dr)
        for r in repos: 
            ret_list.append((u, r))
    return ret_list
        

def add_noncrypto_repo_to_data(url):
    """This takes in a git url of a noncrypto repo and processes the repo. It 
    downloads the whole repo, then only keeps the .c and .h files from that 
    repo, and finally adds those to our data folder, and their contents to the 
    JSON, along with a "0" (noncrypto) label."""
    
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
                          "label": 0, 
                          "content": content
                          }
                data.append(dentry)
            else: 
                os.remove(path + "/" + name)
    
    # We delete all the empty directories to make the folder easier to navigate
    cleanup_repo(target_dir)
    return(data)


def add_crypto_repo_to_data(user_name, repo_name): 
    """This takes in the username/repo_name data of a crypto repo, which will 
    already be downloaded locally because of the hand-labelling we have to do. 
    It looks at both versions of the repo, the one with all files and the one
    where the crypto files have been deleted. It adds the .c and .h files to 
    the JSON with the right label (1 if the file is only in the "all_files" 
    version and 0 if the file is in both versions), then copies the full repo
    over to the data folder, while only keeping the .c and .h files."""
    
    data = []
    allfiles = UNTREATED_DIR + "/all_files/" + user_name + "/" + repo_name
    nocrypto = UNTREATED_DIR + "/no_crypto_files/" + user_name + "/" + repo_name
    
    # Patterns used to only keep C files and also to identify header files
    pattern = ('.c', '.h', '.C', '.H')
    header_pattern = ('.h', '.H')
    
    for path, subdir, files in os.walk(allfiles):
        for name in files:
            if name.endswith(pattern):
                
                # If file also exists in the repo without crypto, give label 0
                # otherwise, give label 1.
                nocrypto_path = path.replace(allfiles, nocrypto) + "/" + name
                if os.path.isfile(nocrypto_path):
                    label = 0 
                else: 
                    label = 1 
                
                # Add to data
                is_header = name.endswith(header_pattern)
                relative_path = path.replace(UNTREATED_DIR + "/all_files", '')
                
                with open(path + "/" + name, 'r', errors='replace') as f: 
                    content = f.read()
                dentry = {"data_source": "github",
                          "label": label, 
                          "file_name": name, 
                          "is_header": is_header, 
                          "source_username": user_name, 
                          "source_repo": repo_name,
                          "file_path": relative_path,
                          "content": content
                          }
                data.append(dentry)
                
                
                # Copy the file (keeping the repo structure) over to the data 
                # folder
                new_path = path.replace(UNTREATED_DIR + "/all_files", DATA_DIR)
                os.makedirs(new_path, exist_ok=True)
                shutil.copyfile(path + "/" + name, new_path + "/" + name)
    return(data)
    

def build_data_and_json(): 
    """This populates the data directory and builds the JSON at the same time"""
    flush_data()
    os.mkdir(DATA_DIR)
    json_to_save = []
    
    ### NONCRYPTO
    
    noncrypto_repos = generate_noncrypto_repos()
    crypto_repos = generate_crypto_repos()
    n = len(noncrypto_repos) + len(crypto_repos)
    
    for i in range(n): 
        
        sys.stdout.write("\rLoading repo " + str(i+1) + "/" + str(n))
        sys.stdout.flush()
        
        if i < len(noncrypto_repos): 
            repo = noncrypto_repos[i]
            dentry = add_noncrypto_repo_to_data(repo)
            
        else: 
            repo = crypto_repos[i - len(noncrypto_repos)]
            dentry = add_crypto_repo_to_data(repo[0], repo[1])
        
        json_to_save.extend(dentry)

        
    with open(JSON_OUTPUT, 'w') as fp:
        json.dump(json_to_save, fp)
    
build_data_and_json()