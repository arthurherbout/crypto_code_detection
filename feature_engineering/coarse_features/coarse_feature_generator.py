import os
import re

from functools import partial

import pandas as pd

from git_root import git_root

BASE_DIR = os.path.join(git_root(), "feature_engineering")
DATA_DIR = os.path.join(git_root(), "data")

train = pd.read_json(os.path.join(DATA_DIR, "train.json"))

content = train["content"]
train = train.drop(columns=["content"])

feature_dict = {
    "non-regex": {
        "proxy_line": "/n",
        "proxy_comment": "/*",
        "proxy_int": "int",
        "proxy_long": "long",
        "proxy_while_loops": "while",
        "proxy_for_loops": "for",
        "proxy_include": "#include",
        "proxy_bit_left_shift": "<<",
        "proxy_bit_right_shift": ">>",
        "proxy_bitwise_and": "&",
        "proxy_bitwise_or": "|",
        "proxy_complement": "~",
        "proxy_bitwise_xor": "^",
        "proxy_xor": "xor", 
        "proxy_hexadecimal": "0x",
    }#,
    #"regex": { 
        #"proxy_function_1": "^\s*(unsigned|signed)?\s+(void|int|char|short|long|float|double)\s+(\w+)\s*\([^)]*\)\s*;",
        # This next regex is from 
        # https://stackoverflow.com/questions/476173/regex-to-pull-out-c-function-prototype-declarations
        #"proxy_function_2": "^(\w+( )?){2,}\([^!@#$+%^]+?\)"
    #}
}

def count_pattern(string, pattern, regex):
    if regex:
        return len(re.findall(pattern, string))
    else:
        return string.count(pattern)

count_features = {}

def compute_count_feature(name, regex):
    global count_features
    pattern = feature_dict[regex][name]
    count_col = content.apply(
        partial(count_pattern, **{"pattern": pattern, "regex":regex})
    )
    count_features[name + "_count"] = count_col

_ = [compute_count_feature(name, regex) for regex in feature_dict for name in feature_dict[regex]]

train = train.assign(**count_features)

train = train.assign(
    proxy_loops_count=train["proxy_while_loops_count"] + train["proxy_for_loops_count"]
).drop(columns=["proxy_while_loops_count", "proxy_for_loops_count"])

