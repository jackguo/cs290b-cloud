For unordered_set, when compile, need the flag:
-std=c++11

For thread, when compile, need the flag:
-std=c++0x -pthread

tips:
1: the tabusize of tabu_search_part should be less
for example, 50

multiple versions:
v0 : filp every edge (simple tabu search)
v1 : flip new edge only
v2 : flip every edge (improved: O(n^7)--> O(n^6), O(n^5))
v3 : multiple flip
v4 : 2 threaded
