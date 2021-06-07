import matplotlib.pyplot as plt
import numpy as np
import argparse

args = argparse.ArgumentParser(description='plot search history')
args.add_argument('--stats',metavar='STATS', type=str, nargs='?', default='', help='search-points.txt')
args = args.parse_args()

index_success = []
cost_success = []
index_err = []

with open(args.stats) as f:
    lines = f.readlines()
    for line in lines:
        elements = line.split(':')
        index = int(elements[0])
        status = elements[1]
        cost = float(elements[2])
        if status == 'Success':
            index_success.append(index)
            cost_success.append(cost)
        else:
            index_err.append(index)

fig, ax = plt.subplots()            
plt.bar(index_success, cost_success)

plt.xlabel('mapping id')
plt.ylabel('edp')
#plt.title('Linear-pruned Search')
plt.title('RL Search')
ax.grid(axis='y')
plt.savefig("test.png")
plt.show()