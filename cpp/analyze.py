# first, run this command in shell to generate data.
# $ ./ katago match - config match.cfg -log-file log.log
# then run this script

from scipy.spatial import distance
import tqdm
import numpy as np

import random


def parse(line):
    prefix = 'DATA_LOG:'
    line = line[line.index(prefix) + len(prefix):]
    win_prob, *owners = line.split('\t')
    win_prob = float(win_prob)
    # Board size issue in `katago match` #248
    # https://github.com/lightvector/KataGo/issues/248
    owners = [float(x) for x in owners if x if float(x)]
    assert len(owners) == 9 * 9
    return win_prob, owners


def main():
    data_raw = []
    with open('log.log') as f:
        lines = f.read().splitlines()
        for line in lines:
            if 'DATA_LOG' in line:
                data_raw.append(line)

    data = []
    for line in data_raw[:20000]:
        try:
            data.append(parse(line))
        except AssertionError:
            continue

    print('len(data):', len(data))

    # num_samples = 1000000
    # xs, ys = [], []
    # for _ in range(num_samples):
    #     (l_win, l_own), (r_win, r_own) = random.sample(data, 2)
    #     similarity = 1 - distance.cosine(l_own, r_own)
    #     diff = abs(l_win - r_win)
    #     xs.append(similarity)
    #     ys.append(diff)

    num_samples = 10000
    xs, ys = [], []
    for _ in tqdm.tqdm(range(num_samples)):
        l_win, l_own = random.choice(data)
        tops = sorted(random.sample(data, 100), key=lambda x: 1 - distance.cosine(l_own, x[1]))[:5]
        for r_win, r_own in tops:
            if abs(l_win - r_win) >= 0.0001:
                similarity = 1 - distance.cosine(l_own, r_own)
                diff = abs(l_win - r_win)
                xs.append(similarity)
                ys.append(diff)

    print(len(xs))
    # print(ys)
    print(np.corrcoef(xs, ys))


if __name__ == '__main__':
    main()
