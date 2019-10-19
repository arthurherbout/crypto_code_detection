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
## -> Possibly facet that by distinction (which ones? SEE BELOW) 
   # by header vs. non header, important
   # crypto vs. non-crypto
# Plotting the number of comments and multiline comments (+ distribution)
   # mmkay why not but don't know how useful (maybe skip for now?)
# Plotting int counts for crypto vs non crypto (distribution?)
   # aggregated is enough (total sum per class or sth)
   # also do for `long`, can be informative maybe
# Plotting loops counts for crypto vs non crypto 
   # aggregated is enough
# Plotting bitwise counts for crypto vs non crypto (distribution?)
# Plotting top bitwise operations per category
   # don't really see why this is particularly interesting, maybe skip for now?
# Plotting number of includes for crypto vs non crypto 
   # yes!
# Give top10 include in general case, in crypto case and in non crypto case (have to code that)
   # sounds interesting

   # For most of these remove comments first to de-noise counts, using:

def remove_comments(string):
    # remove all occurrences streamed comments (/*COMMENT */) from string
    string = re.sub(re.compile(r"/\*.*?\*/",re.DOTALL ) , "" , string) 
    # remove all occurrence single-line comments (//COMMENT\n ) from string
    string = re.sub(re.compile(r"//.*?\n" ) , "" , string)
    return string

content_without_comments = content.apply(remove_comments)

   # maybe interesting to plot something else than counts for some of these
   # since counts are strongly correlated with the file size (which will be very unequal across files)
   # would be nice to have:
   #  - binary feature if there is at least one bitwise operation or not in the file (and plot number of file per class that has bitwise ops)
   #  - binary feature if there is at least one long or not in the file (and plot number of file per class that has longs)
