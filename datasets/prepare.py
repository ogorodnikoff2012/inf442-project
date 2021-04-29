#!/usr/bin/python

import sys
import os.path

def process(filename):
    with open(filename) as f:
        lines = f.readlines()

    node_map = dict()
    edges = []

    for line in lines:
        if line.startswith('#'):
            continue
        edge = tuple(map(int, line.strip().split()))
        for v in edge:
            if v not in node_map:
                node_map[v] = len(node_map)
        edges.append(tuple(map(node_map.get, edge)))

    root, ext = os.path.splitext(filename)
    with open(root + os.path.extsep + "in", "w") as f:
        print(len(node_map), len(edges), file=f)
        for edge in edges:
            print(*edge, file=f)

for arg in sys.argv[1:]:
    process(arg)
