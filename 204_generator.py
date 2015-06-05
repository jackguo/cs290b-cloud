#!/usr/bin/python

f = open("graph_204","w")
m = []
for i in range(0,204):
    tem = []
    for j in range(0,204):
	tem.append(0)
    m.append(tem)

	
for i in range(0,101):
    for j in range(0,101):
        p = j - i
        q = 101-j + i
        if p > 0 and p==1 or p==4 or p==5 or p==6 or p==9 or p==13 \
	          or p==14 or p==16 or p==17 or p==19 or p==20 or p==21 or p==22 or p==23 \
		  or p==24 or p==25 or p==30 or p==31 or p==33 or p==36 \
                  or p==37 or p==43 or p==45 or p==47 or p==49 \
				  or q==1 or q==4 or q==5 or q==6 or q==9 or q==13 \
	          or q==14 or q==16 or q==17 or q==19 or q==20 or q==21 or q==22 or q==23 \
		  or q==24 or q==25 or q==30 or q==31 or q==33 or q==36 \
                  or q==37 or q==43 or q==45 or q==47 or q==49:
             m[i][j] = 1
        elif p < 0:
             m[i][j] = m[j][i]
	else:
             m[i][j] = 0
for i in range(101,202):
    for j in range(101,202):
        p = j - i
        q = 101-j + i
        if p > 0 and p==1 or p==4 or p==5 or p==6 or p==9 or p==13 \
	          or p==14 or p==16 or p==17 or p==19 or p==20 or p==21 or p==22 or p==23 \
		  or p==24 or p==25 or p==30 or p==31 or p==33 or p==36 \
                  or p==37 or p==43 or p==45 or p==47 or p==49 \
				  or q==1 or q==4 or q==5 or q==6 or q==9 or q==13 \
	          or q==14 or q==16 or q==17 or q==19 or q==20 or q==21 or q==22 or q==23 \
		  or q==24 or q==25 or q==30 or q==31 or q==33 or q==36 \
                  or q==37 or q==43 or q==45 or q==47 or q==49:
             m[i][j] = 1
        elif p < 0:
             m[i][j] = m[j][i]
	else:
             m[i][j] = 0
for i in range(0,101):
    for j in range(101,202):
        p = j - i
        if m[i][j-101] == 1:
	   m[i][j] = 0
	else:
           m[i][j] = 1
for i in range(101,202):
    for j in range(0,101):
        p = j - i
        if m[i-101][j] == 1:
           m[i][j] = 0
        else:
           m[i][j] = 1
for i in range(0,101):
    m[202][i] = 1
    m[i][202] = 1
for i in range(101,202):
    m[203][i] = 1
    m[i][203] = 1
for i in range(0,204):
   for j in range(0,204):
     print>>f, m[i][j],
   print>>f, ""
f.close()
