#!/usr/bin/python

import sys
import os.path
import tqdm

def process(filename):
    fin = open(filename)

    node_map = dict()
    edges = []

    print("Reading", filename, file=sys.stderr)
    for line in tqdm.tqdm(fin):
        if line.startswith('#'):
            continue
        edge = tuple(map(int, line.strip().split()))
        for v in edge:
            if v not in node_map:
                node_map[v] = len(node_map)
        edges.append(tuple(map(node_map.get, edge)))

    fin.close()

    root, ext = os.path.splitext(filename)
    out_filename = root + os.path.extsep + "in"
    print("Writing", out_filename, file=sys.stderr)
    with open(out_filename, "w") as f:
        print(len(node_map), len(edges), file=f)
        for edge in tqdm.tqdm(edges):
            print(*edge, file=f)

for arg in sys.argv[1:]:
    process(arg)
