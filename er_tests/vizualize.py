#!/usr/bin/python3

import numpy as np
import matplotlib.pyplot as plt
import utils

N = 1000

with open('xs.txt', 'r') as f:
    xs = np.array([float(line) for line in f.readlines()])

with open('filelist.txt', 'r') as f:
    filenames = [line.strip() for line in f.readlines()]

stats = dict()

for filename in filenames:
    data = utils.read_vars_from_file(filename)
    for k, v in data.items():
        stats.setdefault(k, [])
        stats[k].append(v)

for name, stat in stats.items():
    vals = dict()
    for piece in stat:
        for k, v in piece.items():
            vals.setdefault(k, [])
            vals[k].append(v)

    print(name)
    print(vals)

    fig = plt.figure(figsize=(16, 6))
    fig.suptitle(f"{name} ($N = {N}$)")
    ax1 = fig.add_subplot(1, 2, 1)
    ax2 = fig.add_subplot(1, 2, 2, sharey=ax1)

    avg = np.array(vals['avg'])
    err = np.array(vals['err'])
    ax1.plot(xs, avg, label='Average')
    ax1.fill_between(xs, avg + err, avg - err, label='Standard error', alpha=0.2)
    ax1.legend()
    ax1.set_xlabel('$p$')

    ax2.plot(xs, vals['q50'], label='Median')
    ax2.fill_between(xs, vals['q75'], vals['q25'], label='Interquartile range', alpha=0.2)
    ax2.plot(xs, vals['min'], ':', label='Minimum')
    ax2.plot(xs, vals['max'], ':', label='Maximum')
    ax2.legend()
    ax2.set_xlabel('$p$')

    fig.savefig(f"{name}.png", dpi=fig.dpi)
