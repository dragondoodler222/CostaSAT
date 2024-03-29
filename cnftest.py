# def formula(a,b,c,d,e,f,g,h,i):
# 	return (a + b + d + e < 4) & (b + c + e + f < 4) & \
# 			(d + e + g + h < 4) & (e + f + h + i < 4) & \
# 			(b + c + h + i < 4) & (a + b + g + h < 4) & \
# 			(a + d + c + f < 4) & (d + g + f + i < 4) & \
# 			(a + c + g + i < 4) 
 	

# print("a b c d e f g h i R")
# for x in range(512):
# 	TUP = [int(j) for j in bin(x)[2:].rjust(9,"0")]
# 	z = formula(*TUP)
# 	if not z:continue
# 	for i in TUP:
# 		print(i, end=" ")
# 	print(z)
import itertools
import numpy as np

l = []
n = 5

IDs = np.arange(1,n**2+1).reshape((n, n)) 

for x1, x2 in itertools.combinations(np.arange(n), 2):
	for y1, y2 in itertools.combinations(np.arange(n), 2):
		l.append(f"-{IDs[x1, y1]} -{IDs[x1, y2]} -{IDs[x2, y1]} -{IDs[x2, y2]} 0")

print(f"p cnf {n**2} {len(l)}")
for i in l: print(i)