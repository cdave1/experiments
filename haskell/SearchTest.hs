-- David Petrie
--
-- Simple test of the search algorithms.
-- Should print out the following:
-- "DFS:"
-- 1 
-- 1 2 
-- 1 4 3 7 6 2 5 
-- 1 13 15 14 12 19 18 20 17 16 3 2 7 10 11 9 8 5 4 6 
-- "BFS:"
-- 1 
-- 1 2 
-- 1 2 3 4 5 6 7 
-- 1 2 3 13 7 14 15 5 8 10 12 4 11 9 19 6 18 17 20 16 

module Main where
import BFS
import DFS

emp 1 = []
emp _ = []

dbl 1 = [2]
dbl 2 = [1]
dbl _ = []

g 1 = [2,3,4]
g 2 = [5,6,3]
g 3 = [6,7]
g 5 = [7]
g _ = []

big 1 = [2,3,13]
big 2 = [7]
big 3 = []
big 4 = [5, 6, 14]
big 5 = [4, 8]
big 6 = []
big 7 = [5, 8, 10]
big 8 = [10, 11, 12]
big 9 = []
big 10 = [9, 11]
big 11 = []
big 12 = [19]
big 13 = [14,15]
big 14 = [12,15]
big 15 = []
big 16 = []
big 17 = [16, 20]
big 18 = [17, 20]
big 19 = [18]
big 20 = []
big _ = []

main = do
     print "DFS:"
     print (dfsearch emp 1)
     print (dfsearch dbl 1)
     print (dfsearch g 1)
     print (dfsearch big 1)
     print "BFS:"
     print (bfsearch emp 1)
     print (bfsearch dbl 1)
     print (bfsearch g 1)
     print (bfsearch big 1)