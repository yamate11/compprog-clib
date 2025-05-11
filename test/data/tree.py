#!/usr/bin/env python

import sys
from random import randrange, sample, choice
import numpy as np
from scipy.optimize import linprog

# ------------------------------------------------------------------------------

N = 2 * 10**5

def out(*line):
    print(*line, file=wfp)

with open('auto/tree1.txt', 'w') as wfp:
    out(N)
    for i in range(1, N):
        p = randrange(0, i)
        out(p + 1, i + 1)

with open('auto/tree2.txt', 'w') as wfp:
    out(N)
    nodes = [[0]]
    for i in range(1, N):
        for d in range(len(nodes) - 1, -1, -1):
            if d == 0 or randrange(0, 1000) < 500:
                break
        p = choice(nodes[d])
        out(p + 1, i + 1)
        if d == len(nodes) - 1:
            nodes.append([i])
        else:
            nodes[d + 1].append(i)

with open('auto/tree3.txt', 'w') as wfp:
    out(N)
    nodes = [[0]]
    for i in range(1, N):
        for d in range(0, len(nodes)):
            if d == len(nodes) - 1 or randrange(0, 1000) < 500:
                break
        for j in range(len(nodes[d])):
            if randrange(0, 1000) < 500:
                break
        else:
            j = 0
        p = nodes[d][j]
        out(p + 1, i + 1)
        if d == len(nodes) - 1:
            nodes.append([i])
        else:
            nodes[d + 1].append(i)
        
