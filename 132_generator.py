#!/usr/bin/python 

import random
from datetime import datetime

def find_group():
    int_group = [1] * 63
    previous = None
    count = 0
    while True:
        if random.random() < 0.5:
            random.seed(datetime.now())

        for index in range(1, 63):
            r = random.random()
            if r < 0.5:
                value = 1
            elif r < 0.7:
                value = 2
            elif r < 0.86:
                value = 3
            elif r < 0.9:
                value = 4
            elif r < 0.94:
                value = 5
            elif r < 0.97:
                value = 6
            elif r < 1.0:
                value = 7

            if previous == value:
                count += 1
                if count >= 7:
                    count = 0
                    previous = None
                    break

            int_group[index] = int_group[index - 1] + value

            if index == 62 and int_group[index] == 130:
                return int_group


def CliqueCount(g, gsize):
   
    count = 0
    sgsize = 7
    for i in range(0,gsize-sgsize+1):
        for j in range(i+1, gsize-sgsize+2):
            for k in range(j+1, gsize-sgsize+3):
                if g[i][j] == g[i][k] and g[i][j] == g[j][k]:
                    for l in range(k+1, gsize-sgsize+4):
                        if g[i][j] == g[i][l] and g[i][j] == g[j][l] and g[i][j] == g[k][l]:
                            for m in range(l+1, gsize-sgsize+5):
                                if g[i][j] == g[i][m] and g[i][j] == g[j][m] and g[i][j] == g[k][m] and g[i][j] == g[l][m]:
                                    for n in range(m+1, gsize-sgsize+6):
                                        if g[i][j] == g[i][n] and g[i][j] == g[j][n] and g[i][j] == g[k][n] and g[i][j] == g[l][n] and g[i][j] == g[m][n]:
                                            for o in range(n+1, gsize-sgsize+7):
                                                if g[i][j] == g[i][o] and g[i][j] == g[j][o] and g[i][j] == g[k][o] and g[i][j] == g[l][o] and g[i][j] == g[m][o] and g[i][j] == g[n][o]:
                                                    count+=1
                                                         
 
 
 
    return count

a = find_group()
f = open("graph_204","w")
m = []
for i in range(0,132):
    tem = []
    for j in range(0,132):
	tem.append(0)
    m.append(tem)

for i in range(0,132):
    for j in range(0,132):
        p = j - i
        for w in range(0,63):
          if p > 0 and p==a[w]:
             m[i][j] = 1
          elif p < 0:
             m[i][j] = m[j][i]
	  else:
             m[i][j] = 0
num = 1
while num > 0:
  num = CliqueCount(m,132)
print num
