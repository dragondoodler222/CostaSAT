import sys
import numpy as np
import matplotlib.pyplot as plt

N = int(sys.argv[1])
matrix = np.zeros(N**2)

with open(sys.argv[2], "r") as file:
	while True:
		line = file.readline()
		if not line:
			break

		if line[0] == "c": continue
		if line[0] == "s": print(line) 

		if line[0] == "v":
			vals = line[2:].split(" ")
			for value in vals:
				num = int(value)
				if abs(num) > N**2 + 1:
					continue
				if num > 0:
					matrix[num - 1] = 1

plt.imshow(matrix.reshape((N, N)))
plt.show()