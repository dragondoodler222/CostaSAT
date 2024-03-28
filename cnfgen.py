import numpy as np 
import os
import sys
import math
import itertools
from tqdm import tqdm
import shutil

n = int(input("Enter N: "))

l = 0

def is_the_fourth_the_biggest(p1, p2, p3, p4):
	def f(x): return n*p4[0] + p4[1]
	if max(f(p1), f(p2), f(p3), (c:=f(p4))) == c:
		return True
	return False

with open("out.cnf", "w") as f:
	f.write(f"p cnf {n**2} ?\n")

	clauses = []

	IDs = np.arange(1,n**2+1).reshape((n, n)) 

	for i in range(n):
		clauses.append(" ".join(str(i) for i in IDs[i]) + " 0\n")
		l += 1

		for x, y in itertools.combinations(IDs[i],2):
			clauses.append(f"-{x} -{y} 0\n")
			l += 1

	def purge(clauses):
		f.writelines(clauses)

		clauses.clear()

	X = [(i, j) for i in range(n) for j in range(n)]

	for (p1, p2, p3) in tqdm(itertools.combinations(X, 3)):
		x1, y1 = p1
		x2, y2 = p2
		x3, y3 = p3

		if (y3 - y2 == y2 - y1 and x3 - x2 == x2 - x1) or \
			(y2 - y1 == y1 - y3 and x2 - x1 == x1 - x3) or \
			(y1 - y3 == y3 - y2 and x1 - x3 == x3 - x2):
			continue

		g = lambda ecks, why : is_the_fourth_the_biggest((x1, y1), (x2, y2), (x3, y3), (ecks, why))

		if 0 <= (x4 := x3 + x1 - x2) < n and 0 <= (y4 := y3 + y1 - y2) < n and g(x4, y4):
			l += 1
			clauses.append(f"-{IDs[x1, y1]} -{IDs[x2, y2]} -{IDs[x3, y3]} -{IDs[x4, y4]} 0\n")
		if 0 <= (x4 := x2 + x3 - x1) < n and 0 <= (y4 := y2 + y3 - y1) < n and g(x4, y4):
			l += 1
			clauses.append(f"-{IDs[x1, y1]} -{IDs[x2, y2]} -{IDs[x3, y3]} -{IDs[x4, y4]} 0\n")
		if 0 <= (x4 := x1 + x2 - x3) < n and 0 <= (y4 := y1 + y2 - y3) < n and g(x4, y4): 
			l += 1
			clauses.append(f"-{IDs[x1, y1]} -{IDs[x2, y2]} -{IDs[x3, y3]} -{IDs[x4, y4]} 0\n")

		if len(clauses) > 10000:
			purge(clauses)

print()
print(l)