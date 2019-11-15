import os

import json
from jsoncomment import JsonComment

import pandas as pd

from sklearn.metrics import fbeta_score, precision_score, recall_score

from git_root import git_root

sources = ["code-jam", "crypto-library", "crypto-competitions", "others"]


def load_data():

    detector_output = git_root("models", "benchmark", "crypto-detector_output")
    
    def join_path(source):
        return os.path.join(
            detector_output, f"{source}_output.crypto"
        )

    filenames = {source: join_path(source) for source in sources}

    outputs = {source: None for source in sources}

    for source in filenames:
        with open(filenames[source]) as data_file:    
            parser = JsonComment()
            outputs[source] = parser.load(data_file)
    
    return outputs

            
def fetch_detector_outputs():
    
    outputs = load_data()
    
    # if file has not been detected by crypto-detector (ie. prediction == 0)
    # explanation will be "None"
    detector_results = {
        "data_source": [], 
        "file_name": [], 
        "prediction": [], 
        "label": []
    }

    detector_explanations = {
        "data_source": [], 
        "file_name": [], 
        "reason_for_prediction": [],
        "matched_text": [],
        "matched_line": []
    }

    def add_to_results(dictionary, **kwargs):  
        for arg, value in kwargs.items():
            dictionary[arg].append(value)

    source_to_label = {
        "code-jam": 0, "others": 0, "crypto-library": 1, "crypto-competitions": 1
    }

    explanation_dict = {
        "reason_for_prediction": "evidence_type",
        "matched_text": "matched_text",
        "matched_line": "line_text",
    }

    def create_explanation(hit):
        return {key: hit[explanation_dict[key]] for key in explanation_dict}

    # add matches
    # special care to crypto libraries that can have an intracte folder structure
    for source in outputs:
        lib = False
        if source == "crypto-library":
            lib = True
        outputs_source = outputs[source]["crypto_evidence"]
        for hashed_key in outputs_source.keys():
            match = outputs_source[hashed_key]
            matched_file_abs_path = match["file_paths"][0]
            if lib:
                path_obj = os.path.normpath(matched_file_abs_path)
                path_elements = path_obj.split(os.sep)
                # the filename will contain the whole path following the `files`
                # folder
                matched_filename = os.path.join(
                    *path_elements[path_elements.index('files')+1:]
                )
            else:
                matched_filename = os.path.basename(matched_file_abs_path)
            for hit in match["hits"]:
                add_to_results(
                    detector_explanations, 
                    **{
                        **create_explanation(hit), 
                        "data_source": source, 
                        "file_name": matched_filename
                    }
                )
            add_to_results(
                detector_results,
                **{ 
                    "data_source": source, 
                    "file_name": matched_filename, 
                    "prediction": 1, 
                    "label": source_to_label[source]
                }
            )
    
    return pd.DataFrame(detector_results), pd.DataFrame(detector_explanations)


def get_all_results(detector_results):

    # add all the other files
    all_data_path = git_root("data", "full_data.json")
    all_data = pd.read_json(all_data_path)[["data_source", "label", "file_name"]]
    all_data["prediction"] = 0

    all_results = pd.concat([detector_results, all_data], axis=0, sort=True).\
        drop_duplicates(subset=["data_source", "file_name"], keep="first")

    # sanity check
    assert(all_results.shape == all_data.shape)

    return all_results


def print_score(all_results):
    # F2 score
    score = fbeta_score(all_results["label"], all_results["prediction"], beta=2)
    print(f"The benchmark F2 score is {score:.3f}")

    # precision
    precision = precision_score(all_results["label"], all_results["prediction"])
    print(f"The benchmark precision is {precision:.3f}")

    # recall
    recall = recall_score(all_results["label"], all_results["prediction"])
    print(f"The benchmark recall is {recall:.3f}")




## for interactive debugging purposes


def filename_group_counts(df):
    return df[['data_source', 'file_name']].groupby('data_source').agg('count')

# returns file names in `source` in `df1` but not in `df2`
def filename_not_in(source, df1, df2):
    df1 = df1[df1["data_source"] == source]
    df2 = df2[df2["data_source"] == source]
    df2_filenames = df2["file_name"].values
    return df1.query("file_name not in @df2_filenames")["file_name"]

#filename_group_counts(all_data)
#filename_group_counts(all_results)

#filename_not_in("crypto-library", all_results, all_data)
