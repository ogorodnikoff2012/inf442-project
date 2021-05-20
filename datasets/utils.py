#!/usr/bin/python3

def read_vars_from_file(filename):
    local_vars = dict()
    with open(filename, "r") as f:
        text = f.read()
        exec(text, {'__builtins__': None}, local_vars)
    return local_vars
