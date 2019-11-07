import os
import pandas as pd

from sklearn.model_selection import train_test_split

from git_root import git_root

BASE_DIR = os.path.join(git_root(), "data")
json_path = os.path.join(BASE_DIR, "full_data.json")

df = pd.read_json(
    json_path
)

train, test, _, _ = train_test_split(
    df,
    df.loc[:, "label"],
    test_size = .15,
    stratify= df.loc[:, "label"]
)

train_source_counts = train["label"].value_counts()
test_source_counts = test["label"].value_counts()

for t in ("train", "test"):
    if t == "train":
        props = train_source_counts / train.shape[0]
    else:
        props = test_source_counts / test.shape[0]
    print(f"""
The proportions of files from each source in {t} is:
{props.round(3)}
    """)

train_path = os.path.join(BASE_DIR, "train.json")
test_path = os.path.join(BASE_DIR, "test.json")

train.to_json(train_path, orient='records')
test.to_json(test_path, orient='records')
