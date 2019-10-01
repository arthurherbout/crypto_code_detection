import pandas as pd
import os 

N_SAMPLE = 100
CHDIR = 'C/Users/coren/columbia/capstone/data'

os.chdir(CHDIR)

## Getting csvs with the list of repos containing c and c++ code
os.system("pga list -l c -f csv > outputc1.csv")
os.system("pga list -l c++ -f csv > outputc2.csv")

## Importing the csvs and only keeping the repo url
dfc1 = pd.read_csv('outputc1.csv', header=None)[0]
dfc2 = pd.read_csv('outputc2.csv', header=None)[0]

repos = pd.concat([dfc1,dfc2],
                  ignore_index=True).drop_duplicates().reset_index(drop=True)

# Sampling the repos randomly
repos_sampled = repos.sample(n=N_SAMPLE)



