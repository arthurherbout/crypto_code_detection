import os
from functools import partial
import pandas as pd
from git_root import git_root

BASE_DIR = os.path.join(git_root(), "feature_engineering")
DATA_DIR = os.path.join(git_root(), "data")

df = pd.read_json(os.path.join(DATA_DIR, "full_data.json"))

content = df["content"]
df = df.drop(columns=["content"])

feature_dict = {
    "proxy_line": "\n",
    "proxy_comment": "//",
    "proxy_multiline_comment": "/*",
    "proxy_int": "int",
    "proxy_while_loops": "while",
    "proxy_for_loops": "for",
    "proxy_include": "#include",
    "proxy_bit_left_shift": "<<",
    "proxy_bit_right_shift": ">>",
    "proxy_bitwise_and": "&",
    "proxy_complement": "~",
    "proxy_bitwise_xor": "^",
    "proxy_xor": "xor", 
    "proxy_hexadecimal": "0x",
}

def count_pattern(string, pattern):
    return string.count(pattern)

count_features = {}

def compute_count_feature(name):
    global count_features
    pattern = feature_dict[name]
    count_col = content.apply(
        partial(count_pattern, **{"pattern": pattern})
    )
    count_features[name + "_count"] = count_col

_ = [compute_count_feature(name) for name in feature_dict]

df = df.assign(**count_features)
loops = ["proxy_while_loops_count", "proxy_for_loops_count"]
bitwise_ops = ["proxy_bit_left_shift_count", "proxy_bit_right_shift_count", 
               "proxy_bitwise_and_count", "proxy_complement_count",
               "proxy_bitwise_xor_count", "proxy_xor_count", 
               "proxy_hexadecimal_count"]
df = df.assign(
    proxy_loops_count = sum(df[col] for col in loops),
    proxy_bitwise_count = sum(df[col] for col in bitwise_ops)
)


### Distinctions that can be made: 

# Crypto/non crypto
# Codejam vs others vs crypto comp vs crypto lib
# Header vs non header

### Plot ideas 
# Plotting the number of files in the data for each distinction
# Plotting the average number of lines (distribution of number of lines is too "squashed" to show properly)
## -> Possibly facet that by distinction (which ones?) 
# Plotting the number of comments and multiline comments (+ distribution)
# Plotting int counts for crypto vs non crypto (distribution?)
# Plotting loops counts for crypto vs non crypto 
# Plotting bitwise counts for crypto vs non crypto (distribution?)
# Plotting top bitwise operations per category
# Plotting number of includes for crypto vs non crypto 
# Give top10 include in general case, in crypto case and in non crypto case (have to code that)
