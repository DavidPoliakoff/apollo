import argparse
import warnings

import numpy as np
import pandas as pd

from sklearn.tree import DecisionTreeClassifier
from sklearn.pipeline import Pipeline
from sklearn.cross_validation import cross_val_score

import rlsl
from rlsl.pipeline import get_pipeline_steps
from rlsl.transformers import DataframePipeline, AutoDataFrameMapper

from rlsl.util.loader import PandasCaliperLoader, PandasInstructionLoader
from rlsl.util import get_train_test_inds
from rlsl.codegen import CodeGenerator
from rlsl.util.timer import Timer

description = "Model..."

CLASSIFIERS = [
    'DecisionTreeClassifier',
]

clfs = []


for c in CLASSIFIERS:
    clfs.append(globals()[c]())


def setup_parser(subparser):
    subparser.add_argument(
        '-p', '--predict', action='store', dest='predict',
        help="Select which label to predict for: policy or thread")
    subparser.add_argument(
        'files', nargs=argparse.REMAINDER, help="files containing application samples and instruction data")


def get_optimal_time(X):
    return X.groupby('problem_size')['time.duration'].sum()


def get_time(data, Xf, X, y, labelencoder, kind=None):
    train_inds, test_inds = get_train_test_inds(y)

    for i,c  in enumerate(CLASSIFIERS):
        pipeline = Pipeline([
            ('mapper', AutoDataFrameMapper()),
            ('clf', clfs[i])])

        pipeline.fit(X[train_inds], y[train_inds])

        results = pipeline.predict(X)

        resultlabels = labelencoder.get_encoder().inverse_transform(results)

        if 'policy' in kind:
            test_df = Xf[['problem_size', 'numeric_loop_id', 'policy']]
            test_df['policy'] = resultlabels
            merged_test_df = pd.merge(test_df, data[['numeric_loop_id', 'problem_size', 'policy', 'time.duration']], on=['problem_size', 'numeric_loop_id'], how='left')
            mdf = merged_test_df[(merged_test_df['policy_x'] == merged_test_df['policy_y'])]

        elif 'thread' in kind:
            test_df = Xf[['problem_size', 'numeric_loop_id', 'policy', 'num_threads']]
            test_df['num_threads'] = resultlabels
            merged_test_df = pd.merge(test_df, data[['numeric_loop_id', 'problem_size',
                                                            'num_threads',
                                                            'policy',
                                                            'time.duration']],
                                    on=['problem_size', 'numeric_loop_id', 'policy'], how='inner')
            mdf = merged_test_df[(merged_test_df['num_threads_x'] == merged_test_df['num_threads_y'])]

        elif 'chunk' in kind:
            test_df = Xf[['problem_size', 'numeric_loop_id', 'num_threads', 'chunk_size']]
            test_df['chunk_size'] = resultlabels
            merged_test_df = pd.merge(test_df, data[['numeric_loop_id', 'problem_size', 'chunk_size', 'time.duration']],
                                    on=['problem_size', 'numeric_loop_id'], how='inner')
            mdf = merged_test_df[(merged_test_df['chunk_size_x'] == merged_test_df['chunk_size_y'])]

        return mdf.groupby('problem_size')['time.duration'].sum()


def do_time(app_data, instruction_data, kind, features, interactive=True, keep_features=False):
    if keep_features:
        all_features = list(app_data) + list(instruction_data)
        steps = get_pipeline_steps(
            kind=kind, data=instruction_data,
            dropped_features=[x for x in all_features if x not in features])
    else:
        steps = get_pipeline_steps(
            kind=kind, data=instruction_data,
            dropped_features=getattr(rlsl, features))

    pipeline = DataframePipeline(steps)

    X, y = pipeline.fit_transform(app_data)
    optimal = get_optimal_time(pipeline.get_x('drop features'))
    times = get_time(pipeline.get_x('duplicates'), pipeline.get_x('drop features'), X, y, pipeline['y'], kind=kind)

    if interactive:
        print "Optimal"
        print optmal
        print "Timel"
        print times
    else:
        return (optimal, times)


def time(parser, args):
    warnings.simplefilter("ignore")

    if not args.files:
        sys.stderr.write("time requires two files of application samples\n")
        sys.exit(-1)

    app_data_name, instruction_data_name = (args.files[0], args.files[1])
    app_data, instruction_data = rlsl.util.loader.load(app_data_name, instruction_data_name)

    do_time(app_data, instruction_data, args.predict)
