#!/usr/bin/env python
# coding=utf-8
# Author: Yao
# Mail: zhangyao215@mails.ucas.ac.cn

# from pycallgraph import PyCallGraph
from pycallgraph.output import GraphvizOutput

from nnunet.experiment_planning.nnUNet_plan_and_preprocess import main

graphviz = GraphvizOutput(output_file=r'./trace_detail.png')
