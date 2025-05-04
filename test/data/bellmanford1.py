#!/usr/bin/env python

import sys
from random import randrange, sample
import numpy as np
from scipy.optimize import linprog

# ------------------------------------------------------------------------------

big = int(2e18)

def run(n, i_from, i_to, constrLE, constrGE, maxP):
    c = np.zeros(n)
    c[i_from], c[i_to] = (1, -1) if maxP else (-1, 1);

    A = []
    b = []
    for (u, v, d) in constrLE:
        row = np.zeros(n)
        row[u] = -1
        row[v] = 1
        A.append(row)
        b.append(d)
    for (u, v, d) in constrGE:
        row = np.zeros(n)
        row[u] = 1
        row[v] = -1
        A.append(row)
        b.append(-d)
    # print(A, b, c)
    res = linprog(c, A_ub=A, b_ub=b)
    if res.status == 2:
        return (-1, -1)
    elif res.status == 3:
        return (-2, 1 if maxP else -1)
    elif res.status == 0:
        return (0, round(-res.fun if maxP else res.fun))
    else:
        raise RuntimeError(f'LP failed: {res.message}')

def tmp():
    cL = [[0, 1, 10], [1, 2, 20]]
    x = run(3, 0, 2, [], cL, True)
    print(x)

def main():
    max_n = 10
    t = 3000
    print(t)
    cnt_all = 0
    cnt_n = [0] * (max_n + 1)
    cnt_rc = {0: 0, -1: 0, -2: 0}
    exp_rc = {0: 0.5, -1: 0.25, -2: 0.25}
    cnt = {}
    while cnt_all < t:
        n = randrange(2, max_n + 1)
        if cnt_all / (max_n - 1) + 4 <= cnt_n[n]:
            continue
        lim = (n * (n - 1) + 1) if n <= 3 else (n * (n - 1) // (n // 2))
        m = randrange(1, lim)
        mcL = {}
        mcR = {}
        cL = []
        cR = []
        for _m in range(m):
            u, v = sample(range(0, n), 2)
            e = randrange(0, 2)
            d = randrange(-100, 100)
            if e == 0:
                mcL[(u, v)] = d
            else:
                mcR[(u, v)] = d
        for (k, v) in mcL.items():
            cL.append((k[0], k[1], v))
        for (k, v) in mcR.items():
            cR.append((k[0], k[1], v))
        (i_from, i_to) = sample(range(0, n), 2)
        dir = randrange(0, 2)
        (rc, ans) = run(n, i_from, i_to, cL, cR, dir == 1)
        if exp_rc[rc] * cnt_all + 4 <= cnt_rc[rc]:
            continue
        cnt_all += 1
        cnt_n[n] += 1
        cnt_rc[rc] += 1
        print(n, i_from, i_to, len(cL), len(cR), dir)
        for (u, v, d) in cL:
            print(u, v, d)
        for (u, v, d) in cR:
            print(u, v, d)
        if n not in cnt:
            cnt[n] = {}
        if rc not in cnt[n]:
            cnt[n][rc] = []
        cnt[n][rc].append(m)
        print(rc, ans)
    for k in cnt:
        print(f'k={k}', file=sys.stderr)
        print(cnt[k], file=sys.stderr)

main()



